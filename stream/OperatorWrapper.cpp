#include "OperatorWrapper.h"

#include "Operator.h"
#include "Exception.h"
#include "Id2DataPair.h"
#include "DataContainer.h"

#include <boost/thread/thread.hpp>

namespace stream
{
    OperatorWrapper::OperatorWrapper(Operator* const op)
      : m_op(op),
        m_status(INACTIVE),
        m_inputMap(op->inputs()),
        m_outputMap(op->outputs())
    {
        if(!op)
            throw ArgumentException("Passed null pointer as operator.");
    }
    
    OperatorWrapper::~OperatorWrapper()
    {
        delete m_op;
        
        m_inputMap.clear();
        m_outputMap.clear();
    }
    
    void OperatorWrapper::testForInterrupt()
    {
        try
        {
            boost::this_thread::interruption_point();
        }
        catch(boost::thread_interrupted&)
        {
            throw InterruptException();
        } 
    }
    
    void OperatorWrapper::sleep(const unsigned int microseconds)
    {
        try
        {
            boost::this_thread::sleep(boost::posix_time::microseconds(1));
        }
        catch(boost::thread_interrupted&)
        {
            throw InterruptException();
        } 
    }
    
    void OperatorWrapper::activate()
    {
        lock_t lock(m_mutex);
        
        if(m_status != INACTIVE)
            throw InvalidStateException("Operator must be inactive.");
        
        BOOST_ASSERT(m_inputMap.isEmpty());
        BOOST_ASSERT(m_outputMap.isEmpty());
        
        m_op->activate(*this);
        m_status = ACTIVE;
    }
    
    void OperatorWrapper::deactivate()
    {
        lock_t lock(m_mutex);
        
        if(m_status == INACTIVE)
            return;
        
        if(m_status == EXECUTING)
            throw InvalidStateException("Operator can not be deactivated while it is executing.");
        
        m_op->deactivate();
        
        m_inputMap.clear();
        m_outputMap.clear();
        
        m_status = INACTIVE;
    } 
    
    void OperatorWrapper::clearAllData()
    {
        lock_t lock(m_mutex);
        
        m_inputMap.clear();
        m_outputMap.clear();
    }
    
    void OperatorWrapper::getParameter(unsigned int id, Data& value)
    {
        lock_t lock(m_executeMutex);
        
        m_op->getParameter(id, value);
    }
        
    void OperatorWrapper::setParameter(unsigned int id, const Data& value)
    {
        lock_t lock(m_executeMutex);
        
        m_op->setParameter(id, value);
    }

    void OperatorWrapper::receiveInputData(const Id2DataMapper& mapper)
    {   
        BOOST_ASSERT(m_status == EXECUTING); // this function can only be called from Operator::execute()
        
        // This function is called from Operator::execute which again is called by OperatorWrapper::execute().
        // OperatorWrapper::execute() lock the mutex which is unlocked here.
        m_mutex.unlock();
        
        bool interruptExceptionWasThrown = false;
        
        {
            unique_lock_t lock(m_mutex);
            
            try
            {
                while(! tryGet(mapper, m_inputMap))
                    waitForSignal(lock);
                
                get(mapper, m_inputMap);
            }
            catch(InterruptException&)
            {
                interruptExceptionWasThrown = true;
            }   
        }
        
        if(! interruptExceptionWasThrown)
            m_cond.notify_all();
        
        // lock again before entering Operator::execute()
        m_mutex.lock();
        
        // rethrow exception if necessary
        if(interruptExceptionWasThrown)
            throw InterruptException();
    }

    void OperatorWrapper::sendOutputData(const stream::Id2DataMapper& mapper)
    {
        BOOST_ASSERT(m_status == EXECUTING); // this function can only be called from Operator::execute()
        
        // This function is called from Operator::execute which again is called by OperatorWrapper::execute().
        // OperatorWrapper::execute() lock the mutex which is unlocked here.
        m_mutex.unlock();
        
        bool interruptExceptionWasThrown = false;
        
        {
            unique_lock_t lock(m_mutex);
            
            try
            {
                while(! trySet(mapper, m_outputMap))
                    waitForSignal(lock);
                
                set(mapper, m_outputMap);
            }
            catch(InterruptException&)
            {
                interruptExceptionWasThrown = true;
            }   
        }
        
        if(! interruptExceptionWasThrown)
            m_cond.notify_all();
        
        // lock again before entering Operator::execute()
        m_mutex.lock();
        
        // rethrow exception if necessary
        if(interruptExceptionWasThrown)
            throw InterruptException();   
    }
    
    DataContainer* const OperatorWrapper::getOutputData(const unsigned int id)
    {
        unique_lock_t lock(m_mutex);
        
        if( m_status != ACTIVE
            && m_status != EXECUTING)
        {
            throw InvalidStateException("Can not get output data if operator is inactive.");
        }
    
        while(m_outputMap[id] == 0)
        {
            // this section might throw an InterruptionException which will fall
            // through to the calling function
            if(m_status == ACTIVE)
            {
                // try to get some output data by executing
                execute();
            }
            else
            {
                // wait for the situation to change
                waitForSignal(lock);
            }
        }
        
        return m_outputMap[id];
    }

    void OperatorWrapper::setInputData(const unsigned int id, DataContainer* const data)
    {
        {
            unique_lock_t lock(m_mutex);
            
            if( m_status != ACTIVE
                && m_status != EXECUTING)
            {
                throw InvalidStateException("Can not get output data if operator is inactive.");
            }
        
            while(m_inputMap[id] != 0)
            {
                // this section might throw an InterruptionException which will fall
                // through to the calling function
                if(m_status == ACTIVE)
                {
                    // try to get some output data by executing
                    execute();
                }
                else
                {
                    // wait for the situation to change
                    waitForSignal(lock);
                }
            }
            
            data->reference();
            m_inputMap[id] = data;
        }
        
        m_cond.notify_all();
    }
    
    void OperatorWrapper::clearOutputData(unsigned int id)
    {
        {
            lock_t lock(m_mutex);
            
            if(DataContainer* data = m_outputMap[id])
                data->dereference();
                
            m_outputMap[id] = 0;
        }
        
        m_cond.notify_all();
    }
    
    void OperatorWrapper::execute()
    {
        // m_mutex is locked if this function is called
        // TODO: take care of the input and output data
        //       output data must be referenced and then input data must be
        //       dereferenced
                
        m_status = EXECUTING;
        
        bool interruptExceptionWasThrown = false;
        
        try
        {
            lock_t lock(m_executeMutex);
            m_op->execute(*this);
        }
        catch(std::exception &e)
        {
            try
            {
                dynamic_cast<InterruptException&>(e);
                interruptExceptionWasThrown = true;
            }
            catch(std::bad_cast&)
            {
            }
        }
        
        m_status = ACTIVE;
        
        if(interruptExceptionWasThrown)
            throw InterruptException();
        
        // a signal is emitted in setInputData() and getOutputData()
    }
    
    void OperatorWrapper::waitForSignal(unique_lock_t& lock)
    {
        try
        {
            m_cond.wait(lock);
        }
        catch(boost::thread_interrupted&)
        {
            throw InterruptException();
        } 
    }
    
}

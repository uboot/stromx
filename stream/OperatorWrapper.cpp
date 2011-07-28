#include "OperatorWrapper.h"

#include "Operator.h"
#include "Exception.h"
#include "Id2DataPair.h"
#include "Data.h"
#include "DataContainer.h"
#include "OperatorException.h"

#include <boost/thread/thread.hpp>

namespace stream
{
    OperatorWrapper::OperatorWrapper(Operator* const op)
      : m_op(op),
        m_status(NONE)
    {
        if(!op)
            throw ArgumentException("Passed null pointer as operator.");
    }
    
    OperatorWrapper::~OperatorWrapper()
    {
        delete m_op;
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
    
    void OperatorWrapper::initialize()
    {
        lock_t lock(m_mutex);
        
        if(m_status != NONE)
            throw InvalidStateException("Operator has already been initialized.");
        
        m_op->initialize();
        
        m_inputMap = Id2DataMap(m_op->inputs());
        m_outputMap = Id2DataMap(m_op->outputs());
        
        BOOST_ASSERT(m_inputMap.isEmpty());
        BOOST_ASSERT(m_outputMap.isEmpty());
        
        m_status = INITIALIZED;
    }
    
    
    void OperatorWrapper::activate()
    {
        lock_t lock(m_mutex);
        
        if(m_status != INITIALIZED)
            throw InvalidStateException("Operator must be initialized.");
        
        BOOST_ASSERT(m_inputMap.isEmpty());
        BOOST_ASSERT(m_outputMap.isEmpty());
        
        m_op->activate();
        m_status = ACTIVE;
    }
    
    void OperatorWrapper::deactivate()
    {
        lock_t lock(m_mutex);
        
        if(m_status == INITIALIZED)
            return;
        
        if(m_status == EXECUTING)
            throw InvalidStateException("Operator can not be deactivated while it is executing.");
        
        m_op->deactivate();
        
        m_inputMap.clear();
        m_outputMap.clear();
        
        m_status = INITIALIZED;
    }
    
    const Data& OperatorWrapper::getParameter(unsigned int id)
    {
        lock_t lock(m_executeMutex);
        
        validateParameterId(id);
        validateReadAccess(id);
        return m_op->getParameter(id);
    }
        
    void OperatorWrapper::setParameter(unsigned int id, const Data& value)
    {
        validateParameterId(id);
        validateWriteAccess(id);
        validateParameterType(id, value.type());
        
        DataType parameterType = info()->parameters()[id]->type();
        if(parameterType.is(DataType::TRIGGER))
        {
            m_op->setParameter(id, value);
        }
        else
        {
            lock_t lock(m_executeMutex);
            m_op->setParameter(id, value);
        }
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
    
    DataContainer OperatorWrapper::getOutputData(const unsigned int id)
    {
        unique_lock_t lock(m_mutex);
        
        if( m_status != ACTIVE
            && m_status != EXECUTING)
        {
            throw InvalidStateException("Can not get output data if operator is inactive.");
        }
    
        while(m_outputMap[id].empty())
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

    void OperatorWrapper::setInputData(const unsigned int id, DataContainer data)
    {
        {
            unique_lock_t lock(m_mutex);
            
            if( m_status != ACTIVE
                && m_status != EXECUTING)
            {
                throw InvalidStateException("Can not get output data if operator is inactive.");
            }
        
            while(! m_inputMap[id].empty())
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
            
            m_inputMap[id] = data;
        }
        
        m_cond.notify_all();
    }
    
    void OperatorWrapper::clearOutputData(unsigned int id)
    {
        {
            lock_t lock(m_mutex);
            
            m_outputMap[id] = DataContainer();
        }
        
        m_cond.notify_all();
    }
    
    void OperatorWrapper::execute()
    {
        // m_mutex is locked if this function is called
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
    
    void OperatorWrapper::validateParameterId(const unsigned int id)
    {
        bool isValid = false;
        for(std::vector<const Parameter*>::const_iterator iter = info()->parameters().begin();
            iter != info()->parameters().end();
            ++iter)
        {
            if((*iter)->id() == id)
            {
                isValid = true;
                break;
            }
        }
        
        if(! isValid)
            throw ParameterIdException(id, *this->info());
    }
    
    void OperatorWrapper::validateReadAccess(const unsigned int id)
    {
        const Parameter* param = info()->parameters()[id];
        
        switch(status())
        {
        case NONE:
            switch(param->accessMode())
            {
            case Parameter::NO_ACCESS:
            case Parameter::INITIALIZED_READ:
            case Parameter::INITIALIZED_WRITE:
            case Parameter::ACTIVATED_WRITE:
                throw ParameterAccessModeException(*param, *this->info());
            }
            break;
        case INITIALIZED:
            switch(param->accessMode())
            {
            case Parameter::NO_ACCESS:
                throw ParameterAccessModeException(*param, *this->info());
            }
            break;
        case ACTIVE:
        case EXECUTING:
            switch(param->accessMode())
            {
            case Parameter::NO_ACCESS:
                throw ParameterAccessModeException(*param, *this->info());
            }
            break;
        default:
            BOOST_ASSERT(false);    
        }
    }
    
    void OperatorWrapper::validateWriteAccess(const unsigned int id)
    {
        const Parameter* param = info()->parameters()[id];
        
        switch(status())
        {
        case NONE:
            switch(param->accessMode())
            {
            case Parameter::NO_ACCESS:
            case Parameter::NONE_READ:
            case Parameter::INITIALIZED_READ:
            case Parameter::INITIALIZED_WRITE:
            case Parameter::ACTIVATED_WRITE:
                throw ParameterAccessModeException(*param, *this->info());
            }
            break;
        case INITIALIZED:
            switch(param->accessMode())
            {
            case Parameter::NO_ACCESS:
            case Parameter::NONE_READ:
            case Parameter::NONE_WRITE:
            case Parameter::INITIALIZED_READ:
                throw ParameterAccessModeException(*param, *this->info());
            }
            break;
        case ACTIVE:
        case EXECUTING:
            switch(param->accessMode())
            {
            case Parameter::NO_ACCESS:
            case Parameter::NONE_READ:
            case Parameter::NONE_WRITE:
            case Parameter::INITIALIZED_READ:
            case Parameter::INITIALIZED_WRITE:
                throw ParameterAccessModeException(*param, *this->info());
            }
            break;
        default:
            BOOST_ASSERT(false);    
        }
    }
    
    void OperatorWrapper::validateParameterType(const unsigned int id, const stream::DataType& type)
    {
        const Parameter* param = info()->parameters()[id];
        if(! type.is(param->type()))
            throw ParameterTypeException(*param, *this->info());
    }   
}

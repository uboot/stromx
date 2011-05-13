#include "OperatorWrapper.h"

#include "Operator.h"
#include "Exception.h"
#include "Id2DataPair.h"

namespace stream
{
    OperatorWrapper::OperatorWrapper(Operator* const op)
      : m_op(op),
        m_status(NONE),
        m_inputMap(op->inputs()),
        m_outputMap(op->outputs()),
        m_isDeactivating(false)
    {
        if(!op)
            throw ArgumentException("Passed null pointer as operator.");
    }
    
    
    OperatorWrapper::~OperatorWrapper()
    {
        delete m_op;
    }
    
    void OperatorWrapper::activate()
    {
        lock_t lock(m_mutex);
        
        if(m_status != NONE)
            throw InvalidStateException();
        
        m_op->activate();
        m_status = ACTIVE;
    }
    
    void OperatorWrapper::deactivate()
    { 
        {
            lock_t lock(m_mutex);
            
            if(m_status == NONE)
                return;
            
            // this may improve our chances to be lock-free as soon as possible
            m_isDeactivating = true;
        }
        
        // signal all waiting operations
        m_cond.notify_all();
        
        {
            lock_t lock(m_mutex);
            m_op->deactivate();
            m_isDeactivating = false;
            m_status = ACTIVE;
        }
    }
    
    void OperatorWrapper::getParameter(unsigned int id, Data& value)
    {
        lock_t lock(m_mutex);
        
        m_op->getParameter(id, value);
    }
        
    void OperatorWrapper::setParameter(unsigned int id, const Data& value)
    {
        lock_t lock(m_mutex);
        
        m_op->setParameter(id, value);
    }

    void OperatorWrapper::receiveInputData(Id2DataPair& mapper)
    {        
        BOOST_ASSERT(m_status != NONE);
        
        // the mutex is locked because this function is called from Operator::execute()
        // This function is called by OperatorWrapper::exececute() which locks
        // the mutex
        m_mutex.unlock();
        
        {
            unique_lock_t lock(m_mutex);
            
            while(! tryGet(mapper, m_inputMap))
            {
                m_cond.wait(lock);
                if(m_isDeactivating)
                    throw IsDeactivatingException();
            }
            
            get(mapper, m_inputMap);
        }
        
        m_cond.notify_all();
        
        // lock again before entering Operator::execute()
        m_mutex.lock();
    }
    
    DataContainer* const OperatorWrapper::getOutputData(const unsigned int id)
    {
        DataContainer* value = 0;
        
        unique_lock_t lock(m_mutex);
        BOOST_ASSERT(m_status != NONE);
        
        {
            while(m_outputMap[id] == 0)
            {
                // try to get some output data by executing
                if(m_status == ACTIVE)
                    execute();
                else
                    m_cond.wait(lock);
            }
            
            value = m_outputMap[id];
        }
        
        m_cond.notify_all();
        
        return value;
    }

    void OperatorWrapper::sendOutputData(stream::Id2DataPair& mapper)
    {
        BOOST_ASSERT(m_status != NONE);
        
        // the mutex is locked because this function is called from Operator::execute()
        // This function is called by OperatorWrapper::exececute() which locks
        // the mutex
        m_mutex.unlock();
        
        {
            unique_lock_t lock(m_mutex);
            
            while(! trySet(mapper, m_outputMap))
            {
                m_cond.wait(lock);
                if(m_isDeactivating)
                    throw IsDeactivatingException();
            }
            
            set(mapper, m_outputMap);
        }
        
        m_cond.notify_all();
        
        // lock again before entering Operator::execute()
        m_mutex.lock();
    }

    void OperatorWrapper::clearOutputData(unsigned int id)
    {
        lock_t lock(m_mutex);
        
        m_outputMap[id] = 0;
    }

    void OperatorWrapper::setInputData(const unsigned int id, DataContainer* const data)
    {
        {
            unique_lock_t lock(m_mutex);
            BOOST_ASSERT(m_status != NONE);
            
            while(m_inputMap[id])
            {
                // try to remove some input data by executing
                if(m_status == ACTIVE)
                    execute();
                else
                    m_cond.wait(lock);
            }
            
            m_inputMap[id] = data;
        }
        
        m_cond.notify_all();
    }
    
    void OperatorWrapper::execute()
    {
        // m_mutex is locked if this function is called
        // TODO: take care of the input and output data
        //       output data must be referenced and then input data must be
        //       dereferenced
        m_op->execute(*this);
        
    }

}

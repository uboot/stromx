#include "OperatorWrapper.h"

#include "Operator.h"
#include "Exception.h"
#include "Id2DataPair.h"
#include "DataContainer.h"

namespace stream
{
    OperatorWrapper::OperatorWrapper(Operator* const op)
      : m_op(op),
        m_status(INACTIVE),
        m_inputMap(op->inputs()),
        m_outputMap(op->outputs()),
        m_isDeactivating(false)
    {
        if(!op)
            throw ArgumentException("Passed null pointer as operator.");
    }
    
    DataContainer* const OperatorWrapper::createDataContainer(Data* const data)
    {
        return new DataContainer(data);
    }
    
    OperatorWrapper::~OperatorWrapper()
    {
        delete m_op;
    }
    
    void OperatorWrapper::activate()
    {
        lock_t lock(m_mutex);
        
        if(m_status != INACTIVE)
            throw InvalidStateException("Operator must be inactive.");
        
        m_op->activate(*this);
        m_status = ACTIVE;
    }
    
    void OperatorWrapper::startDeactivating()
    { 
        {
            lock_t lock(m_mutex);
            
            if(m_status == INACTIVE)
                return;
            
            if(m_status == ACTIVE)
                m_status = DEACTIVATING;
            else if(m_status == EXECUTING)
                m_status = DEACTIVATING_AND_EXECUTING;
        }
        
        // signal all waiting operations
        m_cond.notify_all();
    }
    
    void OperatorWrapper::joinDeactivating()
    {
        unique_lock_t lock(m_mutex);
            
        if(m_status == INACTIVE)
            return;
        
        if( m_status != DEACTIVATING
            && m_status != DEACTIVATING_AND_EXECUTING)
        {
            throw InvalidStateException("Operator must be deactivating.");
        }
        
        // wait for any active execute to finish
        while(m_status == DEACTIVATING_AND_EXECUTING)
            m_cond.wait(lock);
        
        m_op->deactivate();
        m_status = INACTIVE;
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

    void OperatorWrapper::receiveInputData(const Id2DataMapper& mapper)
    {   
        if(m_status != EXECUTING)
            throw InvalidStateException("Operator must be executing.");
        
        // the mutex is locked because this function is called from Operator::execute()
        // This function is called by OperatorWrapper::exececute() which locks
        // the mutex
        m_mutex.unlock();
        
        {
            unique_lock_t lock(m_mutex);
            
            while(! tryGet(mapper, m_inputMap))
            {
                m_cond.wait(lock);
                if( m_status == DEACTIVATING
                    || m_status == DEACTIVATING_AND_EXECUTING)
                {
                    throw IsDeactivatingException("Operator is deactivating");
                }
            }
            
            get(mapper, m_inputMap);
        }
        
        m_cond.notify_all();
        
        // lock again before entering Operator::execute()
        m_mutex.lock();
    }

    void OperatorWrapper::sendOutputData(const stream::Id2DataMapper& mapper)
    {
        if(m_status != EXECUTING)
            throw InvalidStateException("Operator must be executing.");
        
        // the mutex is locked because this function is called from Operator::execute()
        // This function is called by OperatorWrapper::exececute() which locks
        // the mutex
        m_mutex.unlock();
        
        {
            unique_lock_t lock(m_mutex);
            
            while(! trySet(mapper, m_outputMap))
            {
                m_cond.wait(lock);
                if( m_status == DEACTIVATING
                    || m_status == DEACTIVATING_AND_EXECUTING)
                {
                    throw IsDeactivatingException("Operator is deactivating");
                }
            }
            
            set(mapper, m_outputMap);
        }
        
        m_cond.notify_all();
        
        // lock again before entering Operator::execute()
        m_mutex.lock();
    }
    
    DataContainer* const OperatorWrapper::getOutputData(const unsigned int id)
    {
        DataContainer* data = 0;
        
        {
            unique_lock_t lock(m_mutex);
            
            if(isDeactivating())
            {
                throw IsDeactivatingException("Operator is deactivating");
            }
            
            if( m_status != ACTIVE
                && m_status != EXECUTING)
            {
                throw InvalidStateException("Can not get output data if operator is inactive.");
            }
        
            while(m_outputMap[id] == 0 && ! isDeactivating())
            {
                // try to get some output data by executing
                if(m_status == ACTIVE)
                {
                    execute();
                }
                else
                {
                    m_cond.wait(lock);
                    if( isDeactivating())
                        throw IsDeactivatingException("Operator is deactivating");
                }
            }
            
            if(m_outputMap[id] != 0)
                data = m_outputMap[id];
        }
        
        m_cond.notify_all();
        
        return data;
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
            
            if(isDeactivating())
            {
                throw IsDeactivatingException("Operator is deactivating");
            }
            
            if( m_status != ACTIVE
                && m_status != EXECUTING)
            {
                throw InvalidStateException("Can not get output data if operator is inactive.");
            }
        
            while(m_inputMap[id] != 0 && ! isDeactivating())
            {
                // try to get some output data by executing
                if(m_status == ACTIVE)
                {
                    execute();
                }
                else
                {
                    m_cond.wait(lock);
                    if( isDeactivating())
                        throw IsDeactivatingException("Operator is deactivating");
                }
            }
            
            if(m_inputMap[id] == 0)
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
        m_status = EXECUTING;
        
        try
        {
            m_op->execute(*this);
        }
        catch(std::exception &e)
        {
        }
        
        if(m_status == EXECUTING)
            m_status = ACTIVE;
        else if(m_status == DEACTIVATING_AND_EXECUTING)
            m_status = DEACTIVATING;
        
        // a signal is emitted in setInputData() and getOutputData()
    }

}

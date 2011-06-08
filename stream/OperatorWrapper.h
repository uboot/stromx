#ifndef STREAM_OPERATORWRAPPER_H
#define STREAM_OPERATORWRAPPER_H

#include "DataProvider.h"
#include "OperatorInterface.h"
#include "Id2DataMap.h"
#include "Parameter.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include "Operator.h"


namespace stream
{
    class Operator;
    class Data;
    
    class OperatorWrapper : public DataProvider, public OperatorInterface
    {
    public:
        enum Status
        {
            INACTIVE,
            ACTIVE,
            EXECUTING
        };
        
        OperatorWrapper(Operator* const op);
        virtual ~OperatorWrapper();
        
        const OperatorInfo* const info() const { return m_op; }
        
        void activate();
        void deactivate();
        
        // DataProvider implementation
        void receiveInputData(const Id2DataMapper& mapper);
        void sendOutputData(const Id2DataMapper& mapper);
        void testForInterrupt();
        void sleep(const unsigned int microseconds);
        
        // OperatorInterface implementation
        const Status status() { return m_status; }
        
        void setParameter(unsigned int id, const Data& value);
        void getParameter(unsigned int id, Data& value);
        
        DataContainer* const getOutputData(const unsigned int id);
        void setInputData(const unsigned int id, DataContainer* const data);
        void clearOutputData(unsigned int id);
        void clearAllData();
        
    private:
        typedef boost::lock_guard<boost::mutex> lock_t;
        typedef boost::unique_lock<boost::mutex> unique_lock_t;
        
        void execute();
        void waitForSignal(unique_lock_t& lock);
        
        Operator* m_op;
        Status m_status;
        boost::condition_variable_any m_cond;
        boost::mutex m_mutex;
        boost::mutex m_executeMutex;
        Id2DataMap m_inputMap;
        Id2DataMap m_outputMap;
    };
}

#endif // STREAM_OPERATORWRAPPER_H

#ifndef STREAM_OPERATORWRAPPER_H
#define STREAM_OPERATORWRAPPER_H

#include "DataProvider.h"
#include "Id2DataMap.h"

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition_variable.hpp>


namespace stream
{
    class Operator;
    class Data;
    
    class OperatorWrapper : public DataProvider
    {
    public:
        enum Status
        {
            NONE,
            ACTIVE,
            EXECUTING
        };
        
        OperatorWrapper(Operator* const op);
        virtual ~OperatorWrapper();
        
        Operator& op() { return *m_op; }
        const Status status() { return m_status; }
        
        void setParameter(unsigned int id, const Data& value);
        void getParameter(unsigned int id, Data& value);
        
        const bool isDeactivating() { return m_isDeactivating; }
        virtual void receiveInputData(Id2DataPair& mapper);
        virtual void sendOutputData(Id2DataPair& mapper);
        DataContainer* const getOutputData(const unsigned int id);
        void setInputData(const unsigned int id, DataContainer* const data);
        virtual void clearOutputData(unsigned int id);
        virtual void activate();
        virtual void deactivate();
        
    private:
        typedef boost::lock_guard<boost::mutex> lock_t;
        typedef boost::unique_lock<boost::mutex> unique_lock_t;
        
        void execute();
        
        Operator* m_op;
        Status m_status;
        boost::mutex m_mutex;
        boost::condition_variable_any m_cond;
        Id2DataMap m_inputMap;
        Id2DataMap m_outputMap;
        bool m_isDeactivating;
    };
}

#endif // STREAM_OPERATORWRAPPER_H

#ifndef STREAM_OPERATORWRAPPER_H
#define STREAM_OPERATORWRAPPER_H

#include "DataProvider.h"
#include "Id2DataMap.h"
#include "Parameter.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include "Operator.h"


namespace stream
{
    class Operator;
    class Data;
    
    class OperatorWrapper : public DataProvider
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
        
        const Status status() { return m_status; }
        virtual void testForInterrupt();
        
        const std::vector<Description>& inputs() { return m_op->inputs(); }
        const std::vector<Description>& outputs() { return m_op->outputs(); }
        const std::vector<Parameter>& parameters() { return m_op->parameters(); }
        
        void setParameter(unsigned int id, const Data& value);
        void getParameter(unsigned int id, Data& value);
        
        virtual void receiveInputData(const Id2DataMapper& mapper);
        virtual void sendOutputData(const Id2DataMapper& mapper);
        
        DataContainer* const getOutputData(const unsigned int id);
        void setInputData(const unsigned int id, DataContainer* const data);
        virtual void clearOutputData(unsigned int id);
        
        virtual void activate();
        virtual void deactivate();
        
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

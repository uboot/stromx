#ifndef STREAM_OPERATORWRAPPER_H
#define STREAM_OPERATORWRAPPER_H

#include "DataProvider.h"

namespace stream
{
    class Operator;
    class Data;
    
    class OperatorWrapper : public DataProvider
    {
    public:
        enum Status
        {
            IDLE,
            EXECUTING
        };
        
        OperatorWrapper(Operator* const op);
        virtual ~OperatorWrapper();
        
        void setParameter(unsigned int id, const Data& value);
        void getParameter(unsigned int id, Data& value);
        void setInputData(unsigned int id, DataContainer* const data);
        DataContainer* const getOutputData(unsigned int id);
        void clearOutputData(unsigned int id);
        
        const bool isStopped() { return false; }
        void receiveInputData(Id2DataMapper& mapper) {}
        void sendOutputData(const Id2DataMapper& mapper) {} 
    };
}

#endif // STREAM_OPERATORWRAPPER_H

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
        void setInputData();
        void getOutputData();
        void clearOutputData();
        
        const bool isStopped() { return false; }
        void receiveInputData() {}
        void sendOutputData() {}
        
    };
}

#endif // STREAM_OPERATORWRAPPER_H

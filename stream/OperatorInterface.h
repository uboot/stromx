#ifndef STREAM_OPERATORINTERFACE_H
#define STREAM_OPERATORINTERFACE_H

#include "Id2DataMap.h"
#include "Parameter.h"


namespace stream
{
    class OperatorInfo;
    class Data;
    
    class OperatorInterface
    {
    public:
        enum Status
        {
            NONE,
            INITIALIZED,
            ACTIVE,
            EXECUTING
        };
        
        virtual ~OperatorInterface() {}
        
        virtual const Status status() = 0;
        virtual const OperatorInfo* const info() const = 0;
        
        virtual void setParameter(unsigned int id, const Data& value) = 0;
        virtual const Data& getParameter(unsigned int id) = 0;
        
        virtual DataContainer getOutputData(const unsigned int id) = 0;
        virtual void setInputData(const unsigned int id, DataContainer data) = 0;
        virtual void clearOutputData(unsigned int id) = 0;
        
        virtual void activate() = 0;
        virtual void deactivate() = 0;
    };
}

#endif // STREAM_OPERATORINTERFACE_H

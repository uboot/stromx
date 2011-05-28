#ifndef STREAM_OPERATORINTERFACE_H
#define STREAM_OPERATORINTERFACE_H

#include "Id2DataMap.h"
#include "Parameter.h"


namespace stream
{
    class Operator;
    class Data;
    
    class OperatorInterface
    {
    public:
        virtual ~OperatorInterface() {}
        
        virtual const std::vector<Description>& inputs() = 0;
        virtual const std::vector<Description>& outputs() = 0;
        virtual const std::vector<Parameter>& parameters() = 0;
        
        virtual void setParameter(unsigned int id, const Data& value) = 0;
        virtual void getParameter(unsigned int id, Data& value) = 0;
        
        virtual DataContainer* const getOutputData(const unsigned int id) = 0;
        virtual void setInputData(const unsigned int id, DataContainer* const data) = 0;
        virtual void clearOutputData(unsigned int id) = 0;
    };
}

#endif // STREAM_OPERATORINTERFACE_H

#ifndef STREAM_OPERATOR_H
#define STREAM_OPERATOR_H

#include "Parameter.h"
#include "Version.h"

namespace stream
{
    class Data;
    class DataContainer;
    class DataProvider;
    
    class Operator
    {
    public:
        Operator(const std::string & name, const Version & version);
        
        DataContainer* const createDataContainer(Data* const data);
        
        virtual void setParameter(unsigned int id, const Data& value) = 0;
        virtual void getParameter(unsigned int id, Data& value) = 0;
        
        virtual void execute(DataProvider& provider) = 0;
        virtual void activate() {}
        virtual void deactivate() {}
        
    private:
        std::string m_name;
        Version m_version;
        
    };
}

#endif // STREAM_OPERATOR_H
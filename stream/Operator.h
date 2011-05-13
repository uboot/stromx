#ifndef STREAM_OPERATOR_H
#define STREAM_OPERATOR_H

#include "Parameter.h"
#include "Version.h"

#include <vector>

namespace stream
{
    class Data;
    class DataContainer;
    class DataProvider;
    
    class Operator
    {
    public:
        Operator(const std::string & name, const Version & version);
        
        const std::string& name() { return m_name; }
        const Version& version() { return m_version; }
        const std::vector<Descriptor>& inputs() { return m_inputs; }
        const std::vector<Descriptor>& outputs() { return m_outputs; }
        const std::vector<Parameter>& parameters() { return m_parameters; }
        
        DataContainer* const createDataContainer(Data* const data);
        
        virtual void setParameter(unsigned int id, const Data& value) = 0;
        virtual void getParameter(unsigned int id, Data& value) = 0;
        
        virtual void execute(DataProvider& provider) = 0;
        virtual void activate() {}
        virtual void deactivate() {}
        
    private:
        std::string m_name;
        Version m_version;
        std::vector<Descriptor> m_inputs;
        std::vector<Descriptor> m_outputs;
        std::vector<Parameter> m_parameters;
        
    };
}

#endif // STREAM_OPERATOR_H
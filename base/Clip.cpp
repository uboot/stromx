#include "Clip.h"



namespace base
{
    const std::string Clip::NAME("Clip");
    const std::string Clip::PACKAGE("Base");
    const stream::Version Clip::VERSION(0, 1);
    
    Clip::Clip()
    : Operator(NAME, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
    {
    }

    void Clip::setParameter(unsigned int id, const stream::Data& value)
    {
    }

    void Clip::getParameter(unsigned int id, stream::Data& value)
    {
    }  
    
    void Clip::execute(stream::DataProvider& provider)
    {
    }

    
    const std::vector< stream::Description > Clip::setupInputs()
    {
        std::vector<stream::Description> inputs;
        
        return inputs;
    }
    
    const std::vector< stream::Description > Clip::setupOutputs()
    {
        std::vector<stream::Description> outputs;
        
        return outputs;
    }
    
    const std::vector< stream::Parameter > Clip::setupParameters()
    {
        std::vector<stream::Parameter> parameters;
                                    
        return parameters;
    }
} 

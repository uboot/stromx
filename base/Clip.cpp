#include "Clip.h"

#include "Config.h"

#include <stream/NumericParameter.h>
#include <stream/Primitive.h>


namespace base
{
    const std::string Clip::NAME("Clip");
    const std::string Clip::PACKAGE("Base");
    const stream::Version Clip::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
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
        
        inputs.push_back(stream::Description("Input", INPUT, stream::DataType(stream::DataType::IMAGE)));
        
        return inputs;
    }
    
    const std::vector< stream::Description > Clip::setupOutputs()
    {
        std::vector<stream::Description> outputs;
        
        outputs.push_back(stream::Description("Output", OUTPUT, stream::DataType(stream::DataType::IMAGE)));
        
        return outputs;
    }
    
    const std::vector< stream::Parameter > Clip::setupParameters()
    {
        std::vector<stream::Parameter> parameters;
        
        parameters.push_back(stream::NumericParameter<stream::UInt32>(
            "Top", TOP, stream::DataType::UINT_32,
            stream::Parameter::READ_WRITE, stream::Parameter::READ_WRITE));
        
        parameters.push_back(stream::NumericParameter<stream::UInt32>(
            "Left", LEFT, stream::DataType::UINT_32,
            stream::Parameter::READ_WRITE, stream::Parameter::READ_WRITE));
        
        parameters.push_back(stream::NumericParameter<stream::UInt32>(
            "Width", WIDTH, stream::DataType::UINT_32,
            stream::Parameter::READ_WRITE, stream::Parameter::READ_WRITE));
        
        parameters.push_back(stream::NumericParameter<stream::UInt32>(
            "Height", HEIGHT, stream::DataType::UINT_32,
            stream::Parameter::READ_WRITE, stream::Parameter::READ_WRITE));
                                    
        return parameters;
    }
} 

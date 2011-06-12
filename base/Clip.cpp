 #include "Clip.h"

#include "Config.h"

#include <stream/NumericParameter.h>
#include <stream/Primitive.h>

using namespace stream;

namespace base
{
    const std::string Clip::NAME("Clip");
    
    const std::string Clip::PACKAGE(PACKAGE_NAME);
    const Version Clip::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    Clip::Clip()
    : Operator(NAME, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
    {
    }

    void Clip::setParameter(unsigned int id, const Data& value)
    {
    }

    void Clip::getParameter(unsigned int id, Data& value)
    {
    }  
    
    void Clip::execute(DataProvider& provider)
    {
    }

    
    const std::vector< Description > Clip::setupInputs()
    {
        std::vector<Description> inputs;
        
        Description input(INPUT);
        input.setName("Input");
        input.setType(DataType::IMAGE);
        inputs.push_back(input);
        
        return inputs;
    }
    
    const std::vector< stream::Description > Clip::setupOutputs()
    {
        std::vector<Description> outputs;
        
        Description output(OUTPUT);
        output.setName("Output");
        output.setType(DataType::IMAGE);
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector< stream::Parameter > Clip::setupParameters()
    {
        std::vector<stream::Parameter> parameters;
        
        Parameter top(TOP);
        top.setName("Top");
        top.setType(DataType::UINT_32);
        top.setInactiveAccessMode(stream::Parameter::READ_WRITE);
        top.setActiveAccessMode(stream::Parameter::READ_WRITE);
        parameters.push_back(top);
        
        Parameter left(LEFT);
        left.setName("Left");
        left.setType(DataType::UINT_32);
        left.setInactiveAccessMode(stream::Parameter::READ_WRITE);
        left.setActiveAccessMode(stream::Parameter::READ_WRITE);
        parameters.push_back(top);
        
        Parameter width(WIDTH);
        width.setName("Width");
        width.setType(DataType::UINT_32);
        width.setInactiveAccessMode(stream::Parameter::READ_WRITE);
        width.setActiveAccessMode(stream::Parameter::READ_WRITE);
        parameters.push_back(top);
        
        Parameter height(HEIGHT);
        height.setName("Height");
        height.setType(DataType::UINT_32);
        height.setInactiveAccessMode(stream::Parameter::READ_WRITE);
        height.setActiveAccessMode(stream::Parameter::READ_WRITE);
        parameters.push_back(top);
                                    
        return parameters;
    }
} 

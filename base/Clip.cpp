#include "Clip.h"

#include "Config.h"
#include "Image.h"
#include "Utilities.h"

#include <stream/NumericParameter.h>
#include <stream/Primitive.h>
#include <stream/OperatorException.h>
#include <stream/DataContainer.h>
#include <stream/DataProvider.h>
#include <stream/WriteAccess.h>
#include <stream/Id2DataPair.h>

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
        try
        {
            switch(id)
            {
            case TOP:
                m_top = dynamic_cast<const UInt32&>(value);
                break;
            case LEFT:
                m_left = dynamic_cast<const UInt32&>(value);
                break;
            case WIDTH:
                m_width = dynamic_cast<const UInt32&>(value);
                break;
            case HEIGHT:
                m_height = dynamic_cast<const UInt32&>(value);
                break;
            default:
                throw ParameterIdException(id, *this);
            }
        }
        catch(std::bad_cast&)
        {
            throw ParameterTypeException(*parameters()[id], *this);
        }
    }

    const Data& Clip::getParameter(unsigned int id)
    {
        switch(id)
        {
        case TOP:
            return m_top;
        case LEFT:
            return m_left;
        case WIDTH:
            return m_width;
        case HEIGHT:
            return m_height;
        default:
            throw ParameterIdException(id, *this);
        }
    }  
    
    void Clip::execute(DataProvider& provider)
    {
        Id2DataPair inputDataMapper(INPUT);
        provider.receiveInputData(inputDataMapper);
        
        DataContainer container = inputDataMapper.data();
        WriteAccess access(container);
        Image* image = dynamic_cast<Image*>(access());
        
        unsigned int top = m_top;
        unsigned int left = m_left;
        unsigned int height = m_height;
        unsigned int width = m_width;
        
        adjustClipRegion(image->width(), image->height(), left, top, width, height);
        
        image->setWidth(width);
        image->setHeight(height);
        image->setData(image->data() + top * image->stride() + left * image->pixelSize());
        
        Id2DataPair outputDataMapper(OUTPUT, container);
        provider.sendOutputData( outputDataMapper);
    }
    
    const std::vector< Description* > Clip::setupInputs()
    {
        std::vector<Description*> inputs;
        
        Description* input = new Description(INPUT, DataType::IMAGE);
        input->setName("Input");
        inputs.push_back(input);
        
        return inputs;
    }
    
    const std::vector< stream::Description* > Clip::setupOutputs()
    {
        std::vector<Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataType::IMAGE);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector< stream::Parameter* > Clip::setupParameters()
    {
        std::vector<stream::Parameter*> parameters;
        
        NumericParameter<UInt32>* top = new NumericParameter<UInt32>(TOP, DataType::UINT_32);
        top->setName("Top");
        top->setInactiveAccessMode(stream::Parameter::READ_WRITE);
        top->setActiveAccessMode(stream::Parameter::READ_WRITE);
        parameters.push_back(top);
        
        NumericParameter<UInt32>* left = new NumericParameter<UInt32>(LEFT, DataType::UINT_32);
        left->setName("Left");
        left->setInactiveAccessMode(stream::Parameter::READ_WRITE);
        left->setActiveAccessMode(stream::Parameter::READ_WRITE);
        parameters.push_back(left);
        
        NumericParameter<UInt32>* width = new NumericParameter<UInt32>(WIDTH, DataType::UINT_32);
        width->setName("Width");
        width->setInactiveAccessMode(stream::Parameter::READ_WRITE);
        width->setActiveAccessMode(stream::Parameter::READ_WRITE);
        parameters.push_back(width);
        
        NumericParameter<UInt32>* height = new NumericParameter<UInt32>(HEIGHT, DataType::UINT_32);
        height->setName("Height");
        height->setInactiveAccessMode(stream::Parameter::READ_WRITE);
        height->setActiveAccessMode(stream::Parameter::READ_WRITE);
        parameters.push_back(height);
                                    
        return parameters;
    }
    
    void Clip::adjustClipRegion(const unsigned int destWidth, const unsigned int destHeight, unsigned int& left, unsigned int& top, unsigned int& width, unsigned int& height)
    {
         left = std::min(left, destWidth);
         width = std::min(width, destWidth - left);
         top = std::min(top, destHeight);
         height = std::min(height, destHeight - top);
    }
} 

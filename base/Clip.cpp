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
    const std::string Clip::TYPE("Clip");
    
    const std::string Clip::PACKAGE(PACKAGE_NAME);
    const Version Clip::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    Clip::Clip()
      : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
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
                throw WrongParameterId(id, *this);
            }
        }
        catch(std::bad_cast&)
        {
            throw WrongParameterType(parameter(id), *this);
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
            throw WrongParameterId(id, *this);
        }
    }  
    
    void Clip::execute(DataProvider& provider)
    {
        Id2DataPair inputDataMapper(INPUT);
        provider.receiveInputData(inputDataMapper);
        
        DataContainer container = inputDataMapper.data();
        WriteAccess access(container);
        Image& image = dynamic_cast<Image&>(access());
        
        unsigned int top = m_top;
        unsigned int left = m_left;
        unsigned int height = m_height;
        unsigned int width = m_width;
        
        adjustClipRegion(image.width(), image.height(), left, top, width, height);
        
        uint8_t* data = image.data() + top * image.stride() + left * image.pixelSize();
        image.initialize(width, height, image.stride(), data, image.pixelType());
        
        Id2DataPair outputDataMapper(OUTPUT, container);
        provider.sendOutputData( outputDataMapper);
    }
    
    const std::vector<const stream::Description*> Clip::setupInputs()
    {
        std::vector<const Description*> inputs;
        
        Description* input = new Description(INPUT, DataVariant::IMAGE);
        input->setName("Input");
        inputs.push_back(input);
        
        return inputs;
    }
    
    const std::vector<const Description*> Clip::setupOutputs()
    {
        std::vector<const Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataVariant::IMAGE);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector<const Parameter*> Clip::setupParameters()
    {
        std::vector<const stream::Parameter*> parameters;
        
        NumericParameter<UInt32>* top = new NumericParameter<UInt32>(TOP, DataVariant::UINT_32);
        top->setName("Top");
        top->setAccessMode(stream::Parameter::ACTIVATED_WRITE);
        parameters.push_back(top);
        
        NumericParameter<UInt32>* left = new NumericParameter<UInt32>(LEFT, DataVariant::UINT_32);
        left->setName("Left");
        left->setAccessMode(stream::Parameter::ACTIVATED_WRITE);
        parameters.push_back(left);
        
        NumericParameter<UInt32>* width = new NumericParameter<UInt32>(WIDTH, DataVariant::UINT_32);
        width->setName("Width");
        width->setAccessMode(stream::Parameter::ACTIVATED_WRITE);
        parameters.push_back(width);
        
        NumericParameter<UInt32>* height = new NumericParameter<UInt32>(HEIGHT, DataVariant::UINT_32);
        height->setName("Height");
        height->setAccessMode(stream::Parameter::ACTIVATED_WRITE);
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

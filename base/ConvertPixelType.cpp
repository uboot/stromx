#include "ConvertPixelType.h"

#include "Config.h"
#include "Image.h"
#include "Utilities.h"

#include <stream/DataContainer.h>
#include <stream/DataProvider.h>
#include <stream/Id2DataPair.h>
#include <stream/OperatorException.h>
#include <stream/EnumParameter.h>

using namespace stream;

namespace base
{
    const std::string ConvertPixelType::NAME("Convert pixel type");
    
    const std::string ConvertPixelType::PACKAGE(PACKAGE_NAME);
    const Version ConvertPixelType::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    ConvertPixelType::ConvertPixelType()
      : Operator(NAME, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
        m_pixelType(stream::Image::MONO_8),
        m_image(0)
    {
    }

    void ConvertPixelType::setParameter(unsigned int id, const Data& value)
    {
        try
        {
            switch(id)
            {
            case PIXEL_TYPE:
                m_pixelType = dynamic_cast<const Enum &>(value);
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

    const Data& ConvertPixelType::getParameter(unsigned int id)
    {
        switch(id)
        {
        case PIXEL_TYPE:
            return m_pixelType;
        default:
            throw ParameterIdException(id, *this);
        }
    }  
    
    void ConvertPixelType::execute(DataProvider& provider)
    {
        Id2DataPair inputDataMapper(INPUT);
        provider.receiveInputData(inputDataMapper);
        
        const Data* inData = inputDataMapper.data()->getReadAccess();
        const Image* inImage = dynamic_cast<const Image*>(inData);
        
        stream::Image::PixelType outPixelType = stream::Image::PixelType((unsigned int)(m_pixelType));
        adjustImage(inImage->width(), inImage->height(), outPixelType, this, m_image);
        
        Image* outImage = dynamic_cast<Image*>(m_image->getWriteAccess());
        
        cv::Mat inCvImage = getOpenCvMat(*inImage);
        cv::Mat outCvImage = getOpenCvMat(*outImage);
        
        if(outPixelType == inImage->pixelType())
        {
            inCvImage.copyTo(outCvImage);
        }
        else
        {
            int code = getCvConversionCode(inImage->pixelType(), outPixelType);
            cv::cvtColor(inCvImage, outCvImage, code);
        }
        
        m_image->clearWriteAccess();
        Id2DataPair outputDataMapper(OUTPUT, m_image);
        provider.sendOutputData( outputDataMapper);
    }
    
    const std::vector< Description* > ConvertPixelType::setupInputs()
    {
        std::vector<Description*> inputs;
        
        Description* input = new Description(INPUT, DataType::IMAGE);
        input->setName("Input");
        inputs.push_back(input);
        
        return inputs;
    }
    
    const std::vector< stream::Description* > ConvertPixelType::setupOutputs()
    {
        std::vector<Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataType::IMAGE);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector< stream::Parameter* > ConvertPixelType::setupParameters()
    {
        std::vector<stream::Parameter*> parameters;
        
        EnumParameter* pixelType = new EnumParameter(PIXEL_TYPE);
        pixelType->setName("Pixel type");
        pixelType->setInactiveAccessMode(stream::Parameter::READ_WRITE);
        pixelType->setActiveAccessMode(stream::Parameter::READ_WRITE);
        pixelType->add(EnumDescription(Enum(stream::Image::MONO_8), "Mono image 8-bit"));
        pixelType->add(EnumDescription(Enum(stream::Image::RGB_24), "RGB image 24-bit"));
        pixelType->add(EnumDescription(Enum(stream::Image::BGR_24), "BGR image 24-bit"));
        pixelType->add(EnumDescription(Enum(stream::Image::BAYERBG_8), "Bayer BG pattern 8-bit"));
        pixelType->add(EnumDescription(Enum(stream::Image::BAYERGB_8), "Bayer GB pattern 8-bit"));
        parameters.push_back(pixelType);
                                    
        return parameters;
    }
    
    const int ConvertPixelType::getCvConversionCode(const stream::Image::PixelType inType, const stream::Image::PixelType outType)
    {
        switch(inType)
        {
        case stream::Image::MONO_8:
            switch(outType)
            {
            case stream::Image::RGB_24:
                return CV_GRAY2RGB;
            case stream::Image::BGR_24:
                return CV_GRAY2BGR;
            default:
                throw stream::ArgumentException("Unknown conversion.");   
            }
        case stream::Image::RGB_24:
            switch(outType)
            {
            case stream::Image::MONO_8:
                return CV_RGB2GRAY;
            default:
                throw stream::ArgumentException("Unknown conversion.");   
            }
        case stream::Image::BGR_24:
            switch(outType)
            {
            case stream::Image::MONO_8:
                return CV_BGR2GRAY;
            default:
                throw stream::ArgumentException("Unknown conversion.");   
            }
        case stream::Image::BAYERBG_8:
            switch(outType)
            {
            case stream::Image::RGB_24:
                return CV_BayerBG2RGB;
            case stream::Image::BGR_24:
                return CV_BayerBG2BGR;
            default:
                throw stream::ArgumentException("Unknown conversion.");   
            }
        case stream::Image::BAYERGB_8:
            switch(outType)
            {
            case stream::Image::RGB_24:
                return CV_BayerGB2RGB;
            case stream::Image::BGR_24:
                return CV_BayerGB2BGR;
            default:
                throw stream::ArgumentException("Unknown conversion.");   
            }
        default:
            throw stream::ArgumentException("Unknown conversion.");
        }         
    }
} 

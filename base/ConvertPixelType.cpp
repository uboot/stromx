#include "ConvertPixelType.h"

#include "Config.h"
#include "Image.h"
#include "Utilities.h"

#include <stream/DataContainer.h>
#include <stream/DataProvider.h>
#include <stream/Id2DataPair.h>
#include <stream/Id2DataComposite.h>
#include <stream/OperatorException.h>
#include <stream/EnumParameter.h>
#include <stream/ReadAccess.h>
#include <stream/WriteAccess.h>

using namespace stream;

namespace base
{
    const std::string ConvertPixelType::NAME("Convert pixel type");
    
    const std::string ConvertPixelType::PACKAGE(PACKAGE_NAME);
    const Version ConvertPixelType::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    ConvertPixelType::ConvertPixelType()
      : Operator(NAME, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
        m_pixelType(stream::Image::MONO_8)
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
        Id2DataPair srcMapper(SOURCE);
        Id2DataPair destMapper(DESTINATION);
        provider.receiveInputData(srcMapper && destMapper);
        
        ReadAccess src(srcMapper.data());
        WriteAccess dest(destMapper.data());
        
        const Image* srcImage = dynamic_cast<const Image*>(src());
        Image* destImage = dynamic_cast<Image*>(dest());
        
        stream::Image::PixelType pixelType = stream::Image::PixelType((unsigned int)(m_pixelType));
        
        unsigned int destImageSize = srcImage->width() * srcImage->height() * getDestPixelSize(pixelType);
        
        if(destImage->size() < destImageSize)
            throw InputException(DESTINATION, *this, "Destination image is too small");
        
        destImage->initialize(srcImage->width(), srcImage->height(), srcImage->width(), destImage->buffer(), pixelType);
        
        cv::Mat inCvImage = getOpenCvMat(*srcImage);
        cv::Mat outCvImage = getOpenCvMat(*destImage);
        
        if(pixelType == srcImage->pixelType())
        {
            inCvImage.copyTo(outCvImage);
        }
        else
        {
            int code = getCvConversionCode(srcImage->pixelType(), pixelType);
            cv::cvtColor(inCvImage, outCvImage, code);
        }
        
        Id2DataPair outputMapper(OUTPUT, destMapper.data());
        provider.sendOutputData( outputMapper);
    }
    
    const std::vector< Description* > ConvertPixelType::setupInputs()
    {
        std::vector<Description*> inputs;
        
        Description* source = new Description(SOURCE, DataType::IMAGE);
        source->setName("Source");
        inputs.push_back(source);
        
        Description* destination = new Description(DESTINATION, DataType::IMAGE);
        destination->setName("Destination");
        inputs.push_back(destination);
        
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
    
    const unsigned int ConvertPixelType::getDestPixelSize(const stream::Image::PixelType pixelType)
    {
        switch(pixelType)
        {
        case stream::Image::MONO_8:
        case stream::Image::MONO_16:
        case stream::Image::BAYERBG_8:
        case stream::Image::BAYERGB_8:
            return 1;
        case stream::Image::BAYERBG_16:
        case stream::Image::BAYERGB_16:
            return 2;
        case stream::Image::RGB_24:
        case stream::Image::BGR_24:
            return 3;
        case stream::Image::RGB_48:
        case stream::Image::BGR_48:
            return 6;
        default:
            throw stream::ArgumentException("Unknown pixel type.");    
        }  
    }
} 

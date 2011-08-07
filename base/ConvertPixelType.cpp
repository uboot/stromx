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
      : OperatorKernel(NAME, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
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
                throw WrongParameterId(id, *this);
            }
        }
        catch(std::bad_cast&)
        {
            throw WrongParameterType(parameter(id), *this);
        }
    }

    const Data& ConvertPixelType::getParameter(unsigned int id)
    {
        switch(id)
        {
        case PIXEL_TYPE:
            return m_pixelType;
        default:
            throw WrongParameterId(id, *this);
        }
    }  
    
    void ConvertPixelType::execute(DataProvider& provider)
    {
        Id2DataPair srcMapper(SOURCE);
        Id2DataPair destMapper(DESTINATION);
        provider.receiveInputData(srcMapper && destMapper);
        
        ReadAccess src(srcMapper.data());
        WriteAccess dest(destMapper.data());
        
        const Image& srcImage = dynamic_cast<const Image&>(src());
        Image& destImage = dynamic_cast<Image&>(dest());
        
        stream::Image::PixelType pixelType = stream::Image::PixelType((unsigned int)(m_pixelType));
        
        unsigned int destImageSize = srcImage.width() * srcImage.height() * getDestPixelSize(pixelType);
        unsigned int destImageStride = srcImage.width() * getDestPixelSize(pixelType);
        
        if(destImage.size() < destImageSize)
            throw InputError(DESTINATION, *this, "Destination image is too small");
        
        destImage.initialize(srcImage.width(), srcImage.height(), destImageStride, destImage.buffer(), pixelType);
        
        if((srcImage.pixelType() == stream::Image::RGB_24 || srcImage.pixelType() == stream::Image::BGR_24)
           && (pixelType == stream::Image::BAYERBG_8 || pixelType == stream::Image::BAYERGB_8))
        {
            // this case is not handled by OpenCV
            rgbToBayer(srcImage, destImage);
        }
        else
        {
            // use OpenCV for conversion
            openCvConversion(srcImage, destImage);
        }
        
        Id2DataPair outputMapper(OUTPUT, destMapper.data());
        provider.sendOutputData( outputMapper);
    }
    
    const std::vector<const stream::Description*> ConvertPixelType::setupInputs()
    {
        std::vector<const Description*> inputs;
        
        Description* source = new Description(SOURCE, DataType::IMAGE);
        source->setName("Source");
        inputs.push_back(source);
        
        Description* destination = new Description(DESTINATION, DataType::IMAGE);
        destination->setName("Destination");
        inputs.push_back(destination);
        
        return inputs;
    }
    
    const std::vector<const Description*> ConvertPixelType::setupOutputs()
    {
        std::vector<const Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataType::IMAGE);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector<const Parameter*> ConvertPixelType::setupParameters()
    {
        std::vector<const stream::Parameter*> parameters;
        
        EnumParameter* pixelType = new EnumParameter(PIXEL_TYPE);
        pixelType->setName("Pixel type");
        pixelType->setAccessMode(stream::Parameter::ACTIVATED_WRITE);
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
                throw stream::InvalidArgument("Unknown conversion.");   
            }
        case stream::Image::RGB_24:
            switch(outType)
            {
            case stream::Image::MONO_8:
                return CV_RGB2GRAY;
            case stream::Image::BGR_24:
                return CV_RGB2BGR;
            default:
                throw stream::InvalidArgument("Unknown conversion.");   
            }
        case stream::Image::BGR_24:
            switch(outType)
            {
            case stream::Image::MONO_8:
                return CV_BGR2GRAY;
            case stream::Image::RGB_24:
                return CV_BGR2RGB;
            default:
                throw stream::InvalidArgument("Unknown conversion.");   
            }
        case stream::Image::BAYERBG_8:
            switch(outType)
            {
            case stream::Image::RGB_24:
                return CV_BayerBG2RGB;
            case stream::Image::BGR_24:
                return CV_BayerBG2BGR;
            default:
                throw stream::InvalidArgument("Unknown conversion.");   
            }
        case stream::Image::BAYERGB_8:
            switch(outType)
            {
            case stream::Image::RGB_24:
                return CV_BayerGB2RGB;
            case stream::Image::BGR_24:
                return CV_BayerGB2BGR;
            default:
                throw stream::InvalidArgument("Unknown conversion.");   
            }
        default:
            throw stream::InvalidArgument("Unknown conversion.");
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
            throw stream::InvalidArgument("Unknown pixel type.");    
        }  
    }
    
    void ConvertPixelType::rgbToBayer(const stream::Image& inImage, stream::Image& outImage)
    {
        if(inImage.pixelSize() != stream::Image::RGB_24
           && outImage.pixelType() != stream::Image::BAYERBG_8)
        {
            throw stream::InvalidArgument("Unknown pixel type.");    
        }
        
        const uint8_t* inLine = inImage.data();
        uint8_t* outLine = outImage.buffer();
        
        for(unsigned int y = 0; y < inImage.height(); ++y)
        {
            const uint8_t* in = inLine;
            uint8_t* out = outLine;
            
            if(y % 2)
            {
                for(unsigned int x = 0; x < inImage.width(); ++x)
                {
                    if(x % 2)
                        out[0] = in[0];
                    else
                        out[0] = in[1];
                    
                    in += 3;
                    out += 1;
                }
            }
            else
            {
                for(unsigned int x = 0; x < inImage.width(); ++x)
                {
                    if(x % 2)
                        out[0] = in[1];
                    else
                        out[0] = in[2];
                    
                    in += 3;
                    out += 1;
                }
            }
            
            inLine += inImage.stride();
            outLine += outImage.stride();
        }
    }
    
    void ConvertPixelType::openCvConversion(const stream::Image& inImage, stream::Image& outImage)
    {
        cv::Mat inCvImage = getOpenCvMat(inImage);
        cv::Mat outCvImage = getOpenCvMat(outImage);
        
        if(inImage.pixelType() == outImage.pixelType())
        {
            inCvImage.copyTo(outCvImage);
        }
        else
        {
            int code = getCvConversionCode(inImage.pixelType(), outImage.pixelType());
            cv::cvtColor(inCvImage, outCvImage, code);
        }
    }
} 

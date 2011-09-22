#include "ConvertPixelType.h"

#include "Config.h"
#include "Image.h"
#include "Utilities.h"

#include <strom/DataContainer.h>
#include <strom/DataProvider.h>
#include <strom/Id2DataPair.h>
#include <strom/Id2DataComposite.h>
#include <strom/OperatorException.h>
#include <strom/EnumParameter.h>
#include <strom/ReadAccess.h>
#include <strom/WriteAccess.h>

using namespace strom;

namespace base
{
    const std::string ConvertPixelType::TYPE("ConvertPixelType");
    
    const std::string ConvertPixelType::PACKAGE(PACKAGE_NAME);
    const Version ConvertPixelType::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    ConvertPixelType::ConvertPixelType()
      : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
        m_pixelType(strom::Image::MONO_8)
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

    const Data& ConvertPixelType::getParameter(const unsigned int id) const
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
        
        ReadAccess<Image> src(srcMapper.data());
        WriteAccess<Image> dest(destMapper.data());
        
        const Image& srcImage = src();
        Image& destImage = dest();
        
        strom::Image::PixelType pixelType = strom::Image::PixelType((unsigned int)(m_pixelType));
        
        unsigned int destImageSize = srcImage.width() * srcImage.height() * getDestPixelSize(pixelType);
        unsigned int destImageStride = srcImage.width() * getDestPixelSize(pixelType);
        
        if(destImage.size() < destImageSize)
            throw InputError(DESTINATION, *this, "Destination image is too small");
        
        destImage.initialize(srcImage.width(), srcImage.height(), destImageStride, destImage.buffer(), pixelType);
        
        if((srcImage.pixelType() == strom::Image::RGB_24 || srcImage.pixelType() == strom::Image::BGR_24)
           && (pixelType == strom::Image::BAYERBG_8 || pixelType == strom::Image::BAYERGB_8))
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
    
    const std::vector<const strom::Description*> ConvertPixelType::setupInputs()
    {
        std::vector<const Description*> inputs;
        
        Description* source = new Description(SOURCE, DataVariant::IMAGE);
        source->setName("Source");
        inputs.push_back(source);
        
        Description* destination = new Description(DESTINATION, DataVariant::IMAGE);
        destination->setName("Destination");
        inputs.push_back(destination);
        
        return inputs;
    }
    
    const std::vector<const Description*> ConvertPixelType::setupOutputs()
    {
        std::vector<const Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataVariant::IMAGE);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector<const Parameter*> ConvertPixelType::setupParameters()
    {
        std::vector<const strom::Parameter*> parameters;
        
        EnumParameter* pixelType = new EnumParameter(PIXEL_TYPE);
        pixelType->setName("Pixel type");
        pixelType->setAccessMode(strom::Parameter::ACTIVATED_WRITE);
        pixelType->add(EnumDescription(Enum(strom::Image::MONO_8), "Mono image 8-bit"));
        pixelType->add(EnumDescription(Enum(strom::Image::RGB_24), "RGB image 24-bit"));
        pixelType->add(EnumDescription(Enum(strom::Image::BGR_24), "BGR image 24-bit"));
        pixelType->add(EnumDescription(Enum(strom::Image::BAYERBG_8), "Bayer BG pattern 8-bit"));
        pixelType->add(EnumDescription(Enum(strom::Image::BAYERGB_8), "Bayer GB pattern 8-bit"));
        parameters.push_back(pixelType);
                                    
        return parameters;
    }
    
    const int ConvertPixelType::getCvConversionCode(const strom::Image::PixelType inType, const strom::Image::PixelType outType)
    {
        switch(inType)
        {
        case strom::Image::MONO_8:
            switch(outType)
            {
            case strom::Image::RGB_24:
                return CV_GRAY2RGB;
            case strom::Image::BGR_24:
                return CV_GRAY2BGR;
            default:
                throw strom::WrongArgument("Unknown conversion.");   
            }
        case strom::Image::RGB_24:
            switch(outType)
            {
            case strom::Image::MONO_8:
                return CV_RGB2GRAY;
            case strom::Image::BGR_24:
                return CV_RGB2BGR;
            default:
                throw strom::WrongArgument("Unknown conversion.");   
            }
        case strom::Image::BGR_24:
            switch(outType)
            {
            case strom::Image::MONO_8:
                return CV_BGR2GRAY;
            case strom::Image::RGB_24:
                return CV_BGR2RGB;
            default:
                throw strom::WrongArgument("Unknown conversion.");   
            }
        case strom::Image::BAYERBG_8:
            switch(outType)
            {
            case strom::Image::RGB_24:
                return CV_BayerBG2RGB;
            case strom::Image::BGR_24:
                return CV_BayerBG2BGR;
            default:
                throw strom::WrongArgument("Unknown conversion.");   
            }
        case strom::Image::BAYERGB_8:
            switch(outType)
            {
            case strom::Image::RGB_24:
                return CV_BayerGB2RGB;
            case strom::Image::BGR_24:
                return CV_BayerGB2BGR;
            default:
                throw strom::WrongArgument("Unknown conversion.");   
            }
        default:
            throw strom::WrongArgument("Unknown conversion.");
        }         
    }
    
    const unsigned int ConvertPixelType::getDestPixelSize(const strom::Image::PixelType pixelType)
    {
        switch(pixelType)
        {
        case strom::Image::MONO_8:
        case strom::Image::MONO_16:
        case strom::Image::BAYERBG_8:
        case strom::Image::BAYERGB_8:
            return 1;
        case strom::Image::BAYERBG_16:
        case strom::Image::BAYERGB_16:
            return 2;
        case strom::Image::RGB_24:
        case strom::Image::BGR_24:
            return 3;
        case strom::Image::RGB_48:
        case strom::Image::BGR_48:
            return 6;
        default:
            throw strom::WrongArgument("Unknown pixel type.");    
        }  
    }
    
    void ConvertPixelType::rgbToBayer(const strom::Image& inImage, strom::Image& outImage)
    {
        if(inImage.pixelSize() != strom::Image::RGB_24
           && outImage.pixelType() != strom::Image::BAYERBG_8)
        {
            throw strom::WrongArgument("Unknown pixel type.");    
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
    
    void ConvertPixelType::openCvConversion(const strom::Image& inImage, strom::Image& outImage)
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

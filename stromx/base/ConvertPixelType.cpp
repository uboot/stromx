/* 
*  Copyright 2011 Matthias Fuchs
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#include "ConvertPixelType.h"
#include "Image.h"
#include "Utilities.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <stromx/core/DataContainer.h>
#include <stromx/core/DataProvider.h>
#include <stromx/core/EnumParameter.h>
#include <stromx/core/Id2DataComposite.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/ReadAccess.h>
#include <stromx/core/WriteAccess.h>

namespace stromx
{
    using namespace core;

    namespace base
    {
        const std::string ConvertPixelType::TYPE("ConvertPixelType");
        
        const std::string ConvertPixelType::PACKAGE(STROMX_BASE_PACKAGE_NAME);
        const Version ConvertPixelType::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR, BASE_VERSION_PATCH);
        
        ConvertPixelType::ConvertPixelType()
        : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
            m_pixelType(core::Image::MONO_8)
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
            
            if(srcMapper.data() == destMapper.data())
                throw InputError(DESTINATION, *this, "Destination image be the same image as the source image."); 
            
            ReadAccess<Image> src(srcMapper.data());
            WriteAccess<Image> dest(destMapper.data());
            
            const Image& srcImage = src();
            Image& destImage = dest();
            
            core::Image::PixelType pixelType = core::Image::PixelType((unsigned int)(m_pixelType));
            
            unsigned int destImageSize = srcImage.width() * srcImage.height() * getDestPixelSize(pixelType);
            unsigned int destImageStride = srcImage.width() * getDestPixelSize(pixelType);
            
            if(destImage.bufferSize() < destImageSize)
                throw InputError(DESTINATION, *this, "Destination image is too small.");
            
            destImage.initialize(srcImage.width(), srcImage.height(), destImageStride, destImage.buffer(), pixelType);
            
            if((srcImage.pixelType() == core::Image::RGB_24 || srcImage.pixelType() == core::Image::BGR_24)
            && (pixelType == core::Image::BAYERBG_8 || pixelType == core::Image::BAYERGB_8))
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
        
        const std::vector<const core::Description*> ConvertPixelType::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* source = new Description(SOURCE, DataVariant::IMAGE);
            source->setDoc("Source");
            inputs.push_back(source);
            
            Description* destination = new Description(DESTINATION, DataVariant::IMAGE);
            destination->setDoc("Destination");
            inputs.push_back(destination);
            
            return inputs;
        }
        
        const std::vector<const Description*> ConvertPixelType::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, DataVariant::IMAGE);
            output->setDoc("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> ConvertPixelType::setupParameters()
        {
            std::vector<const core::Parameter*> parameters;
            
            EnumParameter* pixelType = new EnumParameter(PIXEL_TYPE);
            pixelType->setDoc("Pixel type");
            pixelType->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            pixelType->add(EnumDescription(Enum(core::Image::MONO_8), "Mono image 8-bit"));
            pixelType->add(EnumDescription(Enum(core::Image::RGB_24), "RGB image 24-bit"));
            pixelType->add(EnumDescription(Enum(core::Image::BGR_24), "BGR image 24-bit"));
            pixelType->add(EnumDescription(Enum(core::Image::BAYERBG_8), "Bayer BG pattern 8-bit"));
            pixelType->add(EnumDescription(Enum(core::Image::BAYERGB_8), "Bayer GB pattern 8-bit"));
            parameters.push_back(pixelType);
                                        
            return parameters;
        }
        
        const int ConvertPixelType::getCvConversionCode(const core::Image::PixelType inType, const core::Image::PixelType outType)
        {
            switch(inType)
            {
            case core::Image::MONO_8:
                switch(outType)
                {
                case core::Image::RGB_24:
                    return CV_GRAY2RGB;
                case core::Image::BGR_24:
                    return CV_GRAY2BGR;
                default:
                    throw core::WrongArgument("Unknown conversion.");   
                }
            case core::Image::RGB_24:
                switch(outType)
                {
                case core::Image::MONO_8:
                    return CV_RGB2GRAY;
                case core::Image::BGR_24:
                    return CV_RGB2BGR;
                default:
                    throw core::WrongArgument("Unknown conversion.");   
                }
            case core::Image::BGR_24:
                switch(outType)
                {
                case core::Image::MONO_8:
                    return CV_BGR2GRAY;
                case core::Image::RGB_24:
                    return CV_BGR2RGB;
                default:
                    throw core::WrongArgument("Unknown conversion.");   
                }
            case core::Image::BAYERBG_8:
                switch(outType)
                {
                case core::Image::RGB_24:
                    return CV_BayerBG2RGB;
                case core::Image::BGR_24:
                    return CV_BayerBG2BGR;
                default:
                    throw core::WrongArgument("Unknown conversion.");   
                }
            case core::Image::BAYERGB_8:
                switch(outType)
                {
                case core::Image::RGB_24:
                    return CV_BayerGB2RGB;
                case core::Image::BGR_24:
                    return CV_BayerGB2BGR;
                default:
                    throw core::WrongArgument("Unknown conversion.");   
                }
            default:
                throw core::WrongArgument("Unknown conversion.");
            }         
        }
        
        const unsigned int ConvertPixelType::getDestPixelSize(const core::Image::PixelType pixelType)
        {
            switch(pixelType)
            {
            case core::Image::MONO_8:
            case core::Image::MONO_16:
            case core::Image::BAYERBG_8:
            case core::Image::BAYERGB_8:
                return 1;
            case core::Image::BAYERBG_16:
            case core::Image::BAYERGB_16:
                return 2;
            case core::Image::RGB_24:
            case core::Image::BGR_24:
                return 3;
            case core::Image::RGB_48:
            case core::Image::BGR_48:
                return 6;
            default:
                throw core::WrongArgument("Unknown pixel type.");    
            }  
        }
        
        void ConvertPixelType::rgbToBayer(const core::Image& inImage, core::Image& outImage)
        {
            if(inImage.pixelSize() != core::Image::RGB_24
            && outImage.pixelType() != core::Image::BAYERBG_8)
            {
                throw core::WrongArgument("Unknown pixel type.");    
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
        
        void ConvertPixelType::openCvConversion(const core::Image& inImage, core::Image& outImage)
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
}
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
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/Id2DataComposite.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/runtime/WriteAccess.h>

namespace stromx
{
    using namespace runtime;

    namespace example
    {
        const std::string ConvertPixelType::TYPE("ConvertPixelType");
        
        const std::string ConvertPixelType::PACKAGE(STROMX_EXAMPLE_PACKAGE_NAME);
        const Version ConvertPixelType::VERSION(STROMX_EXAMPLE_VERSION_MAJOR, STROMX_EXAMPLE_VERSION_MINOR, STROMX_EXAMPLE_VERSION_PATCH);
        
        ConvertPixelType::ConvertPixelType()
        : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
            m_pixelType(runtime::Image::MONO_8)
        {
        }

        void ConvertPixelType::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case PIXEL_TYPE:
                    m_pixelType = stromx::runtime::data_cast<Enum>(value);
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

        const DataRef ConvertPixelType::getParameter(const unsigned int id) const
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
                throw InputError(DESTINATION, *this, "Destination image must not be the same image as the source image."); 
            
            ReadAccess<Image> src(srcMapper.data());
            WriteAccess<Image> dest(destMapper.data());
            
            const Image& srcImage = src();
            Image& destImage = dest();
            
            runtime::Image::PixelType pixelType = runtime::Image::PixelType((unsigned int)(m_pixelType));
            
            unsigned int destImageSize = srcImage.width() * srcImage.height() * getDestPixelSize(pixelType);
            unsigned int destImageStride = srcImage.width() * getDestPixelSize(pixelType);
            
            if(destImage.bufferSize() < destImageSize)
                throw InputError(DESTINATION, *this, "Destination image is too small.");
            
            destImage.initializeImage(srcImage.width(), srcImage.height(), destImageStride, destImage.buffer(), pixelType);
            
            if((srcImage.pixelType() == runtime::Image::RGB_24 || srcImage.pixelType() == runtime::Image::BGR_24)
            && (pixelType == runtime::Image::BAYERBG_8 || pixelType == runtime::Image::BAYERGB_8))
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
        
        const std::vector<const runtime::Description*> ConvertPixelType::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* source = new Description(SOURCE, DataVariant::IMAGE);
            source->setTitle("Source");
            inputs.push_back(source);
            
            Description* destination = new Description(DESTINATION, DataVariant::IMAGE);
            destination->setTitle("Destination");
            inputs.push_back(destination);
            
            return inputs;
        }
        
        const std::vector<const Description*> ConvertPixelType::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, DataVariant::IMAGE);
            output->setTitle("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> ConvertPixelType::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            EnumParameter* pixelType = new EnumParameter(PIXEL_TYPE);
            pixelType->setTitle("Pixel type");
            pixelType->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            pixelType->add(EnumDescription(Enum(runtime::Image::MONO_8), "Mono image 8-bit"));
            pixelType->add(EnumDescription(Enum(runtime::Image::RGB_24), "RGB image 24-bit"));
            pixelType->add(EnumDescription(Enum(runtime::Image::BGR_24), "BGR image 24-bit"));
            pixelType->add(EnumDescription(Enum(runtime::Image::BAYERBG_8), "Bayer BG pattern 8-bit"));
            pixelType->add(EnumDescription(Enum(runtime::Image::BAYERGB_8), "Bayer GB pattern 8-bit"));
            parameters.push_back(pixelType);
                                        
            return parameters;
        }
        
        int ConvertPixelType::getCvConversionCode(const runtime::Image::PixelType inType, const runtime::Image::PixelType outType)
        {
            switch(inType)
            {
            case runtime::Image::MONO_8:
                switch(outType)
                {
                case runtime::Image::RGB_24:
                    return CV_GRAY2RGB;
                case runtime::Image::BGR_24:
                    return CV_GRAY2BGR;
                default:
                    throw runtime::WrongArgument("Unknown conversion.");   
                }
            case runtime::Image::RGB_24:
                switch(outType)
                {
                case runtime::Image::MONO_8:
                    return CV_RGB2GRAY;
                case runtime::Image::BGR_24:
                    return CV_RGB2BGR;
                default:
                    throw runtime::WrongArgument("Unknown conversion.");   
                }
            case runtime::Image::BGR_24:
                switch(outType)
                {
                case runtime::Image::MONO_8:
                    return CV_BGR2GRAY;
                case runtime::Image::RGB_24:
                    return CV_BGR2RGB;
                default:
                    throw runtime::WrongArgument("Unknown conversion.");   
                }
            case runtime::Image::BAYERBG_8:
                switch(outType)
                {
                case runtime::Image::RGB_24:
                    return CV_BayerBG2RGB;
                case runtime::Image::BGR_24:
                    return CV_BayerBG2BGR;
                default:
                    throw runtime::WrongArgument("Unknown conversion.");   
                }
            case runtime::Image::BAYERGB_8:
                switch(outType)
                {
                case runtime::Image::RGB_24:
                    return CV_BayerGB2RGB;
                case runtime::Image::BGR_24:
                    return CV_BayerGB2BGR;
                default:
                    throw runtime::WrongArgument("Unknown conversion.");   
                }
            default:
                throw runtime::WrongArgument("Unknown conversion.");
            }         
        }
        
        unsigned int ConvertPixelType::getDestPixelSize(const runtime::Image::PixelType pixelType)
        {
            switch(pixelType)
            {
            case runtime::Image::MONO_8:
            case runtime::Image::MONO_16:
            case runtime::Image::BAYERBG_8:
            case runtime::Image::BAYERGB_8:
                return 1;
            case runtime::Image::BAYERBG_16:
            case runtime::Image::BAYERGB_16:
                return 2;
            case runtime::Image::RGB_24:
            case runtime::Image::BGR_24:
                return 3;
            case runtime::Image::RGB_48:
            case runtime::Image::BGR_48:
                return 6;
            default:
                throw runtime::WrongArgument("Unknown pixel type.");    
            }  
        }
        
        void ConvertPixelType::rgbToBayer(const runtime::Image& inImage, runtime::Image& outImage)
        {
            if(inImage.pixelSize() != runtime::Image::RGB_24
            && outImage.pixelType() != runtime::Image::BAYERBG_8)
            {
                throw runtime::WrongArgument("Unknown pixel type.");    
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
        
        void ConvertPixelType::openCvConversion(const runtime::Image& inImage, runtime::Image& outImage)
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
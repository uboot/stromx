/* 
*  Copyright 2012 Matthias Fuchs
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

#include "Blur.h"
#include "Image.h"
#include "Utilities.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <stromx/core/DataContainer.h>
#include <stromx/core/DataProvider.h>
#include <stromx/core/EnumParameter.h>
#include <stromx/core/Id2DataComposite.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/NumericParameter.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/ReadAccess.h>
#include <stromx/core/WriteAccess.h>

namespace stromx
{
    using namespace core;

    namespace base
    {
        const std::string Blur::TYPE("Blur");
        
        const std::string Blur::PACKAGE(STROMX_BASE_PACKAGE_NAME);
        const Version Blur::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR, BASE_VERSION_PATCH);
        
        Blur::Blur()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupParameters()),
            m_inPlace(true),
            m_filterType(GAUSSIAN),
            m_kernelSize(3)
        {
        }

        void Blur::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case IN_PLACE:
                    m_inPlace = data_cast<const Bool &>(value);
                    break;
                case FILTER_TYPE:
                    m_filterType = data_cast<const Enum &>(value);
                    break;
                case KERNEL_SIZE:
                    if(data_cast<const UInt32 &>(value) % 2 == 0)
                    {
                        throw WrongParameterValue(parameter(KERNEL_SIZE), *this,
                                                  "Kernel size must be an odd value.");
                    }
                    
                    m_kernelSize = data_cast<const UInt32 &>(value);
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(BadCast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const Data& Blur::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case IN_PLACE:
                return m_inPlace;
            case FILTER_TYPE:
                return m_filterType;
            case KERNEL_SIZE:
                return m_kernelSize;
            default:
                throw WrongParameterId(id, *this);
            }
        }  
    
        void Blur::initialize()
        {
            stromx::core::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupInitParameters());
        }
        
        void Blur::execute(DataProvider& provider)
        {
            if(m_inPlace)
            {
                Id2DataPair srcMapper(SOURCE);
                provider.receiveInputData(srcMapper);
                
                WriteAccess<Image> src(srcMapper.data());
                Image& image = src();
                
                cv::Mat cvImage = getOpenCvMat(image);
                
                applyFilter(cvImage, cvImage);
                
                Id2DataPair outputMapper(OUTPUT, srcMapper.data());
                provider.sendOutputData(outputMapper);
            }
            else
            {
                Id2DataPair srcMapper(SOURCE);
                Id2DataPair destMapper(DESTINATION);
                provider.receiveInputData(srcMapper && destMapper);
                
                ReadAccess<Image> src(srcMapper.data());
                WriteAccess<Image> dest(destMapper.data());
                
                const Image& srcImage = src();
                Image& destImage = dest();
                
                unsigned int minimalDestinationSize = srcImage.width() * srcImage.pixelSize() * srcImage.height();
                        
                if(destImage.bufferSize() < minimalDestinationSize)
                    throw InputError(DESTINATION, *this, "Destination image is too small.");
                
                destImage.initialize(srcImage.width(), srcImage.height(), srcImage.width() * srcImage.pixelSize(), destImage.buffer(), srcImage.pixelType());
                
                cv::Mat inCvImage = getOpenCvMat(srcImage);
                cv::Mat outCvImage = getOpenCvMat(destImage);
                
                applyFilter(inCvImage, outCvImage);
                
                Id2DataPair outputMapper(OUTPUT, destMapper.data());
                provider.sendOutputData( outputMapper);
            }
        }
        
        const std::vector<const core::Description*> Blur::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* source = new Description(SOURCE, DataVariant::IMAGE);
            source->setDoc("Source");
            inputs.push_back(source);
            
            if(! m_inPlace)
            {
                Description* destination = new Description(DESTINATION, DataVariant::IMAGE);
                destination->setDoc("Destination");
                inputs.push_back(destination);
            }
            
            return inputs;
        }
        
        const std::vector<const Description*> Blur::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, DataVariant::IMAGE);
            output->setDoc("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Blur::setupParameters()
        {
            std::vector<const core::Parameter*> parameters;
            
            Parameter* inPlace = new Parameter(IN_PLACE, DataVariant::BOOL);
            inPlace->setDoc("Process in place");
            inPlace->setAccessMode(core::Parameter::NONE_WRITE);
            parameters.push_back(inPlace);
                                        
            return parameters;
        }
        
        const std::vector<const Parameter*> Blur::setupInitParameters()
        {
            std::vector<const core::Parameter*> parameters;
            
            EnumParameter* filterType = new EnumParameter(FILTER_TYPE);
            filterType->setDoc("Filter type");
            filterType->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            filterType->add(EnumDescription(Enum(MEDIAN), "Median"));
            filterType->add(EnumDescription(Enum(GAUSSIAN), "Gaussian"));
            parameters.push_back(filterType);
            
            Parameter* kernelSize = new NumericParameter<UInt32>(KERNEL_SIZE, DataVariant::UINT_32);
            kernelSize->setDoc("Kernel size");
            kernelSize->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(kernelSize);
                                        
            return parameters;
        }
        
        void Blur::applyFilter(const cv::Mat& in, cv::Mat& out)
        {
            switch(int(m_filterType))
            {
                case MEDIAN:
                    cv::medianBlur(in, out, int(m_kernelSize));
                    break;
                case GAUSSIAN:
                    cv::GaussianBlur(in, out, cv::Size(int(m_kernelSize), int(m_kernelSize)), 0);
                    break;
                default:
                    throw WrongArgument("Unsupported filter type.");
            }
        }
    } 
}
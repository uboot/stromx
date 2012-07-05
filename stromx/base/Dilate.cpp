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

#include "Dilate.h"
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
        const std::string Dilate::TYPE("Dilate");
        
        const std::string Dilate::PACKAGE(STROMX_BASE_PACKAGE_NAME);
        const Version Dilate::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR, BASE_VERSION_PATCH);
        
        Dilate::Dilate()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupParameters()),
            m_inPlace(true),
            m_kernelShape(RECTANGLE),
            m_kernelSizeX(3),
            m_kernelSizeY(3),
            m_iterations(1)
        {
        }

        void Dilate::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case IN_PLACE:
                    m_inPlace = data_cast<const Bool &>(value);
                    break;
                case KERNEL_SHAPE:
                    m_kernelShape = data_cast<const Enum &>(value);
                    break;
                case KERNEL_SIZE_X:
                    if(data_cast<const UInt32 &>(value) < 1)
                    {
                        throw WrongParameterValue(parameter(KERNEL_SIZE_X), *this,
                                                  "Kernel size must be positive.");
                    }
                    m_kernelSizeX = data_cast<const UInt32 &>(value);
                    break;
                case KERNEL_SIZE_Y:
                    if(data_cast<const UInt32 &>(value) < 1)
                    {
                        throw WrongParameterValue(parameter(KERNEL_SIZE_Y), *this,
                                                  "Kernel size must be positive.");
                    }
                    m_kernelSizeY = data_cast<const UInt32 &>(value);
                    break;
                case ITERATIONS:
                    m_iterations = data_cast<const UInt32 &>(value);
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(BadCast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const Data& Dilate::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case IN_PLACE:
                return m_inPlace;
            case KERNEL_SHAPE:
                return m_kernelShape;
            case KERNEL_SIZE_X:
                return m_kernelSizeX;
            case KERNEL_SIZE_Y:
                return m_kernelSizeY;
            case ITERATIONS:
                return m_iterations;
            default:
                throw WrongParameterId(id, *this);
            }
        }  
    
        void Dilate::initialize()
        {
            stromx::core::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupInitParameters());
        }
        
        void Dilate::execute(DataProvider& provider)
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
        
        const std::vector<const core::Description*> Dilate::setupInputs()
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
        
        const std::vector<const Description*> Dilate::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, DataVariant::IMAGE);
            output->setDoc("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Dilate::setupParameters()
        {
            std::vector<const core::Parameter*> parameters;
            
            Parameter* inPlace = new Parameter(IN_PLACE, DataVariant::BOOL);
            inPlace->setDoc("Process in place");
            inPlace->setAccessMode(core::Parameter::NONE_WRITE);
            parameters.push_back(inPlace);
                                        
            return parameters;
        }
        
        const std::vector<const Parameter*> Dilate::setupInitParameters()
        {
            std::vector<const core::Parameter*> parameters;
            
            EnumParameter* kernelShape = new EnumParameter(KERNEL_SHAPE);
            kernelShape->setDoc("Filter type");
            kernelShape->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            kernelShape->add(EnumDescription(Enum(RECTANGLE), "Rectangle"));
            kernelShape->add(EnumDescription(Enum(ELLIPSE), "Ellipse"));
            kernelShape->add(EnumDescription(Enum(CROSS), "Cross"));
            parameters.push_back(kernelShape);
            
            NumericParameter<UInt32>* kernelSizeX = new NumericParameter<UInt32>(KERNEL_SIZE_X, DataVariant::UINT_32);
            kernelSizeX->setDoc("Kernel size x");
            kernelSizeX->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            kernelSizeX->setMin(UInt32(1));
            parameters.push_back(kernelSizeX);
            
            NumericParameter<UInt32>* kernelSizeY = new NumericParameter<UInt32>(KERNEL_SIZE_Y, DataVariant::UINT_32);
            kernelSizeY->setDoc("Kernel size y");
            kernelSizeY->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            kernelSizeY->setMin(UInt32(1));
            parameters.push_back(kernelSizeY);
            
            NumericParameter<UInt32>* iterations = new NumericParameter<UInt32>(ITERATIONS, DataVariant::UINT_32);
            iterations->setDoc("Iterations");
            iterations->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            iterations->setMin(UInt32(1));
            parameters.push_back(iterations);
                                        
            return parameters;
        }
        
        void Dilate::applyFilter(const cv::Mat& in, cv::Mat& out)
        {
            cv::Mat kernel;
            switch(int(m_kernelShape))
            {
                case RECTANGLE:
                    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(m_kernelSizeX, m_kernelSizeY));
                    break;
                case ELLIPSE:
                    kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(m_kernelSizeX, m_kernelSizeY));
                    break;
                case CROSS:
                    kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(m_kernelSizeX, m_kernelSizeY));
                    break;
                default:
                    throw WrongArgument("Unsupported kernel shape.");
            }
            
            cv::dilate(in, out, kernel, cv::Point(-1, -1), m_iterations);
        }
    } 
}
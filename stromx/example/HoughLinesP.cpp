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

#include <boost/math/constants/constants.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "stromx/example/HoughLinesP.h"
#include "stromx/example/Image.h"
#include "stromx/example/Matrix.h"
#include "stromx/example/Utilities.h"
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/NumericParameter.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/runtime/WriteAccess.h>


namespace stromx
{
    using namespace runtime;

    namespace example
    {
        const std::string HoughLinesP::PACKAGE(STROMX_EXAMPLE_PACKAGE_NAME);
        const Version HoughLinesP::VERSION(STROMX_EXAMPLE_VERSION_MAJOR, STROMX_EXAMPLE_VERSION_MINOR, STROMX_EXAMPLE_VERSION_PATCH);
        const std::string HoughLinesP::TYPE("HoughLinesP");
        
        HoughLinesP::HoughLinesP()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
            m_rho(1.0),
            m_theta(boost::math::constants::pi<double>() / 180.0),
            m_threshold(100),
            m_minLineLength(50),
            m_maxLineGap(5)
        {
        }

        void HoughLinesP::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case RHO:
                    if(data_cast<Double>(value) <= 0.0)
                    {
                        throw WrongParameterValue(parameter(RHO), *this,
                                                  "Rho must be positive.");
                    }
                    m_rho = data_cast<Double>(value);
                    break;
                case THETA:
                    if(data_cast<Double>(value) <= 0.0)
                    {
                        throw WrongParameterValue(parameter(THETA), *this,
                                                  "Theta must be positive.");
                    }
                    m_theta = data_cast<Double>(value);
                    break;
                case THRESHOLD:
                    m_threshold = data_cast<UInt32>(value);
                    break;
                case MIN_LINE_LENGTH:
                    if(data_cast<Double>(value) < 0.0)
                    {
                        throw WrongParameterValue(parameter(MIN_LINE_LENGTH), *this,
                                                  "Minimal line length must not be negative.");
                    }
                    m_minLineLength = data_cast<Double>(value);
                    break;
                case MAX_LINE_GAP:
                    if(data_cast<Double>(value) < 0.0)
                    {
                        throw WrongParameterValue(parameter(MAX_LINE_GAP), *this,
                                                  "Maximal line gap must not be negative.");
                    }
                    m_maxLineGap = data_cast<Double>(value);
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

        const DataRef HoughLinesP::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case RHO:
                return m_rho;
            case THETA:
                return m_theta;
            case THRESHOLD:
                return m_threshold;
            case MIN_LINE_LENGTH:
                return m_minLineLength;
            case MAX_LINE_GAP:
                return m_maxLineGap;
            default:
                throw WrongParameterId(id, *this);
            }
        }    
                
        void HoughLinesP::execute(DataProvider& provider)
        {
            Id2DataPair srcMapper(IMAGE);
            provider.receiveInputData(srcMapper);
            
            WriteAccess<Image> src(srcMapper.data());
            Image& image = src();
            
            if(image.pixelType() != Image::MONO_8)
                throw InputError(HoughLinesP::IMAGE, *this, "Source image must be an 8-bit monochrome image.");
            
            // apply the transform
            cv::Mat cvImage = getOpenCvMat(image);
            cv::Mat cvLines;
            cv::HoughLinesP(cvImage, cvLines, m_rho, m_theta, m_threshold, m_minLineLength, m_maxLineGap);
            cvLines = cvLines.reshape(1, cvLines.cols);
            
            Matrix* lines = new Matrix(cvLines);
            Id2DataPair outputMapper(LINES, DataContainer(lines));
            provider.sendOutputData(outputMapper);
        }
        
        const std::vector<const runtime::Description*> HoughLinesP::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* image = new Description(IMAGE, DataVariant::MONO_IMAGE);
            image->setTitle("Image");
            inputs.push_back(image);
            
            return inputs;
        }
        
        const std::vector<const Description*> HoughLinesP::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* lines = new Description(LINES, DataVariant::DOUBLE_MATRIX);
            lines->setTitle("Lines");
            outputs.push_back(lines);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> HoughLinesP::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            NumericParameter<runtime::Double>* rho = new NumericParameter<runtime::Double>(RHO);
            rho->setMin(Double(0.0));
            rho->setTitle("Rho");
            rho->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(rho);
            
            NumericParameter<runtime::Double>* theta = new NumericParameter<runtime::Double>(THETA);
            theta->setMin(Double(0.0));
            theta->setStep(Double(0.1));
            theta->setTitle("Theta");
            theta->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(theta);
            
            NumericParameter<runtime::UInt32>* threshold = new NumericParameter<runtime::UInt32>(THRESHOLD);
            threshold->setTitle("Threshold");
            threshold->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(threshold);
            
            NumericParameter<runtime::Double>* minLineLength = new NumericParameter<runtime::Double>(MIN_LINE_LENGTH);
            minLineLength->setMin(Double(0.0));
            minLineLength->setTitle("Minimal line length");
            minLineLength->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(minLineLength);
            
            NumericParameter<runtime::Double>* maxLineGap = new NumericParameter<runtime::Double>(MAX_LINE_GAP);
            maxLineGap->setMin(Double(0.0));
            maxLineGap->setTitle("Maximal line gap");
            maxLineGap->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(maxLineGap);
                                        
            return parameters;
        }
    } 
}
/* 
*  Copyright 2012 Thomas Fidler
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

#include <boost/assert.hpp>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvsupport/WebCamera.h"
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/DataVariant.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/Image.h>
#include <stromx/runtime/OperatorException.h>

namespace stromx
{
    namespace cvsupport
    {
        bool WebCamera::m_AlreadyInitialized = false;
        const std::string WebCamera::TYPE("WebCamera");
        const std::string WebCamera::PACKAGE(STROMX_CVSUPPORT_PACKAGE_NAME);
        const runtime::Version WebCamera::VERSION(STROMX_CVSUPPORT_VERSION_MAJOR,STROMX_CVSUPPORT_VERSION_MINOR,STROMX_CVSUPPORT_VERSION_PATCH);
        
        const std::vector<const runtime::Description*> WebCamera::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            return inputs;
        }

        const std::vector<const runtime::Description*> WebCamera::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;

            runtime::Description* output = new runtime::Description(OUTPUT, runtime::DataVariant::RGB_IMAGE);
            output->setTitle("Output");
            outputs.push_back(output);

            return outputs;
        }

        const std::vector<const runtime::Parameter*> WebCamera::setupParameters(cv::VideoCapture* const webcam)
        {
            std::vector<const runtime::Parameter*> parameters;
            
            if(webcam->get(CV_CAP_PROP_FPS) >= 0.0)
            {
                runtime::Parameter* frameRate = new runtime::Parameter(FRAMERATE, runtime::DataVariant::FLOAT_64);
                frameRate->setTitle("Frame rate");
                frameRate->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
                parameters.push_back(frameRate);
            }
            
            if(webcam->get(CV_CAP_PROP_BRIGHTNESS) >= 0.0)
            {
                runtime::Parameter* brightness = new runtime::Parameter(BRIGHTNESS, runtime::DataVariant::FLOAT_64);
                brightness->setTitle("Brightness");
                brightness->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                parameters.push_back(brightness);
            }
            
            if(webcam->get(CV_CAP_PROP_CONTRAST) >= 0.0)
            {
                runtime::Parameter* contrast = new runtime::Parameter(CONTRAST, runtime::DataVariant::FLOAT_64);
                contrast->setTitle("Contrast");
                contrast->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                parameters.push_back(contrast);
            }
            
            if(webcam->get(CV_CAP_PROP_SATURATION) >= 0.0)
            {
                runtime::Parameter* saturation = new runtime::Parameter(SATURATION, runtime::DataVariant::FLOAT_64);
                saturation->setTitle("Saturation");
                saturation->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                parameters.push_back(saturation);
            }
            
            if(webcam->get(CV_CAP_PROP_HUE) >= 0.0)
            {
                runtime::Parameter* hue = new runtime::Parameter(HUE, runtime::DataVariant::FLOAT_64);
                hue->setTitle("Hue");
                hue->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                parameters.push_back(hue);
            }
            
            if(webcam->get(CV_CAP_PROP_GAIN) >= 0.0)
            {
                runtime::Parameter* gain = new runtime::Parameter(GAIN, runtime::DataVariant::FLOAT_64);
                gain->setTitle("Gain");
                gain->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                parameters.push_back(gain);
            }
            
            if(webcam->get(CV_CAP_PROP_EXPOSURE) >= 0.0)
            {
                runtime::Parameter* exposure = new runtime::Parameter(EXPOSURE, runtime::DataVariant::FLOAT_64);
                exposure->setTitle("Exposure");
                exposure->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                parameters.push_back(exposure);
            }

            return parameters;
        }

        WebCamera::WebCamera()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs())
        {
        }
        
        WebCamera::~WebCamera()
        {
        }

        void WebCamera::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                    case FRAMERATE:
                    {
                        runtime::Float64 frameRate = runtime::data_cast<runtime::Float64>(value);
                        m_webcam->set(CV_CAP_PROP_FPS, frameRate);
                        break;
                    }
                    case BRIGHTNESS:
                    {
                        runtime::Float64 brightness = runtime::data_cast<runtime::Float64>(value);
                        m_webcam->set(CV_CAP_PROP_BRIGHTNESS, brightness);
                        break;
                    }
                    case CONTRAST:
                    {
                        runtime::Float64 contrast = runtime::data_cast<runtime::Float64>(value);
                        m_webcam->set(CV_CAP_PROP_CONTRAST, contrast);
                        break;
                    }
                    case SATURATION:
                    {
                        runtime::Float64 saturation = runtime::data_cast<runtime::Float64>(value);
                        m_webcam->set(CV_CAP_PROP_SATURATION, saturation);
                        break;
                    }
                    case HUE:
                    {
                        runtime::Float64 hue = runtime::data_cast<runtime::Float64>(value);
                        m_webcam->set(CV_CAP_PROP_HUE, hue);
                        break;
                    }
                    case GAIN:
                    {
                        runtime::Float64 gain = runtime::data_cast<runtime::Float64>(value);
                        m_webcam->set(CV_CAP_PROP_GAIN, gain);
                        break;
                    }
                    case EXPOSURE:
                    {
                        runtime::Float64 exposure = runtime::data_cast<runtime::Float64>(value);
                        m_webcam->set(CV_CAP_PROP_EXPOSURE, exposure);
                        break;
                    }
                    default:
                        throw runtime::WrongParameterId(id,*this);
                }
            }
            catch(std::bad_cast&)
            {
                throw runtime::WrongParameterType(parameter(id),*this);
            }
        }

        const runtime::DataRef WebCamera::getParameter(const unsigned int id) const
        {
            switch(id)
            {
                case FRAMERATE:
                    return runtime::Float64(m_webcam->get(CV_CAP_PROP_FPS));
                case BRIGHTNESS:
                    return runtime::Float64(m_webcam->get(CV_CAP_PROP_BRIGHTNESS));
                case CONTRAST:
                    return runtime::Float64(m_webcam->get(CV_CAP_PROP_CONTRAST));
                case SATURATION:
                    return runtime::Float64(m_webcam->get(CV_CAP_PROP_SATURATION));
                case HUE:
                    return runtime::Float64(m_webcam->get(CV_CAP_PROP_HUE));
                case GAIN:
                    return runtime::Float64(m_webcam->get(CV_CAP_PROP_GAIN));
                case EXPOSURE:
                    return runtime::Float64(m_webcam->get(CV_CAP_PROP_EXPOSURE));
                default:
                    throw runtime::WrongParameterId(id,*this);
            }
        }

        void WebCamera::execute(runtime::DataProvider& provider)
        {            
            BOOST_ASSERT(m_webcam.get());
        
            // grab and retrieve a frame
            // apparently the returned frame is a pointer to a fixed frame buffer
            // (i.e. with a constant address)
            cv::Mat currentFrame;
            *m_webcam >> currentFrame;
            
            // to make sure the frame is not overwritten by a new one while being
            // processed a copy of the frame is created
            cv::Mat frameCopy;
            currentFrame.copyTo(frameCopy);
            cvsupport::Image* outImage = new cvsupport::Image(frameCopy);
            
            try
            {
                // So far, our experience showed that the webcam image is a BGR 24-bit image.
                // For this reason we try to initalize the image as such.
                // TODO: This could be different for other cameras.
                outImage->initializeImage(outImage->width(), outImage->height(), outImage->stride(),
                                          outImage->data(), Image::BGR_24);
            }
            catch(runtime::WrongArgument&)
            {
            }
            
            runtime::DataContainer outContainer = runtime::DataContainer(outImage);
            
            runtime::Id2DataPair outputDataMapper(OUTPUT, outContainer);
            provider.sendOutputData(outputDataMapper);
        }

        void WebCamera::initialize()
        {
            if(!m_AlreadyInitialized)
            {
                std::auto_ptr<cv::VideoCapture> webcam(new cv::VideoCapture(0));
                if(!webcam.get())
                    throw runtime::OperatorError(*this, "Failed to allocate WebCamera.");
                if(!webcam->isOpened())
                    throw runtime::OperatorError(*this, "Failed to open WebCamera.");
                
                m_webcam = webcam;
                
                //Construct empty inputs and outputs needed for function call OperatorKernel::initialize
                //Since the new input and output is added to existing one, you cannot call
                //setupInputs() and setupOutputs() again! Otherwise identical input-IDs and OutputIDs 
                //occur twice resulting in an exception caused by validateOutputs called 
                //in OperatorKernel::initialize. Note: setupInputs() could be called since for this operator
                //no input is generated, i.e. inputs is automatically an empty vector!
                std::vector<const runtime::Description*> inputs;
                std::vector<const runtime::Description*> outputs;
                
                OperatorKernel::initialize(inputs,outputs,setupParameters(m_webcam.get()));
                WebCamera::m_AlreadyInitialized = true;
            }
            else
                throw runtime::OperatorError(*this, "Instance of WebCamera operator already initialized.");
        }

        void WebCamera::deinitialize()
        {
            OperatorKernel::deinitialize();
            m_webcam.reset();
            WebCamera::m_AlreadyInitialized = false;
        }
    }
}

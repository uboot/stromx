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

#include "Config.h"
#include "Image.h"
#include "Utilities.h"
#include "WebCamera.h"
#include <boost/assert.hpp>
#include <stromx/runtime/Image.h>
#include <stromx/runtime/DataVariant.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataPair.h>

#include <stromx/runtime/OperatorException.h>

namespace stromx
{
    namespace base
    {
        const std::string WebCamera::TYPE("WebCamera");
        const std::string WebCamera::PACKAGE(STROMX_BASE_PACKAGE_NAME);
        const runtime::Version WebCamera::VERSION(BASE_VERSION_MAJOR,BASE_VERSION_MINOR,BASE_VERSION_PATCH);
        
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

        const std::vector<const runtime::Parameter*> WebCamera::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;

            runtime::Parameter* frameRate = new runtime::Parameter(FRAMERATE, runtime::DataVariant::DOUBLE);
            frameRate->setTitle("Frame rate");
            frameRate->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
            parameters.push_back(frameRate);
            
            runtime::Parameter* brightness = new runtime::Parameter(BRIGHTNESS, runtime::DataVariant::DOUBLE);
            brightness->setTitle("Brightness");
            brightness->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(brightness);

            return parameters;
        }

        WebCamera::WebCamera()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
            m_webcam(0)
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
                        runtime::Double frameRate = runtime::data_cast<const runtime::Double&>(value);
                        m_webcam->set(CV_CAP_PROP_FPS, frameRate);
                        break;
                    }
                    case BRIGHTNESS:
                    {
                        runtime::Double brightness = runtime::data_cast<const runtime::Double&>(value);
                        m_webcam->set(CV_CAP_PROP_BRIGHTNESS, brightness);
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
                    return runtime::Double(m_webcam->get(CV_CAP_PROP_FPS));
                case BRIGHTNESS:
                    return runtime::Double(m_webcam->get(CV_CAP_PROP_BRIGHTNESS));
                default:
                    throw runtime::WrongParameterId(id,*this);
            }
        }

        void WebCamera::execute(runtime::DataProvider& provider)
        {            
            BOOST_ASSERT(m_webcam);
        
            // grab and retrieve a frame
            // apparently the returned frame is a pointer to a fixed frame buffer
            // (i.e. with a constant address)
            cv::Mat currentFrame;
            *m_webcam >> currentFrame;
            
            // to make sure the frame is not overwritten by a new one while being
            // processed a copy of the frame is created
            cv::Mat frameCopy;
            currentFrame.copyTo(frameCopy);
            base::Image* outImage = new base::Image(frameCopy);
            
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
            std::auto_ptr<cv::VideoCapture> webcam(new cv::VideoCapture(0));
            if(!webcam.get())
                throw runtime::OperatorError(*this, "Failed to allocate WebCamera.");
            if(!webcam->isOpened())
                throw runtime::OperatorError(*this, "Failed to open WebCamera.");
            
            m_webcam = webcam.release();
        }

        void WebCamera::deinitialize()
        {
            delete m_webcam;
            m_webcam = 0;
        }
    }
}
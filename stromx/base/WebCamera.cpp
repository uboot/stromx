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
#include <stromx/core/Image.h>
#include <stromx/core/DataVariant.h>
#include <stromx/core/DataProvider.h>
#include <stromx/core/Id2DataPair.h>

#include <stromx/core/OperatorException.h>

namespace stromx
{
    namespace base
    {
        const std::string WebCamera::TYPE("WebCamera");
        const std::string WebCamera::PACKAGE(STROMX_BASE_PACKAGE_NAME);
        const core::Version WebCamera::VERSION(BASE_VERSION_MAJOR,BASE_VERSION_MINOR,BASE_VERSION_PATCH);
        
        const std::vector<const core::Description*> WebCamera::setupInputs()
        {
            std::vector<const core::Description*> inputs;
            return inputs;
        }

        const std::vector<const core::Description*> WebCamera::setupOutputs()
        {
            std::vector<const core::Description*> outputs;

            core::Description* output = new core::Description(OUTPUT, core::DataVariant::RGB_IMAGE);
            output->setTitle("Output");
            outputs.push_back(output);

            return outputs;
        }

        const std::vector<const core::Parameter*> WebCamera::setupParameters()
        {
            std::vector<const core::Parameter*> parameters;

            core::Parameter* frameRate = new core::Parameter(FRAMERATE, core::DataVariant::DOUBLE);
            frameRate->setTitle("Frame rate");
            frameRate->setAccessMode(core::Parameter::INITIALIZED_WRITE);
            parameters.push_back(frameRate);
            
            core::Parameter* brightness = new core::Parameter(BRIGHTNESS, core::DataVariant::DOUBLE);
            brightness->setTitle("Brightness");
            brightness->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(brightness);

            return parameters;
        }

        WebCamera::WebCamera() : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
                                m_webcam(0)
        {
        }

        void WebCamera::setParameter(unsigned int id, const core::Data& value)
        {
            try
            {
                switch(id)
                {
                    case FRAMERATE:
                    {
                        core::Double frameRate = core::data_cast<const core::Double&>(value);
                        m_webcam->set(CV_CAP_PROP_FPS, frameRate);
                        break;
                    }
                    case BRIGHTNESS:
                    {
                        core::Double brightness = core::data_cast<const core::Double&>(value);
                        m_webcam->set(CV_CAP_PROP_BRIGHTNESS, brightness);
                        break;
                    }
                    default:
                        throw core::WrongParameterId(id,*this);
                }
            }
            catch(std::bad_cast&)
            {
                throw core::WrongParameterType(parameter(id),*this);
            }
        }

        const core::DataRef WebCamera::getParameter(const unsigned int id) const
        {
            switch(id)
            {
                case FRAMERATE:
                    return core::Double(m_webcam->get(CV_CAP_PROP_FPS));
                case BRIGHTNESS:
                    return core::Double(m_webcam->get(CV_CAP_PROP_BRIGHTNESS));
                default:
                    throw core::WrongParameterId(id,*this);
            }
        }

        void WebCamera::execute(core::DataProvider& provider)
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
            
            core::DataContainer outContainer = core::DataContainer(outImage);
            
            core::Id2DataPair outputDataMapper(OUTPUT, outContainer);
            provider.sendOutputData(outputDataMapper);
        }

        void WebCamera::initialize()
        {
            std::auto_ptr<cv::VideoCapture> webcam(new cv::VideoCapture(0));
            if(!webcam.get())
                throw core::OperatorError(*this, "Failed to allocate WebCamera.");
            if(!webcam->isOpened())
                throw core::OperatorError(*this, "Failed to open WebCamera.");
            
            m_webcam = webcam.release();
        }

        void WebCamera::deinitialize()
        {
            delete m_webcam;
            m_webcam = 0;
        }
    }
}
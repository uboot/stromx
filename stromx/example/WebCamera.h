#ifndef STROMX_EXAMPLE_WEBCAMERA_H
#define STROMX_EXAMPLE_WEBCAMERA_H
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

#include <stromx/runtime/OperatorKernel.h>
#include <opencv2/highgui/highgui.hpp>

namespace stromx
{
    namespace example
    {
        class WebCamera : public runtime::OperatorKernel
        {
        public:  
            enum OutputId
            {
                OUTPUT
            };
            
            enum ParameterId
            {
                FRAMERATE,
                BRIGHTNESS,
                CONTRAST,
                SATURATION,
                HUE,
                GAIN,
                EXPOSURE
            };
                
            //constructor
            WebCamera();
            //destructor
            virtual ~WebCamera(){};
            virtual OperatorKernel* clone() const {return new WebCamera;}
            virtual void execute(runtime::DataProvider& provider);
            virtual void initialize();
            virtual void deinitialize();

            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const runtime::DataRef getParameter(const unsigned int id) const;

        private:
            static const std::vector<const runtime::Description*> setupInputs();
            static const std::vector<const runtime::Description*> setupOutputs();
            const std::vector<const runtime::Parameter*> setupParameters();

            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION;

            cv::VideoCapture* m_webcam;
            double m_configurableFrameRate;
            double m_configurableBrightness;
            double m_configurableContrast;
            double m_configurableSaturation;
            double m_configurableHue;
            double m_configurableGain;
            double m_configurableExposure;
        };
    }
}

#endif //STROMX_EXAMPLE_WEBCAMERA_H

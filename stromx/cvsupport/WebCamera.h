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

#ifndef STROMX_CVSUPPORT_WEBCAMERA_H
#define STROMX_CVSUPPORT_WEBCAMERA_H

#include <opencv2/highgui/highgui.hpp>
#include <stromx/runtime/OperatorKernel.h>

namespace stromx
{
    namespace cvsupport
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
            virtual ~WebCamera();
            virtual OperatorKernel* clone() const {return new WebCamera;}
            virtual void execute(runtime::DataProvider& provider);
            virtual void initialize();
            virtual void deinitialize();

            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const runtime::DataRef getParameter(const unsigned int id) const;

        private:
            static const std::vector<const runtime::Description*> setupInputs();
            static const std::vector<const runtime::Description*> setupOutputs();
            static const std::vector<const runtime::Parameter*> setupParameters(cv::VideoCapture* const webcam);

            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION;

            cv::VideoCapture* m_webcam;
            static bool m_AlreadyInitialized;
        };
    }
}

#endif //STROMX_CVSUPPORT_WEBCAMERA_H

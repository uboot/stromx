#ifndef BASE_WEBCAMERA_H
#define BASE_WEBCAMERA_H
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

#include <stromx/core/OperatorKernel.h>
#include <opencv2/highgui/highgui.hpp>

namespace stromx
{
    namespace base
    {
        class WebCamera : public core::OperatorKernel
        {
        public:  
            enum OutputId
            {
                OUTPUT
            };
            
            enum ParameterId
            {
                FRAMERATE,
                BRIGHTNESS
            };
                
            //constructor
            WebCamera();
            //destructor
            virtual ~WebCamera(){};
            virtual OperatorKernel* clone() const {return new WebCamera;}
            virtual void execute(core::DataProvider& provider);
            virtual void initialize();
            virtual void deinitialize();

            virtual void setParameter(const unsigned int id, const core::Data& value);
            virtual const core::DataRef getParameter(const unsigned int id) const;

        private:
            static const std::vector<const core::Description*> setupInputs();
            static const std::vector<const core::Description*> setupOutputs();
            static const std::vector<const core::Parameter*> setupParameters();

            static const std::string TYPE;
            static const std::string PACKAGE;
            static const core::Version VERSION;

            cv::VideoCapture* m_webcam;
        };
    }
}

#endif //BASE_WEBCAMERA_H
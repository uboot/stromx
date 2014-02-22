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

#ifndef STROMX_CVHIGHGUI_VIDEOCAPTURE_H
#define STROMX_CVHIGHGUI_VIDEOCAPTURE_H

#include <opencv2/highgui/highgui.hpp>
#include <stromx/runtime/Enum.h>
#include <stromx/runtime/OperatorKernel.h>

#include "stromx/cvhighgui/Config.h"

namespace stromx
{
    namespace cvhighgui
    {
        class STROMX_CVHIGHGUI_API VideoCapture : public runtime::OperatorKernel
        {
        public:  
            enum OutputId
            {
                OUTPUT
            };
            
//             enum CameraPortId
//             {
//                 PORT_0,
//                 PORT_1
//             };
            
            enum ParameterId
            {
                CAMERA_PORT,
                FRAMERATE,
                BRIGHTNESS,
                CONTRAST,
                SATURATION,
                HUE,
                GAIN,
                EXPOSURE
            };
                
            //constructor
            VideoCapture();
            //destructor
            virtual ~VideoCapture();
            virtual OperatorKernel* clone() const {return new VideoCapture;}
            virtual void execute(runtime::DataProvider& provider);
            virtual void initialize();
            virtual void deinitialize();

            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const runtime::DataRef getParameter(const unsigned int id) const;

        private:
            static const std::vector<const runtime::Description*> setupInputs();
            static const std::vector<const runtime::Description*> setupOutputs();
            static const std::vector<const runtime::Parameter*> setupInitParameters();
            static const std::vector<const runtime::Parameter*> setupParameters(cv::VideoCapture* const webcam);

            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION;

            std::auto_ptr<cv::VideoCapture> m_webcam;
            runtime::Enum m_portId;
            static std::vector<bool> m_alreadyInitializedCameraPorts;
            static std::vector<int> m_availableCameraPorts;
            static const int m_maxCameraPortScan;
        };
    }
}

#endif //STROMX_CVHIGHGUI_VIDEOCAPTURE_H

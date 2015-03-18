/* 
 *  Copyright 2014 Thomas Fidler
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

#ifndef STROMX_RASPI_RASPISTILLCAM_H
#define STROMX_RASPI_RASPISTILLCAM_H

#include <stromx/runtime/Enum.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/RecycleAccess.h>

class MMAL_COMPONENT_T;
class MMAL_PORT_T;
class MMAL_POOL_T;
class MMAL_BUFFER_HEADER_T;
class MMAL_QUEUE_T;

namespace stromx
{
    namespace raspi
    {
        struct PARAM_FLOAT_RECT_T;
        
        class RaspiStillCam : public runtime::OperatorKernel
        {
        public:
            enum OutputId
            {
                IMAGE
            };

            enum ParameterId
            {
                NUM_BUFFERS,
                SHUTTER_SPEED,
                RESOLUTION,
                ROI_GROUP,
                LEFT,
                TOP,
                WIDTH,
                HEIGHT
            };
            
            enum Resolution
            {
                RESOLUTION_2560_BY_1920,
                RESOLUTION_1280_BY_960,
                RESOLUTION_640_BY_480
            };

            RaspiStillCam();
            virtual OperatorKernel* clone() const {return new RaspiStillCam;}
            virtual void execute(runtime::DataProvider& provider);
            virtual void initialize();
            virtual void deinitialize();
            virtual void activate();
            virtual void deactivate();
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const runtime::DataRef getParameter(const unsigned int id) const;

        private:
            static const std::vector<const runtime::Description*> setupInputs();
            static const std::vector<const runtime::Description*> setupOutputs();
            const std::vector<const runtime::Parameter*> setupParameters();

            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION;

            static void callbackOutVideoPort(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer);
            static int width(const runtime::Enum & resolution);
            static int height(const runtime::Enum & resolution);
            
            bool getRoi(PARAM_FLOAT_RECT_T & rect) const;
            bool setRoi(PARAM_FLOAT_RECT_T & rect);

            MMAL_COMPONENT_T* m_raspicam;
            MMAL_POOL_T* m_outBufferPool;
            MMAL_PORT_T* m_port;
            MMAL_BUFFER_HEADER_T* m_buffer;
            
            runtime::UInt32 m_numBuffers;
            runtime::Enum m_resolution;
        };
    }
}

#endif //STROMX_RASPI_RASPISTILLCAM_H

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

#ifndef STROMX_RASPI_RASPICAM_H
#define STROMX_RASPI_RASPICAM_H

#include <stromx/runtime/OperatorKernel.h>

class MMAL_COMPONENT_T;
class MMAL_PORT_T;
class MMAL_POOL_T;
class MMAL_BUFFER_HEADER_T;
class MMAL_QUEUE_T;

namespace stromx
{
    namespace raspi
    {
        class RaspiCam : public runtime::OperatorKernel
        {
            enum OutputId
            {
                OUTPUT
            };
            
        public:
            RaspiCam();
            virtual ~RaspiCam();
            virtual OperatorKernel* clone() const {return new RaspiCam;}
            virtual void execute(runtime::DataProvider& provider);
	    virtual void initialize();
            
        private:
            static const std::vector<const runtime::Description*> setupInputs();
            static const std::vector<const runtime::Description*> setupOutputs();
            static const std::vector<const runtime::Parameter*> setupInitParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION;

	    static void callbackOutVideoPort(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer);

	    MMAL_COMPONENT_T* m_raspicam;
	    MMAL_POOL_T* m_outBufferPool;
	    MMAL_QUEUE_T* m_outQueue;
        };
    }
}

#endif //STROMX_RASPI_RASPICAM_H

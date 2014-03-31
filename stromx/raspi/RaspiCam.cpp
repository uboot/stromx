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

#include "stromx/raspi/Config.h"
#include "stromx/raspi/RaspiCam.h"

#include </opt/vc/include/bcm_host.h>

#include </opt/vc/include/interface/mmal/mmal_component.h>
#include <mmal_default_components.h>
#include </opt/vc/include/interface/mmal/mmal_port.h>
#include </opt/vc/include/interface/mmal/mmal_parameters_camera.h>

// #define MMAL_CAMERA_VIDEO_PORT 1
// #define MMAL_CAMERA_CAPTURE_PORT 2

namespace stromx
{
    namespace raspi
    {
        const std::string RaspiCam::TYPE("RaspiCam");
        const std::string RaspiCam::PACKAGE(STROMX_RASPI_PACKAGE_NAME);
        const runtime::Version RaspiCam::VERSION(STROMX_RASPI_VERSION_MAJOR,STROMX_RASPI_VERSION_MINOR,STROMX_RASPI_VERSION_PATCH);
        
        const std::vector< const runtime::Description* > RaspiCam::setupInputs()
        {                       
            return std::vector<const runtime::Description*>();
        }
        
        const std::vector< const runtime::Description* > RaspiCam::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;

            runtime::Description* output = new runtime::Description(OUTPUT, runtime::DataVariant::RGB_IMAGE);
            output->setTitle("Output");
            outputs.push_back(output);

            return outputs;
        }

        const std::vector< const runtime::Parameter* > RaspiCam::setupInitParameters()
        {
            return std::vector<const runtime::Parameter*>();
        }
     
        RaspiCam::RaspiCam()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupInitParameters())
        {
	  // try
	  //   {
	  // MMAL_COMPONENT_T *raspicam = 0;
	  // MMAL_STATUS_T status;
	  // MMAL_PORT_T *raspicamVideoPort = 0;
	  // MMAL_PORT_T *raspicamCapturePort = 0;

	  // bcm_host_init();
	  // status = mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA, &raspicam);
	  // if(status != MMAL_SUCCESS)
	  //   {
	  //     std::cout << "MMAL: Could not create default camera." << std::endl;
	  //     throw runtime::OperatorAllocationFailed("Raspi","RaspiCam");
	  //   }

	  // raspicamVideoPort = raspicam->output[MMAL_CAMERA_VIDEO_PORT];
	  // raspicamCapturePort = raspicam->output[MMAL_CAMERA_CAPTURE_PORT];

	  // MMAL_PARAMETER_CAMERA_CONFIG_T raspicamConfig;
	  // raspicamConfig.hdr.id = MMAL_PARAMETER_CAMERA_CONFIG;
	  // raspicamConfig.hdr.size = sizeof(raspicamConfig);
	  // raspicamConfig.max_stills_w = 1280;
	  // raspicamConfig.max_stills_h = 720;
	  // raspicamConfig.stills_yuv422 = 0;
	  // raspicamConfig.one_shot_stills = 0;
	  // raspicamConfig.max_preview_video_w = 1280;
	  // raspicamConfig.max_preview_video_h = 720;
	  // raspicamConfig.num_preview_video_frames = 0;
	  // raspicamConfig.stills_capture_circular_buffer_height = 0;
	  // raspicamConfig.fast_preview_resume = 0;
	  // raspicamConfig.use_stc_timestamp = MMAL_PARAM_TIMESTAMP_MODE_RESET_STC;

	  // mmal_port_parameter_set(raspicam->control, &raspicamConfig.hdr);

	  // MMAL_ES_FORMAT_T* raspicamFormat;
	  // raspicamFormat->encoding = MMAL_ENCODING_I420;
	  // raspicamFormat->encoding = MMAL_ENCODING_I420;

	  // raspicamFormat->es->video.width = 1280;
	  // raspicamFormat->es->video.height = 720;
	  // raspicamFormat->es->video.crop.x = 0;
	  // raspicamFormat->es->video.crop.y = 0;
	  // raspicamFormat->es->video.crop.width = 1280;
	  // raspicamFormat->es->video.crop.height = 720;
	  // raspicamFormat->es->video.frame_rate.num = 30;
	  // raspicamFormat->es->video.frame_rate.den = 1;

	  // raspicamVideoPort->format = raspicamFormat;

	  // raspicamVideoPort->buffer_size = 1280*720*12/8;
	  // raspicamVideoPort->buffer_num = 1;
	  // if(mmal_port_format_commit(raspicamVideoPort) != MMAL_SUCCESS)
	  //   {
	  //     std::cout << "MMAL: could not commit camera video port format." << std::endl;
	  //     throw runtime::OperatorAllocationFailed("Raspi","RaspiCam");
	  //   }
	  //   }
	  // catch(runtime::OperatorAllocationFailed &)
	  //   {}
        }
        

      RaspiCam::~RaspiCam()
        {
        }
        
        void RaspiCam::execute(runtime::DataProvider& provider)
        {

        }

    }
}

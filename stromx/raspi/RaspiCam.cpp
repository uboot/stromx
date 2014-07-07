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

#include <stromx/cvsupport/Image.h>
#include <stromx/runtime/Image.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/OperatorException.h>

#include <opencv2/core/core.hpp>

#include </opt/vc/include/bcm_host.h>

#include </opt/vc/include/interface/mmal/mmal_component.h>
#include <mmal_default_components.h>
#include </opt/vc/include/interface/mmal/mmal_pool.h>
#include </opt/vc/include/interface/mmal/mmal_port.h>
#include </opt/vc/include/interface/mmal/mmal_parameters_camera.h>
#include </opt/vc/include/interface/mmal/util/mmal_util.h>
#include </opt/vc/include/interface/mmal/util/mmal_util_params.h>

#define MMAL_CAMERA_VIDEO_PORT 1
#define MMAL_CAMERA_PREVIEW_PORT 0
#define MMAL_CAMERA_CAPTURE_PORT 2

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
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupInitParameters()),
	    m_raspicam(NULL)
        {
	}

        void RaspiCam::initialize()
        {
	  try
	    {
	  MMAL_STATUS_T status;

	  //bcm_host_init();
	  status = mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA, &m_raspicam);
	  if(status != MMAL_SUCCESS)
	    {
	      std::cout << "MMAL: Could not create default camera." << std::endl;
	      this->~RaspiCam();
	      throw runtime::OperatorAllocationFailed("Raspi","RaspiCam");
	    }
	  if(!m_raspicam->output_num)
	    {
	      std::cout << "MMAL: Default camera doesn't have output ports." << std::endl;
	      this->~RaspiCam();
	      throw runtime::OperatorAllocationFailed("Raspi","RaspiCam");
	    }

	  MMAL_PORT_T *raspicamVideoPort = m_raspicam->output[MMAL_CAMERA_VIDEO_PORT];
	  //MMAL_PORT_T *raspicamPreviewPort = m_raspicam->output[MMAL_CAMERA_PREVIEW_PORT];
	  //MMAL_PORT_T *raspicamCapturePort = m_raspicam->output[MMAL_CAMERA_CAPTURE_PORT];

	  MMAL_PARAMETER_CAMERA_CONFIG_T raspicamConfig;
	  raspicamConfig.hdr.id = MMAL_PARAMETER_CAMERA_CONFIG;
	  raspicamConfig.hdr.size = sizeof(raspicamConfig);
	  raspicamConfig.max_stills_w = 1280;
	  raspicamConfig.max_stills_h = 720;
	  raspicamConfig.stills_yuv422 = 0;
	  raspicamConfig.one_shot_stills = 0;
	  raspicamConfig.max_preview_video_w = 1280;
	  raspicamConfig.max_preview_video_h = 720;
	  raspicamConfig.num_preview_video_frames = 3;
	  raspicamConfig.stills_capture_circular_buffer_height = 0;
	  raspicamConfig.fast_preview_resume = 0;
	  raspicamConfig.use_stc_timestamp = MMAL_PARAM_TIMESTAMP_MODE_RESET_STC;

	  mmal_port_parameter_set(m_raspicam->control, &raspicamConfig.hdr);

	  //Get the pointer to each port format
	  MMAL_ES_FORMAT_T* raspicamVideoFormat = raspicamVideoPort->format;
	  //MMAL_ES_FORMAT_T* raspicamPreviewFormat = raspicamPreviewPort->format;

	  //Set up the formats on each port
	  raspicamVideoFormat->encoding_variant = MMAL_ENCODING_BGR24;//MMAL_ENCODING_I420;
	  raspicamVideoFormat->encoding = MMAL_ENCODING_BGR24;//MMAL_ENCODING_OPAQUE;
	  raspicamVideoFormat->es->video.width = 1280;
	  raspicamVideoFormat->es->video.height = 720;
	  raspicamVideoFormat->es->video.crop.x = 0;
	  raspicamVideoFormat->es->video.crop.y = 0;
	  raspicamVideoFormat->es->video.crop.width = 1280;
	  raspicamVideoFormat->es->video.crop.height = 720;
	  raspicamVideoFormat->es->video.frame_rate.num = 30;
	  raspicamVideoFormat->es->video.frame_rate.den = 1;
	  //raspicamVideoPort->buffer_size = 1280*720*12/8;
	  raspicamVideoPort->buffer_num = 10;


	  // raspicamPreviewFormat->encoding_variant = MMAL_ENCODING_I420;
	  // raspicamPreviewFormat->encoding = MMAL_ENCODING_OPAQUE;
	  // raspicamPreviewFormat->es->video.width = 1280;
	  // raspicamPreviewFormat->es->video.height = 720;
	  // raspicamPreviewFormat->es->video.crop.x = 0;
	  // raspicamPreviewFormat->es->video.crop.y = 0;
	  // raspicamPreviewFormat->es->video.crop.width = 1280;
	  // raspicamPreviewFormat->es->video.crop.height = 720;
	  // raspicamPreviewFormat->es->video.frame_rate.num = 30;
	  // raspicamPreviewFormat->es->video.frame_rate.den = 1;
	  // raspicamPreviewPort->buffer_size = 1280*720*12/8;
	  // raspicamPreviewPort->buffer_num = 3;

	  //Commit port formats
	  if(mmal_port_format_commit(raspicamVideoPort) != MMAL_SUCCESS)
	    {
	      std::cout << "MMAL: could not commit camera video port format." << std::endl;
	      this->~RaspiCam();
	      throw runtime::OperatorAllocationFailed("Raspi","RaspiCam");
	    }
	  // if(mmal_port_format_commit(raspicamPreviewPort) != MMAL_SUCCESS)
	  //   {
	  //     std::cout << "MMAL: could not commit camera preview port format." << std::endl;
	  //     this->~RaspiCam();
	  //     throw runtime::OperatorAllocationFailed("Raspi","RaspiCam");
	  //   }

	  m_outBufferPool = mmal_port_pool_create(raspicamVideoPort, raspicamVideoPort->buffer_num, raspicamVideoPort->buffer_size_recommended);
	  if(m_outBufferPool == NULL)
	    {
	      std::cout << "MMAL: could not create buffer pool for video port." << std::endl;
	      this->~RaspiCam();
	      throw runtime::OperatorAllocationFailed("Raspi","RaspiCam");
	    }

	  m_outQueue = mmal_queue_create();
	  if(m_outQueue == NULL)
	    {
	      std::cout << "MMAL: could not create queue for video port." << std::endl;
	      this->~RaspiCam();
	      throw runtime::OperatorAllocationFailed("Raspi","RaspiCam");
	    }
	  raspicamVideoPort->userdata = (MMAL_PORT_USERDATA_T*)m_outQueue;

	  status = mmal_port_enable(raspicamVideoPort, callbackOutVideoPort);
	  if(status != MMAL_SUCCESS)
	    {
	      std::cout << "MMAL: could not enable video port." << std::endl;
	      throw runtime::OperatorAllocationFailed("Raspi","RaspiCam");
	    }
	  
	  status = mmal_component_enable(m_raspicam);
	  if(status != MMAL_SUCCESS)
	    {
	      std::cout << "MMAL: could not enable camera." << std::endl;
	      throw runtime::OperatorAllocationFailed("Raspi","RaspiCam");
	    }


	    }
	  catch(runtime::OperatorAllocationFailed &)
	    {}
        }
        

      RaspiCam::~RaspiCam()
        {
	  if(m_raspicam)
	    mmal_component_destroy(m_raspicam);
	  if(m_outBufferPool)
	    mmal_pool_destroy(m_outBufferPool);
	  if(m_outQueue)
	    mmal_queue_destroy(m_outQueue);
        }
        
        void RaspiCam::execute(runtime::DataProvider& provider)
        {
	  MMAL_BUFFER_HEADER_T*  buffer;
	  MMAL_BUFFER_HEADER_T*  bufferNew;

	  if(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT]->is_enabled)
	  {
	    unsigned int bufferCounter = 0;
	    while((bufferNew = mmal_queue_get(m_outBufferPool->queue)) != NULL)
	     {
		mmal_port_send_buffer(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT], bufferNew);
		bufferCounter++;
		std::cout << bufferCounter << std::endl;
	      }
	    if(bufferCounter == 0)
	      {
		std::cout << "MMAL: Cannot return a new buffer to video port." << std::endl;
	      }
	  
	    mmal_port_parameter_set_boolean(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT],MMAL_PARAMETER_CAPTURE, 1);

	  if((buffer = mmal_queue_get(m_outQueue)) != NULL)
	    {
	      std::cout << "DEBUG: Entering buffer processing" << std::endl;
	      if(buffer->cmd)
		{
		  mmal_buffer_header_release(buffer);
		}
	      else
		{
	      cv::Mat bufferCopy(1280,720,CV_32FC3);
	      mmal_buffer_header_mem_lock(buffer);
std::cout << "Debug: before memcpy" << std::endl;
	      memcpy(&bufferCopy, buffer->data, 1280*720*3);
std::cout << "Debug: after memcpy" << std::endl;
	      mmal_buffer_header_mem_unlock(buffer);
std::cout << "Debug: buffer unlocked" << std::endl;
//mmal_buffer_header_release(buffer);
std::cout << "Debug: try to create outImage" << std::endl;
	      cvsupport::Image* outImage = new cvsupport::Image(bufferCopy);
 std::cout << "Debug: outImage created" << std::endl;
	      try
		{
 std::cout << "Debug: before outImage initialization" << std::endl;
		  outImage->initializeImage(1280, 720, outImage->stride(), outImage->data(), cvsupport::Image::BGR_24);
 std::cout << "Debug: after outImage initialization" << std::endl;
		}
	      catch(runtime::WrongArgument&)
		{
		}
	      runtime::DataContainer outContainer = runtime::DataContainer(outImage);
	      runtime::Id2DataPair outputDataMapper(OUTPUT, outContainer);
	      provider.sendOutputData(outputDataMapper);
		}
	    }
	  }
        }

      void RaspiCam::callbackOutVideoPort(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
      {
	std::cout << "DEBUG: Entering callbackOutVideoPort" << std::endl;
	MMAL_QUEUE_T* queue = (MMAL_QUEUE_T*)port->userdata;
	mmal_queue_put(queue,buffer);
      }

    }
}

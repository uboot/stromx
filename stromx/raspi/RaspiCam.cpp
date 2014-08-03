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

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <stromx/cvsupport/Image.h>
#include <stromx/runtime/Data.h>
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/OperatorException.h>

#include <bcm_host.h>

#include <interface/mmal/mmal_component.h>
#include <interface/mmal/util/mmal_default_components.h>
#include <interface/mmal/mmal_pool.h>
#include <interface/mmal/mmal_port.h>
#include <interface/mmal/mmal_parameters_camera.h>
#include <interface/mmal/util/mmal_util.h>
#include <interface/mmal/util/mmal_util_params.h>

#define MMAL_CAMERA_VIDEO_PORT 1
#define MMAL_CAMERA_PREVIEW_PORT 0
#define MMAL_CAMERA_CAPTURE_PORT 2

namespace
{
  static boost::mutex mutex;
  static boost::condition_variable_any cond;
}

namespace stromx
{
    namespace raspi
    {
        typedef boost::lock_guard<boost::mutex> lock_t;
        typedef boost::unique_lock<boost::mutex> unique_lock_t;

        // /** \cond */
        // namespace impl
	// {
	//   struct BoostThreadHandle
	//   {
	//     boost::condition_variable_any m_cond;
	//     boost::mutex m_mutex;
	//   };
	// }
        // /** \endcond */

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
	  std::vector<const runtime::Parameter*> parameters;
	  m_cameraModeParameter = new runtime::EnumParameter(CAMERA_MODE);
	  m_cameraModeParameter->setAccessMode(runtime::Parameter::NONE_WRITE);
	  m_cameraModeParameter->setTitle(("Camera mode"));
	  m_cameraModeParameter->add(runtime::EnumDescription(runtime::Enum(STILL), ("Still")));
	  m_cameraModeParameter->add(runtime::EnumDescription(runtime::Enum(VIDEO), ("Video")));
	  parameters.push_back(m_cameraModeParameter);

	  return parameters;
        }

      void RaspiCam::setParameter(unsigned int id, const runtime::Data& value)
      {
	MMAL_STATUS_T status;

	try
	  {
	    switch(id)
	      {
	      case CAMERA_MODE:
		{
		  const runtime::Enum& castedValue = runtime::data_cast<runtime::Enum>(value);
		  m_cameraMode = castedValue;
		}
		break;
	      case FRAME_RATE:
		{
		  runtime::Float32 castedValue = runtime::data_cast<runtime::Float32>(value);
		  MMAL_RATIONAL_T frameRateRational = {int(castedValue),1};
		  status = mmal_port_parameter_set_rational(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT],MMAL_PARAMETER_FRAME_RATE, frameRateRational);
		  if(status != MMAL_SUCCESS)
		    {
		      throw runtime::ParameterError(parameter(id), *this);
		    }
		}
		break;
	      default:
		throw runtime::WrongParameterId(id,*this);
	      }
	  }
	catch(runtime::BadCast&)
	  {
	    throw runtime::WrongParameterType(parameter(id), *this);
	  }
      }

      const runtime::DataRef RaspiCam::getParameter(const unsigned int id) const
      {
	MMAL_STATUS_T status;

	switch(id)
	  {
	  case CAMERA_MODE:
	    return m_cameraMode;
	  case FRAME_RATE:
	    MMAL_RATIONAL_T frameRateValue;
	    status = mmal_port_parameter_get_rational(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT],MMAL_PARAMETER_FRAME_RATE, &frameRateValue);
	    if(status == MMAL_SUCCESS)
	      {
		return runtime::Float32(frameRateValue.num/frameRateValue.den);
	      }
	    else
	      {
		throw runtime::ParameterError(parameter(id), *this);
	      }
	  default:
	    throw runtime::WrongParameterId(id,*this);
	  }
      }
     

        RaspiCam::RaspiCam()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupInitParameters()),
	    m_raspicam(NULL),
	    m_outBufferPool(NULL),
	    m_outQueue(NULL),
	    m_cameraModeParameter(NULL),
	    m_cameraMode(RaspiCam::VIDEO),
	    m_frameIndex(0)
        {
	}

        void RaspiCam::initialize()
        {
	  //Basic setup/initialization of mmal component camera
	  	  try
	    {
	  MMAL_STATUS_T status;

	  bcm_host_init();
	  status = mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA, &m_raspicam);
	  if(status != MMAL_SUCCESS)
	    {
	      std::cout << "MMAL: Could not create default camera." << std::endl;
	      cleanUp();
	      throw runtime::OperatorAllocationFailed("Raspi","RaspiCam");
	    }
	  if(!m_raspicam->output_num)
	    {
	      std::cout << "MMAL: Default camera doesn't have output ports." << std::endl;
	      cleanUp();
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
	  raspicamVideoFormat->es->video.frame_rate.num = 1;
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
	      cleanUp();
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
	      cleanUp();
	      throw runtime::OperatorAllocationFailed("Raspi","RaspiCam");
	    }

	  //Create buffer pool in stromx (recycling)
	  runtime::Data* recycleBuffer = NULL;

	  // delete all remaining buffers in the recycling access
	  while((recycleBuffer = m_recycleBuffers()))
	    delete recycleBuffer;

	  // allocate one buffers and add them to the recycler
	  m_recycleBuffers.add(runtime::DataContainer(new cvsupport::Image(0)));

	  m_outQueue = mmal_queue_create();
	  if(m_outQueue == NULL)
	    {
	      std::cout << "MMAL: could not create queue for video port." << std::endl;
	      cleanUp();
	      throw runtime::OperatorAllocationFailed("Raspi","RaspiCam");
	    }
	  raspicamVideoPort->userdata = (MMAL_PORT_USERDATA_T*)m_outQueue;

	  status = mmal_port_enable(raspicamVideoPort, callbackOutVideoPort);
	  if(status != MMAL_SUCCESS)
	    {
	      std::cout << "MMAL: could not enable video port." << std::endl;
	      cleanUp();
	      throw runtime::OperatorAllocationFailed("Raspi","RaspiCam");
	    }
	  
	  status = mmal_component_enable(m_raspicam);
	  if(status != MMAL_SUCCESS)
	    {
	      std::cout << "MMAL: could not enable camera." << std::endl;
	      cleanUp();
	      throw runtime::OperatorAllocationFailed("Raspi","RaspiCam");
	    }
	  if(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT]->is_enabled)
	  {
	    unsigned int bufferCounter = 0;
	    MMAL_BUFFER_HEADER_T*  bufferNew;
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
	  }
	    }
	  catch(runtime::OperatorAllocationFailed &)
	    {}
	  std::vector<const runtime::Description*> inputs;
	  std::vector<const runtime::Description*> outputs;
	  OperatorKernel::initialize(inputs,outputs,setupParameters());
        }

      const std::vector<const runtime::Parameter*> RaspiCam::setupParameters()
      {
	std::vector<const runtime::Parameter*> parameters;

	if(m_cameraMode == RaspiCam::VIDEO)
	  {
	    runtime::Parameter* frameRate = new runtime::Parameter(FRAME_RATE,runtime::DataVariant::FLOAT_32);
	    frameRate->setTitle("Frame rate");
	    frameRate->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
	    parameters.push_back(frameRate);
	  }

	return parameters;
      }
        

      RaspiCam::~RaspiCam()
        {
        }

      void RaspiCam::cleanUp()
      {
	if(m_raspicam)
	    mmal_component_destroy(m_raspicam);
	if(m_outBufferPool)
	    mmal_pool_destroy(m_outBufferPool);
	if(m_outQueue)
	    mmal_queue_destroy(m_outQueue);
      }

      void RaspiCam::activate()
      {		
	// Activate capture function of port
	  switch (m_cameraMode)
	    {
		    case VIDEO:
		      mmal_port_parameter_set_boolean(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT],MMAL_PARAMETER_CAPTURE, 1);
		      break;
		    case STILL:
		      // ToDo
		      break;
		    default:
		      throw runtime::WrongParameterValue(parameter(CAMERA_MODE), *this);
		     }
      }

      void RaspiCam::deactivate()
      {
	mmal_port_parameter_set_boolean(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT],MMAL_PARAMETER_CAPTURE, 0);
      }

        
        void RaspiCam::execute(runtime::DataProvider& provider)
        {
	  MMAL_STATUS_T status;
	  MMAL_BUFFER_HEADER_T*  buffer;
	  MMAL_BUFFER_HEADER_T*  bufferNew;

	  ++m_frameIndex;

	  try
	    {
	      provider.unlockParameters();
	      unique_lock_t lock(mutex);
	      
	      while((buffer = mmal_queue_get(m_outQueue)) == NULL)
		cond.wait(lock);
	      provider.lockParameters();
	    }
	  catch(boost::thread_interrupted&)
	    {
	      provider.lockParameters();
	      throw runtime::Interrupt();
	    }

	      std::cout << "DEBUG: Entering buffer processing" << std::endl;
	      if(buffer->cmd)
		{
		  mmal_buffer_header_release(buffer);
		}
	      else
		{
		  //Wait to get a free buffer from recycle access
		  cvsupport::Image* recycleBuffer = dynamic_cast<cvsupport::Image*>(m_recycleBuffers());
		  if(recycleBuffer)
		    {
		      // Resize the recycling buffer to fit current image size constraints
		      // NOTE: recycleBuffer owns a cvsupport::Image
		      recycleBuffer->resize(1280,720,runtime::Image::BGR_24);
		      //cv::Mat bufferCopy(720,1280,CV_8UC3);
		      mmal_buffer_header_mem_lock(buffer);
		      std::cout << "Debug: before memcpy" << std::endl;
		      //memcpy(bufferCopy.data, buffer->data, 1280*720*3);
		      memcpy(recycleBuffer->data(), buffer->data, 1280*720*3);
		      std::cout << "Debug: after memcpy" << std::endl;
		      mmal_buffer_header_mem_unlock(buffer);
		      std::cout << "Debug: buffer unlocked" << std::endl;
		      mmal_buffer_header_release(buffer);
		      std::cout << "Debug: try to create outImage" << std::endl;
		      //cvsupport::Image* outImage = new cvsupport::Image(bufferCopy);
		      // cvsupport::Image* outImage = new cvsupport::Image(recycleBuffer);
		      // std::cout << "Debug: outImage created" << std::endl;
		      // try
		      // 	{
		      // 	  std::cout << "Debug: before outImage initialization" << std::endl;
		      // 	  outImage->initializeImage(outImage->width(),outImage->height(), outImage->stride(), outImage->data(), cvsupport::Image::BGR_24);
		      // 	  std::cout << "Debug: after outImage initialization" << std::endl;
		      // 	}
		      // catch(runtime::WrongArgument&)
		      // 	{
		      // 	}
		      //runtime::DataContainer outContainer = runtime::DataContainer(outImage);
		      runtime::DataContainer outContainerIndex = runtime::DataContainer(new runtime::UInt32(m_frameIndex));
		      runtime::DataContainer outContainer = runtime::DataContainer(recycleBuffer);
		      
		      //remember it in the recycling access
		      m_recycleBuffers.add(outContainer);
		      runtime::Id2DataPair outputDataMapper(OUTPUT, outContainerIndex);
		      provider.sendOutputData(outputDataMapper);
		    }
		}
	      if(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT]->is_enabled)
		{
		  bufferNew = mmal_queue_get(m_outBufferPool->queue);
		  if(bufferNew)
		    {
		      status = mmal_port_send_buffer(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT], bufferNew);
		    }

		  if(!bufferNew || status != MMAL_SUCCESS)
		    {
		      std::cout << "Unable to send buffer to video port." << std::endl;
		    }
		}
	}
        

      void RaspiCam::callbackOutVideoPort(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
      {
	std::cout << "DEBUG: Entering callbackOutVideoPort" << std::endl;
	MMAL_QUEUE_T* queue = (MMAL_QUEUE_T*)port->userdata;
	lock_t l(mutex);
	mmal_queue_put(queue,buffer);
	cond.notify_all();
      }

    }
}

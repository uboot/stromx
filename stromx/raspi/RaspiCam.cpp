
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
//#include "RaspiCam.h"

#include <bcm_host.h>

//#include <interface/mmal/mmal_component.h>
//#include <interface/mmal/mmal_pool.h>
//#include <interface/mmal/mmal_port.h>
//#include <interface/mmal/mmal_parameters_camera.h>
#include <interface/mmal/util/mmal_default_components.h>
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

            runtime::Description* outputFrames = new runtime::Description(OUTPUT_FRAMES, runtime::DataVariant::RGB_IMAGE);
            outputFrames->setTitle("Output frames");
            outputs.push_back(outputFrames);

            runtime::Description* outputFrameIndex = new runtime::Description(OUTPUT_FRAME_INDEX, runtime::DataVariant::UINT_32);
            outputFrameIndex->setTitle("Output image counter");
            outputs.push_back(outputFrameIndex);

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
                    case AWB_MODE:
                    {
                        runtime::Enum castedValue = runtime::data_cast<runtime::Enum>(value);
                        MMAL_PARAMETER_AWBMODE_T awbMode ={{MMAL_PARAMETER_AWB_MODE, sizeof(awbMode)}, MMAL_PARAM_AWBMODE_T(int(castedValue))};
                        status = mmal_port_parameter_set(m_raspicam->control, &awbMode.hdr);
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
                    {
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
                    }
                    case AWB_MODE:
                    {
                        MMAL_PARAMETER_AWBMODE_T awbMode = {{MMAL_PARAMETER_AWB_MODE, sizeof(awbMode)},MMAL_PARAM_AWBMODE_AUTO};
                        status = mmal_port_parameter_get(m_raspicam->control,&awbMode.hdr);
                        if(status == MMAL_SUCCESS)
                        {
                            return runtime::Enum(awbMode.value);
                        }
                        else
                        {
                            throw runtime::ParameterError(parameter(id), *this);
                        }
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
            MMAL_STATUS_T status;

            bcm_host_init();
            
            status = mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA, &m_raspicam);
            if(status != MMAL_SUCCESS)
            {
                cleanUp();
                throw runtime::OperatorAllocationFailed("Raspi","Could not create mmal default camera.");
            }
            if(!m_raspicam->output_num)
            {
                cleanUp();
                throw runtime::OperatorAllocationFailed("Raspi","Default camera has no outputs.");
            }

            m_raspicamVideoPort = m_raspicam->output[MMAL_CAMERA_VIDEO_PORT];
            //MMAL_PORT_T *raspicamPreviewPort = m_raspicam->output[MMAL_CAMERA_PREVIEW_PORT];
            m_raspicamCapturePort = m_raspicam->output[MMAL_CAMERA_CAPTURE_PORT];

            // Default settings at initialization
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

            // Set basic parameters
            mmal_port_parameter_set(m_raspicam->control, &raspicamConfig.hdr);
            
            switch(m_cameraMode)
            {
                case VIDEO:
                {
                    //Get the pointer
                    MMAL_ES_FORMAT_T* raspicamVideoFormat = m_raspicamVideoPort->format;
                    
                    //Set up the format
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
                    //m_raspicamVideoPort->buffer_size = 1280*720*12/8;
                    m_raspicamVideoPort->buffer_num = 10;
                    
                    //Commit port format
                    if(mmal_port_format_commit(m_raspicamVideoPort) != MMAL_SUCCESS)
                    {
                        cleanUp();
                        throw runtime::OperatorAllocationFailed("Raspi","Could not commit video port format.");
                    }
                }
                break;
                case STILL:
                {
                    //Get the pointer
                    MMAL_ES_FORMAT_T* raspicamCaptureFormat = m_raspicamCapturePort->format;
                    
                    //Set up the format
                    // raspicamCaptureFormat->encoding_variant = MMAL_ENCODING_I420;
                    // raspicamCaptureFormat->encoding = MMAL_ENCODING_OPAQUE;
                    // raspicamCaptureFormat->es->video.width = 1280;
                    // raspicamCaptureFormat->es->video.height = 720;
                    // raspicamCaptureFormat->es->video.crop.x = 0;
                    // raspicamCaptureFormat->es->video.crop.y = 0;
                    // raspicamCaptureFormat->es->video.crop.width = 1280;
                    // raspicamCaptureFormat->es->video.crop.height = 720;
                    // raspicamCaptureFormat->es->video.frame_rate.num = 30;
                    // raspicamCaptureFormat->es->video.frame_rate.den = 1;
                    // m_raspicamCapturePort->buffer_size = 1280*720*12/8;
                    // m_raspicamCapturePort->buffer_num = 3;
                    
                    // Commit port format
                    // if(mmal_port_format_commit(m_raspicamCapturePort) != MMAL_SUCCESS)
                    //   {
                    //     cleanUp();
                    //     throw runtime::OperatorAllocationFailed("Raspi","Could not commit capture port format.");
                    //   }
                }
                break;
                default:
                    throw runtime::WrongParameterValue(parameter(CAMERA_MODE), *this);
            
            }
            
            std::vector<const runtime::Description*> inputs;
            std::vector<const runtime::Description*> outputs;
            OperatorKernel::initialize(inputs,outputs,setupParameters());
        }

        void RaspiCam::deinitialize()
        {
            if(m_raspicam)
            {
                mmal_component_destroy(m_raspicam);
            }
            bcm_host_deinit();
            OperatorKernel::deinitialize();
        }

        const std::vector<const runtime::Parameter*> RaspiCam::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            // Specific parameters
            if(m_cameraMode == RaspiCam::VIDEO)
            {
                runtime::Parameter* frameRate = new runtime::Parameter(FRAME_RATE,runtime::DataVariant::FLOAT_32);
                frameRate->setTitle("Frame rate");
                frameRate->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                parameters.push_back(frameRate);
            }

            // Common parameters
            runtime::EnumParameter* autoWhiteBalance = new runtime::EnumParameter(AWB_MODE);
            autoWhiteBalance->setTitle("AWB mode");
            autoWhiteBalance->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            autoWhiteBalance->add(runtime::EnumDescription(runtime::Enum(MMAL_PARAM_AWBMODE_OFF), ("Off")));
            autoWhiteBalance->add(runtime::EnumDescription(runtime::Enum(MMAL_PARAM_AWBMODE_AUTO), ("Auto")));
            autoWhiteBalance->add(runtime::EnumDescription(runtime::Enum(MMAL_PARAM_AWBMODE_SUNLIGHT), ("Sunlight")));
            autoWhiteBalance->add(runtime::EnumDescription(runtime::Enum(MMAL_PARAM_AWBMODE_CLOUDY), ("Cloudy")));
            autoWhiteBalance->add(runtime::EnumDescription(runtime::Enum(MMAL_PARAM_AWBMODE_SHADE), ("Shade")));
            parameters.push_back(autoWhiteBalance);

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
                MMAL_STATUS_T status;

                //Create buffer pool in stromx (recycling)
                runtime::Data* recycleBuffer = NULL;

                // delete all remaining buffers in the recycling access
                while((recycleBuffer = m_recycleBuffers()))
                    delete recycleBuffer;

                // allocate one buffers and add them to the recycler
                m_recycleBuffers.add(runtime::DataContainer(new cvsupport::Image(0)));
            
                // Create output buffer pool
                m_outBufferPool = mmal_port_pool_create(m_raspicamVideoPort, m_raspicamVideoPort->buffer_num, m_raspicamVideoPort->buffer_size_recommended);
                
                if(m_outBufferPool == NULL)
                {
                    cleanUp();
                    throw runtime::OperatorAllocationFailed("Raspi","Could not create ouput buffer pool.");
                }

                // Create output buffer queue
                m_outQueue = mmal_queue_create();               
                if(m_outQueue == NULL)
                {
                    cleanUp();
                    throw runtime::OperatorAllocationFailed("Raspi","Could not create output buffer queue.");
                }
                m_raspicamVideoPort->userdata = (MMAL_PORT_USERDATA_T*)m_outQueue;

                status = mmal_port_enable(m_raspicamVideoPort, callbackOutVideoPort);               
                if(status != MMAL_SUCCESS)
                {
                    cleanUp();
                    throw runtime::OperatorAllocationFailed("Raspi","Could not enable video port of camera");
                }
                
                status = mmal_component_enable(m_raspicam);
                if(status != MMAL_SUCCESS)
                {
                    cleanUp();
                    throw runtime::OperatorAllocationFailed("Raspi","Could not enable camera.");
                }
                if(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT]->is_enabled)
                {
                    MMAL_BUFFER_HEADER_T*  bufferNew;
                    while((bufferNew = mmal_queue_get(m_outBufferPool->queue)) != NULL)
                    {
                        mmal_port_send_buffer(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT], bufferNew);
                    }
                }
            // Activate capture function of port
            switch (m_cameraMode)
            {
                case VIDEO:
                    mmal_port_parameter_set_boolean(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT],MMAL_PARAMETER_CAPTURE, 1);
                    break;
                case STILL:
                    // mmal_port_parameter_set_boolean(m_raspicam->output[MMAL_CAMERA_CAPTURE_PORT],MMAL_PARAMETER_CAPTURE,1);
                    break;
                default:
                    throw runtime::WrongParameterValue(parameter(CAMERA_MODE), *this);
            }
        }

        void RaspiCam::deactivate()
        {

            MMAL_STATUS_T status;
            // Stop capture function of port and disable processing on port
            switch (m_cameraMode)
            {
                case VIDEO:
                    mmal_port_parameter_set_boolean(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT],MMAL_PARAMETER_CAPTURE, 0);
                    status = mmal_port_disable(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT]);
                    break;
                case STILL:
                    // mmal_port_parameter_set_boolean(m_raspicam->output[MMAL_CAMERA_CAPTURE_PORT],MMAL_PARAMETER_CAPTURE,0);
                    // status = mmal_port_disable(m_raspicam->output[MMAL_CAMERA_CAPTURE_PORT]);
                    break;
                default:
                    throw runtime::WrongParameterValue(parameter(CAMERA_MODE), *this);
            }

            // Deactivate component
            status = mmal_component_disable(m_raspicam);

            // Destroy queue
            mmal_queue_destroy(m_outQueue);
            m_outQueue = NULL;

            // Destroy buffers
            // MMAL buffers
            mmal_port_pool_destroy(m_raspicam->output[MMAL_CAMERA_VIDEO_PORT], m_outBufferPool);
            m_outBufferPool = NULL;

            // Stromx buffers (recycling)
            runtime::Data* recycleBuffer = NULL;

            // delete all remaining buffers in the recycling access
            while((recycleBuffer = m_recycleBuffers()))
                delete recycleBuffer;
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
                    recycleBuffer->resize(1280,720,runtime::Image::BGR_24);
                    mmal_buffer_header_mem_lock(buffer);
                    memcpy(recycleBuffer->data(), buffer->data, 1280*720*3);
                    mmal_buffer_header_mem_unlock(buffer);
                    mmal_buffer_header_release(buffer);

                    runtime::DataContainer outContainerIndex = runtime::DataContainer(new runtime::UInt32(m_frameIndex));
                    runtime::DataContainer outContainer = runtime::DataContainer(recycleBuffer);
                    
                    //remember it in the recycling access
                    m_recycleBuffers.add(outContainer);

                    runtime::Id2DataPair outputDataMapperIndex(OUTPUT_FRAME_INDEX, outContainerIndex);
                    runtime::Id2DataPair outputDataMapperFrames(OUTPUT_FRAMES, outContainer);
                    provider.sendOutputData(outputDataMapperIndex);
                    provider.sendOutputData(outputDataMapperFrames);
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
            MMAL_QUEUE_T* queue = (MMAL_QUEUE_T*)port->userdata;
            lock_t l(mutex);
            mmal_queue_put(queue,buffer);
            cond.notify_all();
        }
    }
}

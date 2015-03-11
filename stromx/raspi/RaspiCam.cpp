
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

#include <bcm_host.h>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <interface/mmal/util/mmal_default_components.h>
#include <interface/mmal/util/mmal_util.h>
#include <interface/mmal/util/mmal_util_params.h>
#include <stromx/cvsupport/Image.h>
#include <stromx/runtime/Data.h>
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/OperatorException.h>

#define MMAL_CAMERA_VIDEO_PORT 1
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

            runtime::Description* outputFrames = new runtime::Description(OUTPUT_FRAMES, runtime::Variant::RGB_IMAGE);
            outputFrames->setTitle("Output frames");
            outputs.push_back(outputFrames);

            runtime::Description* outputFrameIndex = new runtime::Description(OUTPUT_FRAME_INDEX, runtime::Variant::UINT_32);
            outputFrameIndex->setTitle("Output image counter");
            outputs.push_back(outputFrameIndex);

            return outputs;
        }

        const std::vector< const runtime::Parameter* > RaspiCam::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            //m_cameraModeParameter = new runtime::EnumParameter(CAMERA_MODE);
            //m_cameraModeParameter->setAccessMode(runtime::Parameter::NONE_WRITE);
            //m_cameraModeParameter->setTitle(("Camera mode"));
            //m_cameraModeParameter->add(runtime::EnumDescription(runtime::Enum(STILL), ("Still")));
            //m_cameraModeParameter->add(runtime::EnumDescription(runtime::Enum(VIDEO), ("Video")));
            //parameters.push_back(m_cameraModeParameter);

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
            m_currentPort(NULL),
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
                deinitialize();
                throw runtime::OperatorError(*this,"Could not create mmal default camera.");
            }
            if(!m_raspicam->output_num)
            {
                deinitialize();
                throw runtime::OperatorError(*this,"Default mmal camera has no outputs.");
            }

            switch(m_cameraMode)
            {
                case VIDEO:
                {
                    //Get the pointer
                    m_currentPort = m_raspicam->output[MMAL_CAMERA_VIDEO_PORT];
                    MMAL_ES_FORMAT_T* raspicamVideoFormat = m_currentPort->format;

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
                    m_currentPort->buffer_num = 10;

                    //Commit port format
                    if(mmal_port_format_commit(m_currentPort) != MMAL_SUCCESS)
                    {
                        deinitialize();
                        throw runtime::OperatorError(*this,"Could not commit video port format.");
                    }
                }
                break;
                case STILL:
                {
                    //Get the pointer
                    m_currentPort = m_raspicam->output[MMAL_CAMERA_CAPTURE_PORT];
                    MMAL_ES_FORMAT_T* raspicamCaptureFormat = m_currentPort->format;

                    //Set up the format
                    raspicamCaptureFormat->encoding_variant = MMAL_ENCODING_BGR24;
                    raspicamCaptureFormat->encoding = MMAL_ENCODING_BGR24;
                    raspicamCaptureFormat->es->video.width = 1280;
                    raspicamCaptureFormat->es->video.height = 720;
                    raspicamCaptureFormat->es->video.crop.x = 0;
                    raspicamCaptureFormat->es->video.crop.y = 0;
                    raspicamCaptureFormat->es->video.crop.width = 1280;
                    raspicamCaptureFormat->es->video.crop.height = 720;
                    raspicamCaptureFormat->es->video.frame_rate.num = 1;
                    raspicamCaptureFormat->es->video.frame_rate.den = 1;
                    m_currentPort->buffer_num = 3;

                    // Commit port format
                    if(mmal_port_format_commit(m_currentPort) != MMAL_SUCCESS)
                    {
                        deinitialize();
                        throw runtime::OperatorError(*this,"Could not commit capture port format.");
                    }
                }
                break;
                default:
                    throw runtime::WrongParameterValue(parameter(m_cameraMode), *this);

            }

            std::vector<const runtime::Description*> inputs;
            std::vector<const runtime::Description*> outputs;
            OperatorKernel::initialize(inputs,outputs,setupParameters());
        }

        void RaspiCam::deinitialize()
        {
            m_currentPort = NULL;
            if(m_raspicam)
            {
                mmal_component_destroy(m_raspicam);
                m_raspicam = NULL;
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
                runtime::Parameter* frameRate = new runtime::Parameter(FRAME_RATE,runtime::Variant::FLOAT_32);
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

        void RaspiCam::activate()
        {
                MMAL_STATUS_T status;

                //Create buffer pool in stromx (recycling)
                runtime::Data* recycleBuffer = NULL;

                // delete all remaining buffers in the recycling access
                while((recycleBuffer = m_recycleBuffers()))
                    delete recycleBuffer;

                // allocate one buffer and add it to the recycler
                m_recycleBuffers.add(runtime::DataContainer(new cvsupport::Image(0)));

                // Create output buffer pool
                m_outBufferPool = mmal_port_pool_create(m_currentPort, m_currentPort->buffer_num, m_currentPort->buffer_size_recommended);
                if(m_outBufferPool == NULL)
                {
                    deactivate();
                    throw runtime::OperatorError(*this,"Could not create output buffer pool.");
                }

                // Create output buffer queue
                m_outQueue = mmal_queue_create();
                if(m_outQueue == NULL)
                {
                    deactivate();
                    throw runtime::OperatorError(*this,"Could not create output buffer queue.");
                }
                m_currentPort->userdata = (MMAL_PORT_USERDATA_T*)m_outQueue;

                // Enable port
                status = mmal_port_enable(m_currentPort, callbackOutVideoPort);
                if(status != MMAL_SUCCESS)
                {
                    deactivate();
                    throw runtime::OperatorError(*this,"Could not enable port of camera");
                }

                // Enable camera component
                status = mmal_component_enable(m_raspicam);
                if(status != MMAL_SUCCESS)
                {
                    deactivate();
                    throw runtime::OperatorError(*this,"Could not enable camera.");
                }

                // Send buffer to enabled port
                if(m_currentPort->is_enabled)
                {
                    MMAL_BUFFER_HEADER_T*  bufferNew;
                    while((bufferNew = mmal_queue_get(m_outBufferPool->queue)) != NULL)
                    {
                        mmal_port_send_buffer(m_currentPort, bufferNew);
                    }
                }

                // Activate capture function of port
                mmal_port_parameter_set_boolean(m_currentPort,MMAL_PARAMETER_CAPTURE, 1);
        }

        void RaspiCam::deactivate()
        {

            MMAL_STATUS_T status;
            // Stop capture function of port and disable processing on port
            mmal_port_parameter_set_boolean(m_currentPort,MMAL_PARAMETER_CAPTURE, 0);
            status = mmal_port_disable(m_currentPort);

            // Deactivate component
            status = mmal_component_disable(m_raspicam);

            // Destroy queue
            mmal_queue_destroy(m_outQueue);
            m_outQueue = NULL;

            // Destroy buffers
            mmal_port_pool_destroy(m_currentPort, m_outBufferPool);
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
                    // TODO: potential segmentation fault caused by fixed resolution
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
            if(m_currentPort->is_enabled)
            {
                bufferNew = mmal_queue_get(m_outBufferPool->queue);
                if(bufferNew)
                {
                    status = mmal_port_send_buffer(m_currentPort, bufferNew);
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

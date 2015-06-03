/* 
*  Copyright 2014 Matthias Fuchs
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

#include "stromx/raspi/RaspiCam.h"
#include "stromx/raspi/test/RaspiCamTest.h"
#include <cppunit/TestAssert.h>
#include <stromx/cvsupport/Image.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>


CPPUNIT_TEST_SUITE_REGISTRATION (stromx::raspi::RaspiCamTest);

namespace stromx
{
    using namespace runtime;

    namespace raspi
    {
        namespace
        {
            // Unsupported auto white balance mode
            const int UNSUPPORTED_AWB_MODE = 100000;
        }

        void RaspiCamTest::setAndGetParameterAutoWhiteBalance(const MMAL_PARAM_AWBMODE_T& mode)
        {
            runtime::DataRef paramValue;
            runtime::Enum paramValueCasted;
            CPPUNIT_ASSERT_NO_THROW(m_operator->setParameter(RaspiCam::AWB_MODE, runtime::Enum(mode)));
            CPPUNIT_ASSERT_NO_THROW(paramValue = m_operator->getParameter(RaspiCam::AWB_MODE));
            CPPUNIT_ASSERT_NO_THROW(paramValueCasted = runtime::data_cast<runtime::Enum>(paramValue));
            CPPUNIT_ASSERT_EQUAL(MMAL_PARAM_AWBMODE_T(int(paramValueCasted)),mode);
        }


        void RaspiCamTest::setUp()
        {
            m_operator = new OperatorTester(new RaspiCam());
        }

        void RaspiCamTest::testInitialize()
        {
            CPPUNIT_ASSERT_NO_THROW(m_operator->initialize());
        }

        void RaspiCamTest::testDeinitialize()
        {
            m_operator->initialize();
            CPPUNIT_ASSERT_NO_THROW(m_operator->deinitialize());
        }

        void RaspiCamTest::testReInitialize()
        {
            m_operator->initialize();
            m_operator->deinitialize();
            CPPUNIT_ASSERT_NO_THROW(m_operator->initialize());
            m_operator->deinitialize();
        }

        void RaspiCamTest::testActivate()
        {
            m_operator->initialize();
            CPPUNIT_ASSERT_NO_THROW(m_operator->activate());
        }

        void RaspiCamTest::testDeactivate()
        {
            m_operator->initialize();
            m_operator->activate();
            CPPUNIT_ASSERT_NO_THROW(m_operator->deactivate());
        }

        void RaspiCamTest::testReactivate()
        {
            m_operator->initialize();
            m_operator->activate();
            m_operator->deactivate();
            CPPUNIT_ASSERT_NO_THROW(m_operator->activate());
        }

        void RaspiCamTest::testExecuteVideo()
        {
            //m_operator->setParameter(RaspiCam::CAMERA_MODE, runtime::Enum(RaspiCam::VIDEO));
            m_operator->initialize();
            m_operator->setParameter(RaspiCam::AWB_MODE, runtime::Enum(MMAL_PARAM_AWBMODE_AUTO));
            m_operator->setParameter(RaspiCam::FRAME_RATE, runtime::Float32(5));
            m_operator->activate();

            for(unsigned int i = 0; i < 10; ++i)
            {
                DataContainer dataFrame = m_operator->getOutputData(RaspiCam::OUTPUT_FRAMES);
                DataContainer dataFrameIndex = m_operator->getOutputData(RaspiCam::OUTPUT_FRAME_INDEX);
                m_operator->clearOutputData(RaspiCam::OUTPUT_FRAMES);
                m_operator->clearOutputData(RaspiCam::OUTPUT_FRAME_INDEX);
            }
            DataContainer dataFrame = m_operator->getOutputData(RaspiCam::OUTPUT_FRAMES);
            DataContainer dataFrameIndex = m_operator->getOutputData(RaspiCam::OUTPUT_FRAME_INDEX);
            runtime::ReadAccess access(dataFrame);
            cvsupport::Image::save("RaspiCamTestExecuteVideoOutput.png", access.get<runtime::Image>());
        }

        void RaspiCamTest::testExecuteCapture()
        {
            m_operator->setParameter(RaspiCam::CAMERA_MODE, runtime::Enum(RaspiCam::STILL));
            m_operator->initialize();
            m_operator->setParameter(RaspiCam::AWB_MODE, runtime::Enum(MMAL_PARAM_AWBMODE_AUTO));
            m_operator->activate();
            for(unsigned int i = 0; i < 10; ++i)
            {
                DataContainer dataFrame = m_operator->getOutputData(RaspiCam::OUTPUT_FRAMES);
                DataContainer dataFrameIndex = m_operator->getOutputData(RaspiCam::OUTPUT_FRAME_INDEX);
                m_operator->clearOutputData(RaspiCam::OUTPUT_FRAMES);
                m_operator->clearOutputData(RaspiCam::OUTPUT_FRAME_INDEX);
            }
            DataContainer dataFrame = m_operator->getOutputData(RaspiCam::OUTPUT_FRAMES);
            DataContainer dataFrameIndex = m_operator->getOutputData(RaspiCam::OUTPUT_FRAME_INDEX);
            runtime::ReadAccess access(dataFrame);
            cvsupport::Image::save("RaspiCamTestExecuteCaptureOutput.png", access.get<runtime::Image>());
        }

        void RaspiCamTest::testSetParameterCameraModeVideo()
        {
            CPPUNIT_ASSERT_NO_THROW(m_operator->setParameter(RaspiCam::CAMERA_MODE, runtime::Enum(RaspiCam::VIDEO)));
        }

        void RaspiCamTest::testGetParameterCameraModeVideo()
        {
            m_operator->setParameter(RaspiCam::CAMERA_MODE, runtime::Enum(RaspiCam::VIDEO));
            runtime::DataRef returnValue;
            CPPUNIT_ASSERT_NO_THROW(returnValue = m_operator->getParameter(RaspiCam::CAMERA_MODE));
            CPPUNIT_ASSERT_EQUAL(runtime::Enum(RaspiCam::VIDEO),runtime::data_cast<runtime::Enum>(returnValue));
        }

        void RaspiCamTest::testSetParameterFramerateVideo()
        {
            //m_operator->setParameter(RaspiCam::CAMERA_MODE, runtime::Enum(RaspiCam::VIDEO));
            m_operator->initialize(); 
            CPPUNIT_ASSERT_NO_THROW(m_operator->setParameter(RaspiCam::FRAME_RATE, runtime::Float32(5)));
        }

        void RaspiCamTest::testGetParameterFramerateVideo()
        {
            //m_operator->setParameter(RaspiCam::CAMERA_MODE, runtime::Enum(RaspiCam::VIDEO));
            m_operator->initialize(); 
            m_operator->setParameter(RaspiCam::FRAME_RATE, runtime::Float32(5));
            runtime::DataRef returnValue;
            CPPUNIT_ASSERT_NO_THROW(returnValue = m_operator->getParameter(RaspiCam::FRAME_RATE));
            CPPUNIT_ASSERT_EQUAL(runtime::Float32(5),runtime::data_cast<runtime::Float32>(returnValue));
        }

        void RaspiCamTest::testSetAndGetParameterAutoWhiteBalanceVideoInitialized()
        {
            //m_operator->setParameter(RaspiCam::CAMERA_MODE, runtime::Enum(RaspiCam::VIDEO));
            m_operator->initialize();

            // Test all supported modes for initialized operator
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_OFF);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_AUTO);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SUNLIGHT);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_CLOUDY);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SHADE);

            // Test unsupported mode for initialized operator
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(RaspiCam::AWB_MODE, runtime::Enum(UNSUPPORTED_AWB_MODE)), runtime::ParameterError);
        }

        void RaspiCamTest::testSetAndGetParameterAutoWhiteBalanceVideoActivated()
        {
            //m_operator->setParameter(RaspiCam::CAMERA_MODE, runtime::Enum(RaspiCam::VIDEO));
            m_operator->initialize();
            m_operator->activate();

            // Test all supported modes for activated operator
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_OFF);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_AUTO);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SUNLIGHT);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_CLOUDY);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SHADE);

            // Test unsupported mode for activated operator
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(RaspiCam::AWB_MODE, runtime::Enum(UNSUPPORTED_AWB_MODE)), runtime::ParameterError);
        }

        void RaspiCamTest::testSetParameterAutoWhiteBalanceCapture()
        {
            m_operator->setParameter(RaspiCam::CAMERA_MODE, runtime::Enum(RaspiCam::STILL));
            m_operator->initialize();

            // Test all supported modes in deactivated stage of stream
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_OFF);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_AUTO);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SUNLIGHT);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_CLOUDY);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SHADE);

            // Test unsupported mode in deactivated stage of stream
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(RaspiCam::AWB_MODE, runtime::Enum(UNSUPPORTED_AWB_MODE)), runtime::ParameterError);

            // Test all supported modes in activated stage of stream
            m_operator->activate();
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_OFF);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_AUTO);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SUNLIGHT);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_CLOUDY);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SHADE);

            // Test unsupported mode in activated stage of stream
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(RaspiCam::AWB_MODE, runtime::Enum(UNSUPPORTED_AWB_MODE)), runtime::ParameterError);
        }

        void RaspiCamTest::tearDown()
        {
            delete m_operator;
        }
    }
}

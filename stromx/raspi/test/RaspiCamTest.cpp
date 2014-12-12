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

#include "stromx/raspi/test/RaspiCamTest.h"
#include <cppunit/TestAssert.h>
#include "stromx/raspi/RaspiCam.h"
#include "stromx/runtime/OperatorException.h"


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

        void RaspiCamTest::testExecute()
        {
            m_operator->initialize(); 
            m_operator->activate();
            DataContainer dataFrames = m_operator->getOutputData(RaspiCam::OUTPUT_FRAMES);
            DataContainer dataFrameIndex = m_operator->getOutputData(RaspiCam::OUTPUT_FRAME_INDEX);
        }

        void RaspiCamTest::testSetParameterCameraModeVideo()
        {
            m_operator->setParameter(RaspiCam::CAMERA_MODE, runtime::Enum(RaspiCam::VIDEO));
        }

        void RaspiCamTest::testSetParameterFramerateVideo()
        {
            m_operator->setParameter(RaspiCam::CAMERA_MODE, runtime::Enum(RaspiCam::VIDEO));
            m_operator->initialize(); 
            m_operator->setParameter(RaspiCam::FRAME_RATE, runtime::Float32(5));
        }

        void RaspiCamTest::testSetAndGetParameterAutoWhiteBalanceVideo()
        {   
            CPPUNIT_ASSERT_NO_THROW(m_operator->setParameter(RaspiCam::CAMERA_MODE, runtime::Enum(RaspiCam::VIDEO)));
            CPPUNIT_ASSERT_NO_THROW(m_operator->initialize());

            // Test all supported modes in deactivated stage of stream          
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_OFF);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_AUTO);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SUNLIGHT);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_CLOUDY);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SHADE);

            // Test unsupported mode in deactivated stage of stream
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(RaspiCam::AWB_MODE, runtime::Enum(UNSUPPORTED_AWB_MODE)), runtime::ParameterError);

            // Test all supported modes in activated stage of stream
            CPPUNIT_ASSERT_NO_THROW(m_operator->activate());
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_OFF);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_AUTO);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SUNLIGHT);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_CLOUDY);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SHADE);

            // Test unsupported mode in activated stage of stream
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(RaspiCam::AWB_MODE, runtime::Enum(UNSUPPORTED_AWB_MODE)), runtime::ParameterError);

            CPPUNIT_ASSERT_NO_THROW(m_operator->deactivate());
        }

        void RaspiCamTest::testSetParameterAutoWhiteBalanceCapture()
        {
            CPPUNIT_ASSERT_NO_THROW(m_operator->setParameter(RaspiCam::CAMERA_MODE, runtime::Enum(RaspiCam::STILL)));
            CPPUNIT_ASSERT_NO_THROW(m_operator->initialize());

            // Test all supported modes in deactivated stage of stream
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_OFF);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_AUTO);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SUNLIGHT);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_CLOUDY);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SHADE);

            // Test unsupported mode in deactivated stage of stream
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(RaspiCam::AWB_MODE, runtime::Enum(UNSUPPORTED_AWB_MODE)), runtime::ParameterError);

            // Test all supported modes in activated stage of stream
            CPPUNIT_ASSERT_NO_THROW(m_operator->activate());
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_OFF);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_AUTO);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SUNLIGHT);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_CLOUDY);
            setAndGetParameterAutoWhiteBalance(MMAL_PARAM_AWBMODE_SHADE);

            // Test unsupported mode in activated stage of stream
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(RaspiCam::AWB_MODE, runtime::Enum(UNSUPPORTED_AWB_MODE)), runtime::ParameterError);

            CPPUNIT_ASSERT_NO_THROW(m_operator->deactivate());
        }


        void RaspiCamTest::testSetResolution()
        {
            m_operator->initialize();
            m_operator->setParameter(RaspiCam::RESOLUTION,Enum(RaspiCam::VGA));
        }
        
        void RaspiCamTest::tearDown()
        {
            delete m_operator;
        }
    }
}

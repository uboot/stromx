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

#ifndef STROMX_RASPI_RASPICAMTEST_H
#define STROMX_RASPI_RASPICAMTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/OperatorTester.h"
#include <interface/mmal/util/mmal_util_params.h>

namespace stromx
{
    namespace raspi
    {
        class RaspiCamTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (RaspiCamTest);
            CPPUNIT_TEST(testInitialize);
            CPPUNIT_TEST(testDeinitialize);
            CPPUNIT_TEST(testReInitialize);
            CPPUNIT_TEST(testActivate);
            CPPUNIT_TEST(testDeactivate);
            CPPUNIT_TEST(testReactivate);
            CPPUNIT_TEST(testExecuteVideo);
            //CPPUNIT_TEST(testExecuteCapture);
            //CPPUNIT_TEST(testSetParameterCameraModeVideo);
            //CPPUNIT_TEST(testGetParameterCameraModeVideo);
            CPPUNIT_TEST(testSetParameterFramerateVideo);
            CPPUNIT_TEST(testGetParameterFramerateVideo);
            CPPUNIT_TEST(testSetAndGetParameterAutoWhiteBalanceVideoInitialized);
            CPPUNIT_TEST(testSetAndGetParameterAutoWhiteBalanceVideoActivated);
            //CPPUNIT_TEST(testSetParameterAutoWhiteBalanceCapture);
            CPPUNIT_TEST_SUITE_END ();

        public:
            RaspiCamTest() : m_operator(0) {}

            void setUp();
            void tearDown();

        protected:
            void testInitialize();
            void testDeinitialize();
            void testReInitialize();
            void testActivate();
            void testDeactivate();
            void testReactivate();
            void testExecuteVideo();
            void testExecuteCapture();
            void testSetParameterCameraModeVideo();
            void testGetParameterCameraModeVideo();
            void testSetParameterFramerateVideo();
            void testGetParameterFramerateVideo();
            void testSetAndGetParameterAutoWhiteBalanceVideoInitialized();
            void testSetAndGetParameterAutoWhiteBalanceVideoActivated();
            void testSetParameterAutoWhiteBalanceCapture();

        private:
            void setAndGetParameterAutoWhiteBalance(const MMAL_PARAM_AWBMODE_T& mode);
            runtime::OperatorTester* m_operator;
        };
    }
}

#endif // STROMX_RASPI_RASPICAMTEST_H

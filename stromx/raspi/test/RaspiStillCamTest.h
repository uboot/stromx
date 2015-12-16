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

#ifndef STROMX_RASPI_RASPISTILLCAMTEST_H
#define STROMX_RASPI_RASPISTILLCAMTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/OperatorTester.h"

namespace stromx
{
    namespace raspi
    {
        class RaspiStillCamTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (RaspiStillCamTest);
            CPPUNIT_TEST(testInitialize);
            CPPUNIT_TEST(testInitializeTwice);
            CPPUNIT_TEST(testDeinitialize);
            CPPUNIT_TEST(testActivate);
            CPPUNIT_TEST(testDeactivate);
            CPPUNIT_TEST(testExecute);
            CPPUNIT_TEST(testExecuteWithTrigger);
            CPPUNIT_TEST(testExecuteTwice);
            CPPUNIT_TEST(testExecuteTwoBuffers);
            CPPUNIT_TEST(testGetShutterSpeed);
            CPPUNIT_TEST(testSetShutterSpeed);
            CPPUNIT_TEST(testSetShutterSpeedAndExecute);
            CPPUNIT_TEST(testSetResolution);
            CPPUNIT_TEST(testFramesPerSecond640);
            CPPUNIT_TEST(testFramesPerSecond1280);
            CPPUNIT_TEST(testFramesPerSecond2560);
            CPPUNIT_TEST(testFramesPerSecondRoi);
            CPPUNIT_TEST(testGetRoi);
            CPPUNIT_TEST(testSetRoi);
            CPPUNIT_TEST(testGetAwbMode);
            CPPUNIT_TEST(testSetAwbMode);
            CPPUNIT_TEST(testSetAwbModeOffAndExecute);
            CPPUNIT_TEST(testSetAwbModeSunlightAndExecute);
            CPPUNIT_TEST(testSetRoiAndExecute);
            CPPUNIT_TEST(testGetAwbGains);
            CPPUNIT_TEST(testSetAwbGains);
            CPPUNIT_TEST_SUITE_END ();

        public:
            RaspiStillCamTest() : m_operator(0) {}

            void setUp();
            void tearDown();

        protected:
            void testInitialize();
            void testInitializeTwice();
            void testDeinitialize();
            void testActivate();
            void testDeactivate();
            void testExecute();
            void testExecuteWithTrigger();
            void testExecuteTwice();
            void testExecuteTwoBuffers();
            void testGetShutterSpeed();
            void testSetShutterSpeed();
            void testSetShutterSpeedAndExecute();
            void testSetResolution();
            void testFramesPerSecond640();
            void testFramesPerSecond1280();
            void testFramesPerSecond2560();
            void testFramesPerSecondRoi();
            void testGetRoi();
            void testSetRoi();
            void testGetAwbMode();
            void testSetAwbMode();
            void testSetAwbModeOffAndExecute();
            void testSetAwbModeSunlightAndExecute();
            void testSetRoiAndExecute();
            void testGetAwbGains();
            void testSetAwbGains();

        private:
            runtime::OperatorTester* m_operator;
        };
    }
}

#endif // STROMX_RASPI_RASPISTILLCAMTEST_H

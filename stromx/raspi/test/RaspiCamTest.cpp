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
#include <cppunit/TestAssert.h>
#include "stromx/raspi/RaspiCam.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::raspi::RaspiCamTest);

namespace stromx
{
    using namespace runtime;

    namespace raspi
    {
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
	
	    void RaspiCamTest::testSetParameterFramerate()
	    {
	        m_operator->setParameter(RaspiCam::CAMERA_MODE, runtime::Enum(RaspiCam::VIDEO));
	        m_operator->initialize(); 
	        m_operator->setParameter(RaspiCam::FRAME_RATE, runtime::Float32(5));
	    }

        void RaspiCamTest::tearDown()
        {
            delete m_operator;
        }
    }
}

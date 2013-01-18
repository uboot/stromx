/* 
*  Copyright 2013 Thomas Fidler
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

#include "WebCameraTest.h"
#include "../WebCamera.h"
#include <stromx/runtime/OperatorTester.h>
#include <stromx/runtime/OperatorException.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::example::WebCameraTest);

namespace stromx
{
    namespace example
    {
        void WebCameraTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new WebCamera);
            try
            {
                m_operator->initialize();
            }
            catch(stromx::runtime::OperatorError&)
            {
                m_hasCamera = false;
            }
        }

        void WebCameraTest::testSetParameter()
        {
            if(m_hasCamera)
            {
            }
        }

        void WebCameraTest::testGetParameter()
        {
            if(m_hasCamera)
            {
            }
        }
        
        void WebCameraTest::testExecute()
        {
            if(m_hasCamera)
            {
            }
        }

        void WebCameraTest::tearDown()
        {
            delete m_operator;
        }

    }
}

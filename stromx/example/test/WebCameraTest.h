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

#ifndef STROMX_EXAMPLE_WEBCAMERATEST_H
#define STROMX_EXAMPLE_WEBCAMERATEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

namespace stromx
{
    namespace runtime
    {
        class OperatorTester;
    }
    
    namespace example
    {
        class WebCameraTest : public CPPUNIT_NS :: TestFixture
        {
            
            
            CPPUNIT_TEST_SUITE (WebCameraTest);
            CPPUNIT_TEST (testSetParameter);
            CPPUNIT_TEST (testGetParameter);
            CPPUNIT_TEST (testExecute);
            CPPUNIT_TEST_SUITE_END ();
            
            public:
                WebCameraTest():m_operator(0),m_hasCamera(true){}
                
                void setUp();
                void tearDown();
                
            protected:
                void testSetParameter();
                void testGetParameter();
                void testExecute();
                
            private:
                runtime::OperatorTester* m_operator;
                bool m_hasCamera;
        };
    }
}

#endif // STROMX_EXAMPLE_WEBCAMERATEST_H

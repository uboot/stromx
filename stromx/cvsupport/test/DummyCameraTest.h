/* 
*  Copyright 2011 Matthias Fuchs
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

#ifndef STROMX_CVSUPPORT_DUMMYCAMERATEST_H
#define STROMX_CVSUPPORT_DUMMYCAMERATEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

namespace stromx
{
    namespace runtime
    {
        class DataContainer;
        class OperatorTester;
    }

    namespace cvsupport
    {
        class DummyCameraTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (DummyCameraTest);
            CPPUNIT_TEST (testExecuteSoftwareTrigger);
            CPPUNIT_TEST (testExecuteInternalTrigger);
            CPPUNIT_TEST (testAdjustRoi);
            CPPUNIT_TEST (testAdjustPixelType);
            CPPUNIT_TEST (testAdjustExposure);
            CPPUNIT_TEST (testAdjustWhiteBalance);
            CPPUNIT_TEST (testFlicker);
            CPPUNIT_TEST (testValidateBufferSize);
            CPPUNIT_TEST_SUITE_END ();

            public:
                DummyCameraTest() : m_operator(0) {}
                
                void setUp();
                void tearDown();

            protected:
                void testExecuteSoftwareTrigger();
                void testExecuteInternalTrigger();
                void testAdjustRoi();
                void testAdjustPixelType();
                void testAdjustExposure();
                void testAdjustWhiteBalance();
                void testFlicker();
                void testValidateBufferSize();
                
            private:
                runtime::OperatorTester* m_operator;
        };
    }
}

#endif // STROMX_CVSUPPORT_DUMMYCAMERATEST_H
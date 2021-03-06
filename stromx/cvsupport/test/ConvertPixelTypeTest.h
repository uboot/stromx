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

#ifndef STROMX_CVSUPPORT_CONVERTPIXELTYPETEST_H
#define STROMX_CVSUPPORT_CONVERTPIXELTYPETEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <stromx/runtime/DataContainer.h>

namespace stromx
{
    namespace runtime
    {
        class OperatorTester;
    }

    namespace cvsupport
    {
        class ConvertPixelTypeTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (ConvertPixelTypeTest);
            CPPUNIT_TEST (testExecuteMono8Manual);
            CPPUNIT_TEST (testExecuteBayerBg8Manual);
            CPPUNIT_TEST (testExecuteBayerRgb24Manual);
            CPPUNIT_TEST (testExecuteIdenticalInputsManual);
            CPPUNIT_TEST (testExecuteMono8Allocate);
            CPPUNIT_TEST_SUITE_END ();

        public:
                ConvertPixelTypeTest() : m_operator(0) {}
                
                void setUp();
                void tearDown();

            protected:
                void testExecuteMono8Manual();
                void testExecuteBayerBg8Manual();
                void testExecuteBayerRgb24Manual();
                void testExecuteIdenticalInputsManual();
                void testExecuteMono8Allocate();
                
            private:
                runtime::OperatorTester* m_operator;
                runtime::DataContainer m_source;
                
                void initializeOperator(unsigned int dataFlow);
        };
    }
}

#endif // STROMX_CVSUPPORT_CONVERTPIXELTYPETEST_H
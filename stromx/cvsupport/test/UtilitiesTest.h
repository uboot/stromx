/* 
*  Copyright 2013 Matthias Fuchs
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

#ifndef STROMX_CVSUPPORT_UTILITIESTEST_H
#define STROMX_CVSUPPORT_UTILITIESTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <stromx/runtime/Variant.h>

namespace stromx
{
    namespace cvsupport
    {

        class UtilitiesTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (UtilitiesTest);
            CPPUNIT_TEST (testComputeOutPixelTypeSame);
            CPPUNIT_TEST (testComputeOutPixelType8Bit);
            CPPUNIT_TEST (testComputeOutPixelType16Bit);
            CPPUNIT_TEST (getOpenCvMat3Channels);
            CPPUNIT_TEST_SUITE_END ();

            public:
                UtilitiesTest() {}
                
                void setUp() {}
                void tearDown() {}

            protected:
                void testComputeOutPixelTypeSame();
                void testComputeOutPixelType8Bit();
                void testComputeOutPixelType16Bit();
                void getOpenCvMat3Channels();
                
            private:
                runtime::Factory* m_factory;
        };
    }
}

#endif // STROMX_CVSUPPORT_UTILITIESTEST_H
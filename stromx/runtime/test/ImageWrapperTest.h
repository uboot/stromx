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

#ifndef STROMX_RUNTIME_IMAGEWRAPPERTEST_H
#define STROMX_RUNTIME_IMAGEWRAPPERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <stromx/runtime/Version.h>

namespace stromx
{
    namespace runtime
    {
        class ImageWrapper;

        class ImageWrapperTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (ImageWrapperTest);
            CPPUNIT_TEST (testResizeDimension);
            CPPUNIT_TEST (testResizeLength);
            CPPUNIT_TEST_SUITE_END ();

            public:
                ImageWrapperTest() : m_image(0) {}
                
                void setUp();
                void tearDown();

            protected:
                void testResizeDimension();
                void testResizeLength();
                
            private:
                ImageWrapper* m_image;
        };
    }
}

#endif // STROMX_RUNTIME_IMAGEWRAPPERTEST_H
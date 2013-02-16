/* 
*  Copyright 2012 Matthias Fuchs
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

#ifndef STROMX_EXAMPLE_IMAGEFILTERTEST_H
#define STROMX_EXAMPLE_IMAGEFILTERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "stromx/example/ImageFilter.h"

namespace stromx
{
    namespace runtime
    {
        class OperatorTester;
    }

    namespace example
    {
        class ImageFilterTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (ImageFilterTest);
            CPPUNIT_TEST (testOneInput);
            CPPUNIT_TEST (testTwoInputs);
            CPPUNIT_TEST (testIdenticalInputs);
            CPPUNIT_TEST (testWrongDestination);
            CPPUNIT_TEST (testWrongSource);
            CPPUNIT_TEST_SUITE_END ();

            public:
                ImageFilterTest() : m_operator(0) {}
                
                void setUp();
                void tearDown();

            protected:
                void testOneInput();
                void testTwoInputs();
                void testWrongSource();
                void testIdenticalInputs();
                void testWrongDestination();
                
            private:
                runtime::OperatorTester* m_operator;
        };
    }
}

#endif // STROMX_EXAMPLE_IMAGEFILTERTEST_H
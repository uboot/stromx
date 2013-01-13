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

#ifndef STROMX_EXAMPLE_EXAMPLETEST_H
#define STROMX_EXAMPLE_EXAMPLETEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <stromx/runtime/DataVariant.h>

namespace stromx
{
    namespace runtime
    {
        class Factory;
    }

    namespace example
    {
        class Image;

        class ExampleTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (ExampleTest);
            CPPUNIT_TEST (testRegisterExample);
            CPPUNIT_TEST_SUITE_END ();

            public:
                ExampleTest() : m_factory(0) {}
                
                void setUp();
                void tearDown();

            protected:
                void testRegisterExample();
                
            private:
                runtime::Factory* m_factory;
        };
    }
}

#endif // STROMX_EXAMPLE_EXAMPLETEST_H
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

#ifndef STROMX_RUNTIME_DATACONTAINERTEST_H
#define STROMX_RUNTIME_DATACONTAINERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

namespace stromx
{
    namespace runtime
    {
        class DataContainer;
        
        class DataContainerTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (DataContainerTest);
            CPPUNIT_TEST (testDestroy);
            CPPUNIT_TEST (testDestroyDelayed);
            CPPUNIT_TEST (testComparison);
            CPPUNIT_TEST (testEmpty);
            CPPUNIT_TEST_SUITE_END ();

            public:
                DataContainerTest() {}
                
                void setUp() {}
                void tearDown() {}

            protected:
                void testDestroy();
                void testDestroyDelayed();
                void testComparison();
                void testEmpty();
            private:
                void destroyDelayed(DataContainer & container);
        };
    }
}

#endif // STROMX_RUNTIME_DATACONTAINERTEST_H
/* 
 *  Copyright 2015 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_TRIBOOLTEST_H
#define STROMX_RUNTIME_TRIBOOLTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "stromx/runtime/Tribool.h"

namespace stromx
{
    namespace runtime
    {
        class TriboolTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (TriboolTest);
            CPPUNIT_TEST(testSerializeUndefined);
            CPPUNIT_TEST(testDeserializeUndefined);
            CPPUNIT_TEST(testSerializeTrue);
            CPPUNIT_TEST(testDeserializeTrue);
            CPPUNIT_TEST(testSerializeFalse);
            CPPUNIT_TEST(testDeserializeFalse);
            CPPUNIT_TEST_SUITE_END ();

        public:
            TriboolTest() {}
            
            void setUp() {}
            void tearDown() {}

        protected:
            void testSerializeUndefined();
            void testDeserializeUndefined();
            void testSerializeTrue();
            void testDeserializeTrue();
            void testSerializeFalse();
            void testDeserializeFalse();
                
        private:             
            const static Version VERSION;
            
            Tribool m_value;
        };
    }
}

#endif // STROMX_RUNTIME_TRIBOOLTEST_H
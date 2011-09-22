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

#ifndef STROM_ID2DATACOMPOSITETEST_H
#define STROM_ID2DATACOMPOSITETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <strom/DataContainer.h>

namespace strom
{
    namespace impl
    {
        class Id2DataMap;
    }
    
    class Id2DataCompositeTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (Id2DataCompositeTest);
        CPPUNIT_TEST (testTrySet);
        CPPUNIT_TEST (testTryGet);
        CPPUNIT_TEST (testSet);
        CPPUNIT_TEST (testGet);
        CPPUNIT_TEST_SUITE_END ();

        public:
            Id2DataCompositeTest() : m_map(0) {}
            void setUp();
            void tearDown();

        protected:
            void testTrySet();
            void testTryGet();
            void testSet();
            void testGet();
            
        private:
            impl::Id2DataMap* m_map;
            DataContainer m_dataContainer; 
    };
}

#endif // STROM_ID2DATACOMPOSITETEST_H
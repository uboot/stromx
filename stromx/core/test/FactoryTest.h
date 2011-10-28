/* 
*  Copyright 2011 Thomas Fidler
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

#ifndef STROMX_CORE_FACTORYTEST_H
#define STROMX_CORE_FACTORYTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace stromx
{
    namespace core
    {
        class Factory;
        
        class FactoryTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (FactoryTest);
            CPPUNIT_TEST(testRegisterOperator);
            CPPUNIT_TEST(testNewOperator);
            CPPUNIT_TEST(testRegisterData);
            CPPUNIT_TEST(testNewData);
            CPPUNIT_TEST_SUITE_END ();

        public:
            FactoryTest () {}
            
            void setUp();
            void tearDown();

        protected:
            void testRegisterOperator();
            void testNewOperator();
            void testRegisterData();
            void testNewData();
                
        private:
            Factory* m_factory;
        };
    }
}

#endif // STROMX_CORE_FACTORYTEST_H
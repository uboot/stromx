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

#include "stromx/runtime/test/MergeTest.h"

#include <cppunit/TestAssert.h>

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/List.h"
#include "stromx/runtime/Merge.h"
#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/test/TestData.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::MergeTest);

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        void MergeTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new Merge());
            
            m_operator->initialize();
            m_operator->activate();
        }
        
        void MergeTest::testExecute()
        {
            for (std::size_t i = 0; i < 2; ++i)
            {
                Data* data1 = new None();
                Data* data2 = new None();
            
                m_operator->setInputData(Merge::INPUT_NUM_ITEMS,
                                         DataContainer(new UInt64(2)));
                {
                    DataContainer in1(data1);
                    m_operator->setInputData(Merge::INPUT_DATA, in1);
                }
                
                {
                    DataContainer in2(data2);
                    m_operator->setInputData(Merge::INPUT_DATA, in2);
                }
                
                DataContainer out;
                out = m_operator->getOutputData(Merge::OUTPUT);
                m_operator->clearOutputData(Merge::OUTPUT);
                const List & list = ReadAccess<List>(out).get();
                
                CPPUNIT_ASSERT(ReadAccess<>(out)().isVariant(Variant::LIST));
                CPPUNIT_ASSERT_EQUAL(std::size_t(2), list.content().size());
                CPPUNIT_ASSERT_EQUAL((const Data*)(data1), list.content()[0]);
                CPPUNIT_ASSERT_EQUAL((const Data*)(data2), list.content()[1]);
            }
        }
        
        void MergeTest::testExecuteEmptyList()
        {
            {
                m_operator->setInputData(Merge::INPUT_NUM_ITEMS,
                                         DataContainer(new UInt64(0)));
                                         
                DataContainer out = m_operator->getOutputData(Merge::OUTPUT);
                m_operator->clearOutputData(Merge::OUTPUT);
                const List & list = ReadAccess<List>(out).get();
                
                CPPUNIT_ASSERT_EQUAL(std::size_t(0), list.content().size());
            }
            
            {
                m_operator->setInputData(Merge::INPUT_NUM_ITEMS,
                                         DataContainer(new UInt64(1)));
                {
                    DataContainer in(new None());
                    m_operator->setInputData(Merge::INPUT_DATA, in);
                }
                
                DataContainer out = m_operator->getOutputData(Merge::OUTPUT);
                m_operator->clearOutputData(Merge::OUTPUT);
                const List & list = ReadAccess<List>(out).get();
                
                CPPUNIT_ASSERT_EQUAL(std::size_t(1), list.content().size());
            }
        }
        
        void MergeTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}

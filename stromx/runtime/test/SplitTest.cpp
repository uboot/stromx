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

#include "stromx/runtime/test/SplitTest.h"

#include <cppunit/TestAssert.h>

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Split.h"
#include "stromx/runtime/List.h"
#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/test/TestData.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::SplitTest);

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        void SplitTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new Split());
            
            m_operator->initialize();
            m_operator->activate();
        }
        
        void SplitTest::testExecute()
        {
            for (std::size_t i = 0; i < 2; ++i)
            {
                Data* data1 = new None();
                Data* data2 = new None();
                {
                    List* list = new List();
                    list->content().push_back(data1);
                    list->content().push_back(data2);
                    DataContainer in(list);
                    
                    m_operator->setInputData(Split::INPUT, in);
                }
                
                DataContainer outData, outNumItems;
                outData = m_operator->getOutputData(Split::OUTPUT_DATA);
                outNumItems = m_operator->getOutputData(Split::OUTPUT_NUM_ITEMS);
                m_operator->clearOutputData(Split::OUTPUT_DATA);
                m_operator->clearOutputData(Split::OUTPUT_NUM_ITEMS);
                
                CPPUNIT_ASSERT_EQUAL((const Data*)(data1), &ReadAccess(outData).get());
                CPPUNIT_ASSERT_EQUAL(UInt64(2), ReadAccess(outNumItems).get<UInt64>());
                
                outData = m_operator->getOutputData(Split::OUTPUT_DATA);
                m_operator->clearOutputData(Split::OUTPUT_DATA);
                
                CPPUNIT_ASSERT_EQUAL((const Data*)(data2), &ReadAccess(outData).get());
            }
        }
        
        void SplitTest::testExecuteEmptyList()
        {
            {
                List* list = new List();
                DataContainer in(list);
                
                m_operator->setInputData(Split::INPUT, in);
            }
            
            DataContainer outNumItems;
            outNumItems = m_operator->getOutputData(Split::OUTPUT_NUM_ITEMS);
            m_operator->clearOutputData(Split::OUTPUT_NUM_ITEMS);
            
            CPPUNIT_ASSERT_EQUAL(UInt64(0), ReadAccess(outNumItems).get<UInt64>());
            
            {
                List* list = new List();
                list->content().push_back(new None());
                DataContainer in(list);
                
                m_operator->setInputData(Split::INPUT, in);
            }
            
            DataContainer outData;
            outData = m_operator->getOutputData(Split::OUTPUT_DATA);
            outNumItems = m_operator->getOutputData(Split::OUTPUT_NUM_ITEMS);
            m_operator->clearOutputData(Split::OUTPUT_DATA);
            m_operator->clearOutputData(Split::OUTPUT_NUM_ITEMS);
            
            CPPUNIT_ASSERT_EQUAL(UInt64(1), ReadAccess(outNumItems).get<UInt64>());
        }
        
        void SplitTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}

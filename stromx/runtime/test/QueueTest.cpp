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

#include <cppunit/TestAssert.h>
#include "QueueTest.h"
#include "../Queue.h"
#include "../DataContainer.h"
#include "../OperatorTester.h"
#include "../Primitive.h"
#include "../ReadAccess.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::QueueTest);

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        void QueueTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new Queue());
            m_operator->initialize();
            m_operator->setParameter(Queue::SIZE, UInt32(3));
            m_operator->activate();
        }
        
        void QueueTest::testExecute()
        {
            DataContainer input1(new UInt8);
            DataContainer input2(new UInt8);
            DataContainer input3(new UInt8);
            
            m_operator->setInputData(Queue::INPUT, input1);
            m_operator->setInputData(Queue::INPUT, input2);
            m_operator->setInputData(Queue::INPUT, input3);
            
            DataContainer output1 = m_operator->getOutputData(Queue::OUTPUT);
            DataContainer output2 = m_operator->getOutputData(Queue::OUTPUT);
            DataContainer output3 = m_operator->getOutputData(Queue::OUTPUT);
            
            CPPUNIT_ASSERT_EQUAL(&ReadAccess<>(input1)(), &ReadAccess<>(output1)());
        }    

        void QueueTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}

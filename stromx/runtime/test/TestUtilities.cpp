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

#include "stromx/runtime/Operator.h"
#include "stromx/runtime/Stream.h"
#include "stromx/runtime/Thread.h"
#include "stromx/runtime/test/TestOperator.h"
#include "stromx/runtime/test/TestUtilities.h"

namespace stromx
{
    namespace runtime
    {
        stromx::runtime::Stream* stromx::runtime::TestUtilities::buildTestStream()
        {
            Stream* stream = new Stream;
            stream->setName("TestStream");
            
            Operator* op0 = new Operator(new TestOperator);
            op0->setName("Number 1");
            stream->addOperator(op0);
            op0->setParameter(TestOperator::BUFFER_SIZE, UInt32(5000));
            stream->initializeOperator(op0);
            op0->setParameter(TestOperator::SLEEP_TIME, UInt32(200));
            op0->setParameter(TestOperator::TEST_DATA, TestData(1));
            
            Operator* op1 = new Operator(new TestOperator);
            op1->setName("Number 2");
            stream->addOperator(op1);
            op1->setParameter(TestOperator::BUFFER_SIZE, UInt32(6000));
            stream->initializeOperator(op1);
            op1->setParameter(TestOperator::SLEEP_TIME, UInt32(250));
            op1->setParameter(TestOperator::TEST_DATA, TestData(2));
            
            Operator* op2 = new Operator(new TestOperator);
            op2->setName("Number 3");
            stream->addOperator(op2);
            op2->setParameter(TestOperator::BUFFER_SIZE, UInt32(7000));
            stream->initializeOperator(op2);
            op2->setParameter(TestOperator::SLEEP_TIME, UInt32(300));
            op2->setParameter(TestOperator::TEST_DATA, TestData(3));
            
            stream->connect(op0, TestOperator::OUTPUT_1, op1, TestOperator::INPUT_1);
            stream->connect(op0, TestOperator::OUTPUT_2, op1, TestOperator::INPUT_2);
            
            stream->connect(op1, TestOperator::OUTPUT_1, op2, TestOperator::INPUT_1);
            stream->connect(op1, TestOperator::OUTPUT_2, op2, TestOperator::INPUT_2);
            
            Thread* thread = stream->addThread();
            thread->setName("Processing thread");
            
            thread->addInput(op1, TestOperator::INPUT_1);
            thread->addInput(op1, TestOperator::INPUT_2);
            thread->addInput(op2, TestOperator::INPUT_1);
            thread->addInput(op2, TestOperator::INPUT_2);
            
            return stream;
        }
    }
}
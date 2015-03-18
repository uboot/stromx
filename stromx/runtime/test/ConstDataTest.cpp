/* 
*  Copyright 2014 Matthias Fuchs
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
#include "stromx/runtime/test/ConstDataTest.h"
#include "stromx/runtime/ConstData.h"
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/ReadAccess.h"
#include <stromx/runtime/String.h>
#include <stromx/runtime/TriggerData.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::ConstDataTest);

std::ostream& operator<<(std::ostream& out, const stromx::runtime::TriggerData&)
{
    return out;
}

bool operator==(const stromx::runtime::TriggerData&, const stromx::runtime::TriggerData&)
{
    return true;
}

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {   
        namespace
        {
            template <class data_t>
            void runTest(OperatorTester* op, const data_t value)
            {
                Enum dataType(value.variant().id());
                op->setParameter(ConstData::DATA_TYPE, dataType);
                op->initialize();
                
                DataRef paramValue = op->getParameter(ConstData::VALUE);
                CPPUNIT_ASSERT_NO_THROW(data_cast<data_t>(paramValue));
                
                op->setParameter(ConstData::VALUE, value);
                op->activate();
                
                DataContainer data = op->getOutputData(ConstData::OUTPUT);
                CPPUNIT_ASSERT_EQUAL(value, ReadAccess<data_t>(data)());
            }
            
        }
        void ConstDataTest::setUp()
        {
            m_operator = new OperatorTester(new ConstData());
        }

        void ConstDataTest::testExecuteBool()
        {
            runTest(m_operator, Bool(true));
        }

        void ConstDataTest::testExecuteInt32()
        {
            runTest(m_operator, Int32(-17));
        }

        void ConstDataTest::testExecuteUInt32()
        {
            runTest(m_operator, Int32(17));
        }

        void ConstDataTest::testExecuteFloat32()
        {
            runTest(m_operator, Float32(1.4));
        }

        void ConstDataTest::testExecuteString()
        {
            runTest(m_operator, String("Hello"));
        }

        void ConstDataTest::testExecuteTrigger()
        {
            runTest(m_operator, TriggerData());
        }

        void ConstDataTest::testNoAllocation()
        {
            m_operator->setParameter(ConstData::DATA_TYPE, Enum(Variant::INT_32.id()));
            m_operator->setParameter(ConstData::ALLOCATE_DATA, Bool(false));
            m_operator->initialize();
            
            m_operator->setParameter(ConstData::VALUE, Int32(15));
            m_operator->activate();
            
            const Data* dataPtr = 0;
            {
                DataContainer data;
                data = m_operator->getOutputData(ConstData::OUTPUT);
                dataPtr = &ReadAccess<>(data)();
            }
            
            DataContainer data;
            m_operator->clearOutputData(ConstData::OUTPUT);
            data = m_operator->getOutputData(ConstData::OUTPUT);
            CPPUNIT_ASSERT_EQUAL(dataPtr, &ReadAccess<>(data)());
        }

        void ConstDataTest::tearDown()
        {
            delete m_operator;
        }
    }
}

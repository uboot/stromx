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

#include "stromx/runtime/test/PushTest.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <cppunit/TestAssert.h>
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/Push.h"
#include "stromx/runtime/TriggerData.h"
#include "stromx/runtime/Variant.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::PushTest);

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        void PushTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new Push());
            m_operator->setParameter(Push::DATA_TYPE, Enum(Variant::BOOL.id()));
            m_operator->initialize();
            m_operator->activate();
        }
        
        void PushTest::testExecute()
        {
            boost::thread t(boost::bind(&PushTest::pushDataDelayed, this));
            DataContainer result = m_operator->getOutputData(Push::OUTPUT);
            
            ReadAccess access(result);
            CPPUNIT_ASSERT_EQUAL(true, bool(access.get<Bool>()));
            
            t.join();
        }
        
        void PushTest::testInterrupt()
        {
            boost::thread t(boost::bind(&PushTest::getOutputData, this));
            t.interrupt();
            t.join();
        }

        void PushTest::testGetParameter()
        {
            m_operator->setParameter(Push::VALUE, runtime::Bool(false));
            DataRef value = m_operator->getParameter(Push::VALUE);
            
            CPPUNIT_ASSERT_EQUAL(false, bool(data_cast<Bool>(value)));
        }

        void PushTest::testGetParameterNoValue()
        {
            CPPUNIT_ASSERT_THROW(m_operator->getParameter(Push::VALUE),
                                 OperatorError);
        }
        
        void PushTest::testParameter()
        {
            const Parameter & param = m_operator->info().parameter(Push::VALUE);
            CPPUNIT_ASSERT_EQUAL(Parameter::PUSH, param.updateBehavior());
        }
        
        void PushTest::tearDown ( void )
        {
            delete m_operator;
        }    
        
        void PushTest::pushDataDelayed()
        {
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
            m_operator->setParameter(Push::VALUE, runtime::Bool(true));
        }
        
        void PushTest::getOutputData()
        {
            CPPUNIT_ASSERT_THROW(m_operator->getOutputData(Push::OUTPUT), Interrupt);
        }
    }
}

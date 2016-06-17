/* 
*  Copyright 2012 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_SENDRECEIVETEST_H
#define STROMX_RUNTIME_SENDRECEIVETEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

namespace stromx
{
    namespace runtime
    {
        class Factory;
        class OperatorTester;
        
        class SendReceiveTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (SendReceiveTest);
            CPPUNIT_TEST (testSendUInt32);
            CPPUNIT_TEST (testInterrupt);
            CPPUNIT_TEST_SUITE_END ();

        public:
                SendReceiveTest() : m_send(0), m_receive(0), m_factory(0) {}
                
                void setUp();
                void tearDown();

            protected:
                void testSendUInt32();
                void testInterrupt();
                
            private:
                runtime::OperatorTester* m_send;
                runtime::OperatorTester* m_receive;
                runtime::Factory* m_factory;
        };
    }
}

#endif // STROMX_RUNTIME_SENDRECEIVETEST_H
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

#ifndef STROMX_RUNTIME_INPUTNODETEST_H
#define STROMX_RUNTIME_INPUTNODETEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "stromx/runtime/DataContainer.h"

namespace stromx
{
    namespace runtime
    {
        class Operator;
        
        namespace impl
        {
            class OutputNode;
            class InputNode;
        }
        
        class InputNodeTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (InputNodeTest);
            CPPUNIT_TEST(testSource);
            CPPUNIT_TEST(testConnect);
            CPPUNIT_TEST(testDisconnect);
            CPPUNIT_TEST(testSetInputData);
            CPPUNIT_TEST_SUITE_END ();

        public:
            InputNodeTest() 
              : m_operatorWrapper(0),
                m_sourceOperatorWrapper(0),
                m_inputNode(0),
                m_sourceNode(0)
            {}
            
            void setUp();
            void tearDown();

        protected:
            void testSource();
            void testConnect();
            void testDisconnect();
            void testSetInputData();
                
        private: 
            Operator* m_operatorWrapper;
            Operator* m_sourceOperatorWrapper;
            DataContainer m_container;
            impl::InputNode* m_inputNode;
            impl::OutputNode* m_sourceNode;
            
        };
    }
}

#endif // STROMX_RUNTIME_INPUTNODETEST_H
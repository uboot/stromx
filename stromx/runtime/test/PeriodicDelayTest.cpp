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

#include <boost/thread.hpp>
#include <cppunit/TestAssert.h>
#include "PeriodicDelayTest.h"
#include "../PeriodicDelay.h"
#include "../Exception.h"
#include "../OperatorTester.h"
#include "../ReadAccess.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::PeriodicDelayTest);

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        void PeriodicDelayTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new PeriodicDelay());
            m_operator->initialize();
            m_operator->activate();
            m_data = DataContainer(new UInt32(4));
            m_operator->setInputData(PeriodicDelay::INPUT, m_data);
        }
        
        void PeriodicDelayTest::testExecute()
        {
            m_operator->setParameter(PeriodicDelay::PERIOD, UInt32(1000));
            
            {
                DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
                ReadAccess<UInt32> access(result);
                access();
            }
            
            {
                m_operator->clearOutputData(PeriodicDelay::OUTPUT);
                m_operator->setInputData(PeriodicDelay::INPUT, m_data);
                DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
            }
            
            {
                m_operator->clearOutputData(PeriodicDelay::OUTPUT);
                m_operator->setInputData(PeriodicDelay::INPUT, m_data);
                DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
            }
            
            {
                m_operator->clearOutputData(PeriodicDelay::OUTPUT);
                m_operator->setInputData(PeriodicDelay::INPUT, m_data);
                DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
            }

            m_operator->clearOutputData(PeriodicDelay::OUTPUT);
            m_operator->setInputData(PeriodicDelay::INPUT, m_data);
            boost::thread t1(boost::bind(&PeriodicDelayTest::getOutputDataInterrupted, this));
            t1.interrupt();
            t1.join();
        }
        
        void PeriodicDelayTest::testExecuteZeroPeriod()
        {
            m_operator->setParameter(PeriodicDelay::PERIOD, runtime::UInt32(0));
            
            {
                DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
                ReadAccess<UInt32> access(result);
                access();
            }
            
            {
                m_operator->clearOutputData(PeriodicDelay::OUTPUT);
                m_operator->setInputData(PeriodicDelay::INPUT, m_data);
                DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
            }
            
            {
                m_operator->clearOutputData(PeriodicDelay::OUTPUT);
                m_operator->setInputData(PeriodicDelay::INPUT, m_data);
                DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
            }   
            
            {
                m_operator->clearOutputData(PeriodicDelay::OUTPUT);
                m_operator->setInputData(PeriodicDelay::INPUT, m_data);
                DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
            }
        }
        
        void PeriodicDelayTest::getOutputDataInterrupted()
        {
            CPPUNIT_ASSERT_THROW(m_operator->getOutputData(PeriodicDelay::OUTPUT), Interrupt);
        }
        
        void PeriodicDelayTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}

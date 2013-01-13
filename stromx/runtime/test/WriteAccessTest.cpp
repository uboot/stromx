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

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <cppunit/TestAssert.h>
#include "TestData.h"
#include "WriteAccessTest.h"
#include "../DataContainer.h"
#include "../Exception.h"
#include "../None.h"


CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::WriteAccessTest);

namespace stromx
{
    namespace runtime
    {
        void WriteAccessTest::testWriteAccess()
        {
            Data* data = new TestData;
            {
                DataContainer container(data);
                WriteAccess<> access(container);
                CPPUNIT_ASSERT_EQUAL(data, &access());
            }
            
            CPPUNIT_ASSERT(TestData::wasDestructed);  
        }
        
        void WriteAccessTest::testEmptyWriteAccess()
        {
            WriteAccess<> access;
            CPPUNIT_ASSERT(access.empty());
            CPPUNIT_ASSERT_THROW(access.get(), AccessEmpty);
        }
        
        void WriteAccessTest::testWriteAccessCast()
        {
            Data* data = new TestData;
            
            DataContainer container(data);
            WriteAccess<TestData> access(container);
            CPPUNIT_ASSERT_NO_THROW(access());
        }
        
        
        void WriteAccessTest::testWriteAccessWrongCast()
        {
            Data* data = new None;
            
            DataContainer container(data);
            WriteAccess<TestData> access(container);
            CPPUNIT_ASSERT_THROW(access(), BadCast);
        }
        
        void WriteAccessTest::testWriteAccessEmptyContainer()
        {
            DataContainer container;
            CPPUNIT_ASSERT_THROW(WriteAccess<> access(container), WrongArgument);
        }
        
        void WriteAccessTest::testReleaseWriteAccess()
        {
            Data* data = new TestData;
            DataContainer container(data);
            {
                WriteAccess<> access(container);
            }
            
            WriteAccess<> access(container);
            CPPUNIT_ASSERT_EQUAL(data, &access());
        }
        
        void WriteAccessTest::testWriteAccessDelayed()
        {
            m_data = new TestData;
            DataContainer container(m_data);
            
            {
                boost::thread t(boost::bind(&WriteAccessTest::releaseDelayed, this, _1), WriteAccess<>(container));
            }
            
            WriteAccess<> access(container);
            CPPUNIT_ASSERT_EQUAL(m_data, &access());
        }
        
        void WriteAccessTest::releaseDelayed(WriteAccess<>& access)
        {
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            CPPUNIT_ASSERT_EQUAL(m_data, &access());
        }
            
        void WriteAccessTest::writeAccessInterrupt(DataContainer& container)
        {
            CPPUNIT_ASSERT_THROW(WriteAccess<> access(container), Interrupt);
        }
        
        void WriteAccessTest::testWriteAccessInterrupt()
        {
            {
                DataContainer container = DataContainer(new TestData());
                WriteAccess<> access(container);
                boost::thread t(boost::bind(&WriteAccessTest::writeAccessInterrupt, this, _1), container);
                
                t.interrupt();
                boost::this_thread::sleep(boost::posix_time::seconds(1));
                t.join();
            }
            
            CPPUNIT_ASSERT(TestData::wasDestructed);
        }
        
        void WriteAccessTest::testWriteAccessTimeout()
        {
            m_data = new TestData;
            DataContainer container(m_data);
            
            {
                CPPUNIT_ASSERT_NO_THROW(WriteAccess<>(container, 100));
            }
            
            WriteAccess<> write(container);
            
            CPPUNIT_ASSERT_THROW(WriteAccess<>(container, 100), Timeout);
        }
    }
}


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
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/None.h"
#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/test/ReadAccessTest.h"
#include "stromx/runtime/test/TestData.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::ReadAccessTest);

namespace stromx
{
    namespace runtime
    {
        void ReadAccessTest::testReadAccessEmptyContainer()
        {
            DataContainer container;
            CPPUNIT_ASSERT_THROW(ReadAccess<> access(container), WrongArgument);
        }

        void ReadAccessTest::testReadAccess()
        {
            Data* data = new TestData;
            {
                DataContainer container(data);
                ReadAccess<> access1(container);
                CPPUNIT_ASSERT_EQUAL(static_cast<const Data*>(data), &access1());
                
                ReadAccess<> access2(container);
                CPPUNIT_ASSERT_EQUAL(static_cast<const Data*>(data), &access2());
            }
            
            CPPUNIT_ASSERT(TestData::wasDestructed);  
        }
        
        void ReadAccessTest::testEmptyReadAccess()
        {
            ReadAccess<> access;
            CPPUNIT_ASSERT(access.empty());
            CPPUNIT_ASSERT_THROW(access.get(), AccessEmpty);
        }

        void ReadAccessTest::testReadAccessCast()
        {
            Data* data = new TestData;
            DataContainer container(data);
            ReadAccess<TestData> access1(container);
            
            CPPUNIT_ASSERT_NO_THROW(access1());
        }

        void ReadAccessTest::testReadAccessWrongCast()
        {
            Data* data = new None;
            DataContainer container(data);
            ReadAccess<TestData> access1(container);
            
            CPPUNIT_ASSERT_THROW(access1(), BadCast);
        }
        
        void ReadAccessTest::testDestroyReadAccess()
        {
            Data* data = new TestData;
            DataContainer container(data);
            {
                ReadAccess<> access(container);
                
                // the read access is destroyed before leaving the scope
            }
            
            WriteAccess<> access(container);
            CPPUNIT_ASSERT_EQUAL(data, &access());
        }
        
        void ReadAccessTest::testRelease()
        {
            Data* data = new TestData;
            DataContainer container(data);
            
            ReadAccess<> access(container);
            access.release();
            
            CPPUNIT_ASSERT(access.empty());
        }
        
        void ReadAccessTest::testReadAccessDelayed()
        {
            m_data = new TestData;
            DataContainer container(m_data);
            
            {
                boost::thread t(boost::bind(&ReadAccessTest::releaseDelayed, this, _1), WriteAccess<>(container));
            }
            
            ReadAccess<> access(container);
            CPPUNIT_ASSERT_EQUAL((const Data*)(m_data), &access());
        }
        
        void ReadAccessTest::releaseDelayed(WriteAccess<>& access)
        {
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            CPPUNIT_ASSERT_EQUAL(m_data, &access());
        }
            
        void ReadAccessTest::readAccessInterrupt(DataContainer& container)
        {
            CPPUNIT_ASSERT_THROW(ReadAccess<> access(container), Interrupt);
        }
        
        void ReadAccessTest::testReadAccessTimeout()
        {
            m_data = new TestData;
            DataContainer container(m_data);
            
            {
                CPPUNIT_ASSERT_NO_THROW(ReadAccess<>(container, 100));
            }
            
            WriteAccess<> write(container);
            
            CPPUNIT_ASSERT_THROW(ReadAccess<>(container, 100), Timeout);
        }
        
        void ReadAccessTest::testReadAccessInterrupt()
        {
            {
                DataContainer container = DataContainer(new TestData());
                WriteAccess<> access(container);
                boost::thread t(boost::bind(&ReadAccessTest::readAccessInterrupt, this, _1), container);
                
                t.interrupt();
                boost::this_thread::sleep(boost::posix_time::seconds(1));
                t.join();
            }
            
            CPPUNIT_ASSERT(TestData::wasDestructed);
        }
    }
}

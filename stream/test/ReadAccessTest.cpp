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

#include "ReadAccessTest.h"

#include "TestData.h"

#include <stream/DataContainer.h>
#include <stream/ReadAccess.h>
#include <stream/WriteAccess.h>
#include <stream/Exception.h>

#include <cppunit/TestAssert.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::ReadAccessTest);

namespace stream
{
    void ReadAccessTest::testReadAccessEmpty()
    {
        DataContainer container;
        CPPUNIT_ASSERT_THROW(ReadAccess access(container), WrongArgument);
    }

    void ReadAccessTest::testReadAccess()
    {
        Data* data = new TestData;
        {
            DataContainer container(data);
            ReadAccess access1(container);
            CPPUNIT_ASSERT_EQUAL(static_cast<const Data*>(data), &access1());
            
            ReadAccess access2(container);
            CPPUNIT_ASSERT_EQUAL(static_cast<const Data*>(data), &access2());
        }
        
        CPPUNIT_ASSERT(TestData::wasDestructed);  
    }
    
    void ReadAccessTest::testReleaseReadAccess()
    {
        Data* data = new TestData;
        DataContainer container(data);
        {
            ReadAccess access(container);
        }
        
        WriteAccess access(container);
        CPPUNIT_ASSERT_EQUAL(data, &access());
    }
    
    void ReadAccessTest::testReadAccessDelayed()
    {
        m_data = new TestData;
        DataContainer container(m_data);
        
        {
            boost::thread t(boost::bind(&ReadAccessTest::releaseDelayed, this, _1), WriteAccess(container));
        }
        
        ReadAccess access(container);
        CPPUNIT_ASSERT_EQUAL((const Data*)(m_data), &access());
    }
    
    void ReadAccessTest::releaseDelayed(WriteAccess& access)
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        CPPUNIT_ASSERT_EQUAL(m_data, &access());
    }
        
    void ReadAccessTest::readAccessInterrupt(DataContainer& container)
    {
        CPPUNIT_ASSERT_THROW(ReadAccess access(container), Interrupt);
    }
    
    void ReadAccessTest::testReadAccessInterrupt()
    {
        {
            DataContainer container = DataContainer(new TestData());
            WriteAccess access(container);
            boost::thread t(boost::bind(&ReadAccessTest::readAccessInterrupt, this, _1), container);
            
            t.interrupt();
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            t.join();
        }
        
        CPPUNIT_ASSERT(TestData::wasDestructed);
    }
}

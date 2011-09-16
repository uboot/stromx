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

#ifndef STREAM_READACCESSTEST_H
#define STREAM_READACCESSTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <strom/WriteAccess.h>

namespace strom
{
    class DataContainer;
    class Data;
    
    class ReadAccessTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (ReadAccessTest);
        CPPUNIT_TEST(testReadAccessEmpty);
        CPPUNIT_TEST(testReadAccess);
        CPPUNIT_TEST(testReadAccessCast);
        CPPUNIT_TEST(testReadAccessWrongCast);
        CPPUNIT_TEST(testReadAccessDelayed);
        CPPUNIT_TEST(testReadAccessInterrupt);
        CPPUNIT_TEST(testReleaseReadAccess);
        CPPUNIT_TEST_SUITE_END ();

    public:
        ReadAccessTest () {}
        
        void setUp() {}
        void tearDown() {}

    protected:
        void testReadAccessEmpty();
        void testReadAccess();
        void testReadAccessCast();
        void testReadAccessWrongCast();
        void testReadAccessDelayed();
        void testReadAccessInterrupt();
        void testReleaseReadAccess();

    private:
        void releaseDelayed(WriteAccess<>& access);
        void readAccessInterrupt(DataContainer & container);
        
        Data* m_data;
    };
}

#endif // STREAM_READACCESSTEST_H
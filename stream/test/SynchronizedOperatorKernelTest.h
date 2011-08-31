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

#ifndef STREAM_SYNCHRONIZEDOPERATORKERNELTEST_H
#define STREAM_SYNCHRONIZEDOPERATORKERNELTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stream/DataContainer.h>

namespace stream
{
    namespace impl
    {
        class SynchronizedOperatorKernel;
    }
    class TestOperator;

    class SynchronizedOperatorKernelTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (SynchronizedOperatorKernelTest);
        CPPUNIT_TEST (testSetInputData);
        CPPUNIT_TEST (testGetOutputData);
        CPPUNIT_TEST (testClearOutputData);
        CPPUNIT_TEST (testInitialize);
        CPPUNIT_TEST (testActivate);
        CPPUNIT_TEST (testDeactivate);
        CPPUNIT_TEST (testGetParameter);
        CPPUNIT_TEST (testSetParameter);
        CPPUNIT_TEST (testGetParameterStatusNone);
        CPPUNIT_TEST (testSetParameterStatusNone);
        CPPUNIT_TEST_SUITE_END ();

    public:
        SynchronizedOperatorKernelTest() : m_operatorWrapper(0), m_testOperator(0) {}
        
        void setUp();
        void tearDown();

    protected:
        void testSetInputData();
        void testGetOutputData();
        void testClearOutputData();
        void testInitialize();
        void testActivate();
        void testDeactivate();
        void testGetParameter();
        void testSetParameter();
        void testGetParameterStatusNone();
        void testSetParameterStatusNone();
            
    private:
        void setInputDataDelayed(const unsigned int id); 
        void clearOutputDataDelayed(const unsigned int id); 
        void getOutputDataWithInterrupt(const unsigned id);
        void setInputDataWithInterrupt(const unsigned int id);
        
        impl::SynchronizedOperatorKernel* m_operatorWrapper;
        TestOperator* m_testOperator;
        DataContainer m_container;
    };
}

#endif // STREAM_SYNCHRONIZEDOPERATORKERNELTEST_H
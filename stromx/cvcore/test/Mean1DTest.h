#ifndef STROMX_CVCORE_MEAN1DTEST_H
#define STROMX_CVCORE_MEAN1DTEST_H

#include "stromx/cvcore/Config.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/OperatorTester.h"

namespace stromx
{
    namespace cvcore
    {
        class Mean1DTest : public CPPUNIT_NS::TestFixture
        {
            CPPUNIT_TEST_SUITE(Mean1DTest);
            CPPUNIT_TEST(testAllocate0);
            CPPUNIT_TEST(testAllocate1);
            CPPUNIT_TEST_SUITE_END();
            
        public:
            Mean1DTest() : m_operator(0) {}
            void setUp();
            void tearDown();
            
        protected:
            void testAllocate0();
            void testAllocate1();
            
        private:
            runtime::OperatorTester* m_operator;
        };
    } // cvcore
} // stromx

#endif // STROMX_CVCORE_MEAN1DTEST_H

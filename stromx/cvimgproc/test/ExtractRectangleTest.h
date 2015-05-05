#ifndef STROMX_CVIMGPROC_EXTRACTRECTANGLETEST_H
#define STROMX_CVIMGPROC_EXTRACTRECTANGLETEST_H

#include "stromx/cvimgproc/Config.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/OperatorTester.h"

namespace stromx
{
    namespace cvimgproc
    {
        class ExtractRectangleTest : public CPPUNIT_NS::TestFixture
        {
            CPPUNIT_TEST_SUITE(ExtractRectangleTest);
            CPPUNIT_TEST(testAllocate0);
            CPPUNIT_TEST(testAllocate1);
            CPPUNIT_TEST(testAllocate2);
            CPPUNIT_TEST_SUITE_END();
            
        public:
            ExtractRectangleTest() : m_operator(0) {}
            void setUp();
            void tearDown();
            
        protected:
            void testAllocate0();
            void testAllocate1();
            void testAllocate2();
            
        private:
            runtime::OperatorTester* m_operator;
        };
    } // cvimgproc
} // stromx

#endif // STROMX_CVIMGPROC_EXTRACTRECTANGLETEST_H

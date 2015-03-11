#ifndef STROMX_CVIMGPROC_DISTANCETRANSFORMTEST_H
#define STROMX_CVIMGPROC_DISTANCETRANSFORMTEST_H

#include "stromx/cvimgproc/Config.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/OperatorTester.h"

namespace stromx
{
    namespace cvimgproc
    {
        class DistanceTransformTest : public CPPUNIT_NS::TestFixture
        {
            CPPUNIT_TEST_SUITE(DistanceTransformTest);
            CPPUNIT_TEST(testManual0);
            CPPUNIT_TEST(testAllocate0);
            CPPUNIT_TEST(testAllocate1);
            CPPUNIT_TEST(testAllocate2);
            CPPUNIT_TEST_SUITE_END();
            
        public:
            DistanceTransformTest() : m_operator(0) {}
            void setUp();
            void tearDown();
            
        protected:
            void testManual0();
            void testAllocate0();
            void testAllocate1();
            void testAllocate2();
            
        private:
            runtime::OperatorTester* m_operator;
        };
    } // cvimgproc
} // stromx

#endif // STROMX_CVIMGPROC_DISTANCETRANSFORMTEST_H

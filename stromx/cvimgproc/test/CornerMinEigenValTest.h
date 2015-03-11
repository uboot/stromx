#ifndef STROMX_CVIMGPROC_CORNERMINEIGENVALTEST_H
#define STROMX_CVIMGPROC_CORNERMINEIGENVALTEST_H

#include "stromx/cvimgproc/Config.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/OperatorTester.h"

namespace stromx
{
    namespace cvimgproc
    {
        class CornerMinEigenValTest : public CPPUNIT_NS::TestFixture
        {
            CPPUNIT_TEST_SUITE(CornerMinEigenValTest);
            CPPUNIT_TEST(testManual0);
            CPPUNIT_TEST(testAllocate0);
            CPPUNIT_TEST_SUITE_END();
            
        public:
            CornerMinEigenValTest() : m_operator(0) {}
            void setUp();
            void tearDown();
            
        protected:
            void testManual0();
            void testAllocate0();
            
        private:
            runtime::OperatorTester* m_operator;
        };
    } // cvimgproc
} // stromx

#endif // STROMX_CVIMGPROC_CORNERMINEIGENVALTEST_H

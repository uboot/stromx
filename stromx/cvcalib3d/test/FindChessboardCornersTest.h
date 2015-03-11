#ifndef STROMX_CVCALIB3D_FINDCHESSBOARDCORNERSTEST_H
#define STROMX_CVCALIB3D_FINDCHESSBOARDCORNERSTEST_H

#include "stromx/cvcalib3d/Config.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/OperatorTester.h"

namespace stromx
{
    namespace cvcalib3d
    {
        class FindChessboardCornersTest : public CPPUNIT_NS::TestFixture
        {
            CPPUNIT_TEST_SUITE(FindChessboardCornersTest);
            CPPUNIT_TEST(testAllocate0);
            CPPUNIT_TEST(testAllocate1);
            CPPUNIT_TEST_SUITE_END();
            
        public:
            FindChessboardCornersTest() : m_operator(0) {}
            void setUp();
            void tearDown();
            
        protected:
            void testAllocate0();
            void testAllocate1();
            
        private:
            runtime::OperatorTester* m_operator;
        };
    } // cvcalib3d
} // stromx

#endif // STROMX_CVCALIB3D_FINDCHESSBOARDCORNERSTEST_H

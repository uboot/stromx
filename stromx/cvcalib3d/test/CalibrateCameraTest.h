#ifndef STROMX_CVCALIB3D_CALIBRATECAMERATEST_H
#define STROMX_CVCALIB3D_CALIBRATECAMERATEST_H

#include "stromx/cvcalib3d/Config.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/OperatorTester.h"

namespace stromx
{
    namespace cvcalib3d
    {
        class CalibrateCameraTest : public CPPUNIT_NS::TestFixture
        {
            CPPUNIT_TEST_SUITE(CalibrateCameraTest);
            CPPUNIT_TEST(testAllocate0);
            CPPUNIT_TEST_SUITE_END();
            
        public:
            CalibrateCameraTest() : m_operator(0) {}
            void setUp();
            void tearDown();
            
        protected:
            void testAllocate0();
            
        private:
            runtime::OperatorTester* m_operator;
        };
    } // cvcalib3d
} // stromx

#endif // STROMX_CVCALIB3D_CALIBRATECAMERATEST_H

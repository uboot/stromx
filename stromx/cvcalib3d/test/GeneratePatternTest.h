#ifndef STROMX_CVCALIB3D_GENERATEPATTERNTEST_H
#define STROMX_CVCALIB3D_GENERATEPATTERNTEST_H

#include "stromx/cvcalib3d/Config.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/OperatorTester.h"

namespace stromx
{
    namespace cvcalib3d
    {
        class GeneratePatternTest : public CPPUNIT_NS::TestFixture
        {
            CPPUNIT_TEST_SUITE(GeneratePatternTest);
            CPPUNIT_TEST(testAllocate0);
            CPPUNIT_TEST_SUITE_END();
            
        public:
            GeneratePatternTest() : m_operator(0) {}
            void setUp();
            void tearDown();
            
        protected:
            void testAllocate0();
            
        private:
            runtime::OperatorTester* m_operator;
        };
    } // cvcalib3d
} // stromx

#endif // STROMX_CVCALIB3D_GENERATEPATTERNTEST_H

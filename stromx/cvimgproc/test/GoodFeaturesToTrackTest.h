#ifndef STROMX_CVIMGPROC_GOODFEATURESTOTRACKTEST_H
#define STROMX_CVIMGPROC_GOODFEATURESTOTRACKTEST_H

#include "stromx/cvimgproc/Config.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/OperatorTester.h"

namespace stromx
{
    namespace cvimgproc
    {
        class GoodFeaturesToTrackTest : public CPPUNIT_NS::TestFixture
        {
            CPPUNIT_TEST_SUITE(GoodFeaturesToTrackTest);
            CPPUNIT_TEST(testAllocate0);
            CPPUNIT_TEST_SUITE_END();
            
        public:
            GoodFeaturesToTrackTest() : m_operator(0) {}
            void setUp();
            void tearDown();
            
        protected:
            void testAllocate0();
            
        private:
            runtime::OperatorTester* m_operator;
        };
    } // cvimgproc
} // stromx

#endif // STROMX_CVIMGPROC_GOODFEATURESTOTRACKTEST_H

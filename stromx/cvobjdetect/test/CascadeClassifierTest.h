#ifndef STROMX_CVOBJDETECT_CASCADECLASSIFIERTEST_H
#define STROMX_CVOBJDETECT_CASCADECLASSIFIERTEST_H

#include "stromx/cvobjdetect/Config.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/OperatorTester.h"

namespace stromx
{
    namespace cvobjdetect
    {
        class CascadeClassifierTest : public CPPUNIT_NS::TestFixture
        {
            CPPUNIT_TEST_SUITE(CascadeClassifierTest);
            CPPUNIT_TEST(testAllocate0);
            CPPUNIT_TEST(testAllocate1);
            CPPUNIT_TEST_SUITE_END();
            
        public:
            CascadeClassifierTest() : m_operator(0) {}
            void setUp();
            void tearDown();
            
        protected:
            void testAllocate0();
            void testAllocate1();
            
        private:
            runtime::OperatorTester* m_operator;
        };
    } // cvobjdetect
} // stromx

#endif // STROMX_CVOBJDETECT_CASCADECLASSIFIERTEST_H

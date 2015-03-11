#ifndef STROMX_CVIMGPROC_MORPHOLOGYEXTEST_H
#define STROMX_CVIMGPROC_MORPHOLOGYEXTEST_H

#include "stromx/cvimgproc/Config.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/OperatorTester.h"

namespace stromx
{
    namespace cvimgproc
    {
        class MorphologyExTest : public CPPUNIT_NS::TestFixture
        {
            CPPUNIT_TEST_SUITE(MorphologyExTest);
            CPPUNIT_TEST(testManual0);
            CPPUNIT_TEST(testManual1);
            CPPUNIT_TEST(testAllocate0);
            CPPUNIT_TEST(testAllocate1);
            CPPUNIT_TEST(testInPlace0);
            CPPUNIT_TEST(testInPlace1);
            CPPUNIT_TEST_SUITE_END();
            
        public:
            MorphologyExTest() : m_operator(0) {}
            void setUp();
            void tearDown();
            
        protected:
            void testManual0();
            void testManual1();
            void testAllocate0();
            void testAllocate1();
            void testInPlace0();
            void testInPlace1();
            
        private:
            runtime::OperatorTester* m_operator;
        };
    } // cvimgproc
} // stromx

#endif // STROMX_CVIMGPROC_MORPHOLOGYEXTEST_H

#ifndef STROMX_CVIMGPROC_CORNERSUBPIXTEST_H
#define STROMX_CVIMGPROC_CORNERSUBPIXTEST_H

#include "stromx/cvimgproc/Config.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/OperatorTester.h"

namespace stromx
{
    namespace cvimgproc
    {
        class CornerSubPixTest : public CPPUNIT_NS::TestFixture
        {
            CPPUNIT_TEST_SUITE(CornerSubPixTest);
            CPPUNIT_TEST(testInPlace0);
            CPPUNIT_TEST_SUITE_END();
            
        public:
            CornerSubPixTest() : m_operator(0) {}
            void setUp();
            void tearDown();
            
        protected:
            void testInPlace0();
            
        private:
            runtime::OperatorTester* m_operator;
        };
    } // cvimgproc
} // stromx

#endif // STROMX_CVIMGPROC_CORNERSUBPIXTEST_H

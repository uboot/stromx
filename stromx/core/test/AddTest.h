#ifndef STROMX_CORE_ADDTEST_H
#define STROMX_CORE_ADDTEST_H

#include "stromx/core/Config.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/OperatorTester.h"

namespace stromx
{
    namespace core
    {
        class AddTest : public CPPUNIT_NS::TestFixture
        {
            CPPUNIT_TEST_SUITE(AddTest);
            CPPUNIT_TEST(testManual0);
            CPPUNIT_TEST(testManual1);
            CPPUNIT_TEST(testAllocate0);
            CPPUNIT_TEST(testAllocate1);
            CPPUNIT_TEST_SUITE_END();
            
        public:
            AddTest() : m_operator(0) {}
            void setUp();
            void tearDown();
            
        protected:
            void testManual0();
            void testManual1();
            void testAllocate0();
            void testAllocate1();
            
        private:
            runtime::OperatorTester* m_operator;
        };
    }
}

#endif // STROMX_CORE_ADDTEST_H

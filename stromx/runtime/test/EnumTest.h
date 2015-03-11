/* 
 *  Copyright 2013 Matthias Fuchs
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef STROMX_RUNTIME_ENUMTEST_H
#define STROMX_RUNTIME_ENUMTEST_H

#include "stromx/runtime/test/PrimitiveTest.h"
#include "stromx/runtime/Enum.h"

namespace stromx
{
    namespace runtime
    {
        class EnumTest : public PrimitiveTest<Enum, unsigned int>
        {
            CPPUNIT_TEST_SUITE (EnumTest);
            CPPUNIT_TEST (testDefaultConstructor);
            CPPUNIT_TEST (testConstructor);
            CPPUNIT_TEST (testCopyConstructor);
            CPPUNIT_TEST (testAssignment);
            CPPUNIT_TEST (testSerialize);
            CPPUNIT_TEST (testDeserialize);
            CPPUNIT_TEST (testIsVariant);
            CPPUNIT_TEST (testPtrCast);
            CPPUNIT_TEST (testRefCast);
            CPPUNIT_TEST_SUITE_END ();
            
        protected:
            virtual unsigned int value() const { return 3; }
            virtual std::string str() const { return "3"; }
            virtual const VariantInterface & variant() const { return Variant::ENUM; }
        };
    }
}

#endif // STROMX_RUNTIME_ENUMTEST_H
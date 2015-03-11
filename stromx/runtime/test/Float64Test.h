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

#ifndef STROMX_RUNTIME_FLOAT64TEST_H
#define STROMX_RUNTIME_FLOAT64TEST_H

#include "stromx/runtime/test/PrimitiveTest.h"

namespace stromx
{
    namespace runtime
    {
        class Float64Test : public PrimitiveTest<Float64, double>
        {
            CPPUNIT_TEST_SUITE (Float64Test);
            CPPUNIT_TEST (testDefaultConstructor);
            CPPUNIT_TEST (testConstructor);
            CPPUNIT_TEST (testCopyConstructor);
            CPPUNIT_TEST (testAssignment);
            CPPUNIT_TEST (testSerialize);
            CPPUNIT_TEST (testDeserialize);
            CPPUNIT_TEST (testIsVariant);
            CPPUNIT_TEST (testPtrCast);
            CPPUNIT_TEST (testRefCast);
            CPPUNIT_TEST (testIsFloatVariant);
            CPPUNIT_TEST_SUITE_END ();
            
        protected:
            virtual double value() const { return -5.3; }
            virtual std::string str() const { return "-5.3"; }
            virtual const VariantInterface & variant() const { return Variant::FLOAT_64; }
            
            void testIsFloatVariant();
        };
    }
}

#endif // STROMX_RUNTIME_FLOAT_64TEST_H
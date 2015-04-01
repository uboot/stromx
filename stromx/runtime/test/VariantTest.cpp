/* 
*  Copyright 2015 Matthias Fuchs
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

#include <cppunit/TestAssert.h>
#include "stromx/runtime/Variant.h"
#include "stromx/runtime/VariantComposite.h"
#include "stromx/runtime/test/VariantTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::VariantTest);

namespace stromx
{
    namespace runtime
    {
        void VariantTest::testImageTypes()
        {
            CPPUNIT_ASSERT(Variant::RGB_IMAGE.isVariant(Variant::IMAGE));
            CPPUNIT_ASSERT(Variant::RGB_24_IMAGE.isVariant(Variant::IMAGE));
            CPPUNIT_ASSERT(Variant::RGB_24_IMAGE.isVariant(Variant::RGB_IMAGE));
            
            CPPUNIT_ASSERT(! Variant::IMAGE.isVariant(Variant::RGB_IMAGE));
            CPPUNIT_ASSERT(! Variant::IMAGE.isVariant(Variant::RGB_24_IMAGE));
            CPPUNIT_ASSERT(! Variant::RGB_IMAGE.isVariant(Variant::RGB_24_IMAGE));
            
            CPPUNIT_ASSERT(Variant::MONO_IMAGE.isVariant(Variant::IMAGE));
            CPPUNIT_ASSERT(Variant::MONO_8_IMAGE.isVariant(Variant::IMAGE));
            CPPUNIT_ASSERT(Variant::MONO_8_IMAGE.isVariant(Variant::MONO_IMAGE));
        }

        void VariantTest::testIntTypes()
        {
            CPPUNIT_ASSERT(Variant::UINT.isVariant(Variant::INT));
            CPPUNIT_ASSERT(Variant::UINT_8.isVariant(Variant::UINT));
            CPPUNIT_ASSERT(Variant::UINT_16.isVariant(Variant::UINT));
            CPPUNIT_ASSERT(Variant::UINT_32.isVariant(Variant::UINT));
            
            CPPUNIT_ASSERT(! Variant::INT.isVariant(Variant::UINT));
            CPPUNIT_ASSERT(! Variant::UINT.isVariant(Variant::UINT_8));
            CPPUNIT_ASSERT(! Variant::UINT.isVariant(Variant::UINT_16));
            CPPUNIT_ASSERT(! Variant::UINT.isVariant(Variant::UINT_32));
        }
        
        void VariantTest::testInt64Types()
        {
            CPPUNIT_ASSERT(Variant::UINT_64.isVariant(Variant::UINT_64));
            CPPUNIT_ASSERT(Variant::INT_64.isVariant(Variant::INT_64));
            
            CPPUNIT_ASSERT(Variant::UINT_64.isVariant(Variant::UINT));
            CPPUNIT_ASSERT(Variant::UINT_64.isVariant(Variant::INT));
            
            CPPUNIT_ASSERT(! Variant::INT_64.isVariant(Variant::UINT));
            CPPUNIT_ASSERT(Variant::INT_64.isVariant(Variant::INT));
            
            CPPUNIT_ASSERT(! Variant::INT_64.isVariant(Variant::INT_32));
            CPPUNIT_ASSERT(! Variant::UINT_64.isVariant(Variant::INT_64));
            CPPUNIT_ASSERT(! Variant::INT_64.isVariant(Variant::UINT_64));
        }
        
        void VariantTest::testImageIsMatrix()
        {
            CPPUNIT_ASSERT(Variant::IMAGE.isVariant(Variant::MATRIX));
        }
        
        void VariantTest::testUInt8IsIntOrImage()
        {
            CPPUNIT_ASSERT(Variant::UINT_8.isVariant(Variant::INT || Variant::IMAGE));
        }
        
        void VariantTest::testInt32MatrixIsInt32MatrixOrFloat32Matrix()
        {
            CPPUNIT_ASSERT(Variant::INT_32_MATRIX.isVariant(Variant::INT_32_MATRIX || Variant::FLOAT_32_MATRIX));
        }
        
        void VariantTest::testFloat32MatrixIsInt32MatrixOrFloat32Matrix()
        {
            CPPUNIT_ASSERT(Variant::FLOAT_32_MATRIX.isVariant(Variant::INT_32_MATRIX || Variant::FLOAT_32_MATRIX));
        }
        
        void VariantTest::testFloat64MatrixIsNotInt32MatrixOrFloat32Matrix()
        {
            CPPUNIT_ASSERT(! Variant::FLOAT_64_MATRIX.isVariant(Variant::INT_32_MATRIX || Variant::FLOAT_32_MATRIX));
        }
        
        void VariantTest::testBinaryFileIsFile()
        {
            CPPUNIT_ASSERT(Variant::BINARY_FILE.isVariant(Variant::BINARY_FILE));
            CPPUNIT_ASSERT(Variant::BINARY_FILE.isVariant(Variant::FILE));
            CPPUNIT_ASSERT(! Variant::FILE.isVariant(Variant::BINARY_FILE));
        }
        
        void VariantTest::testTextFileIsFile()
        {
            CPPUNIT_ASSERT(Variant::TEXT_FILE.isVariant(Variant::TEXT_FILE));
            CPPUNIT_ASSERT(Variant::TEXT_FILE.isVariant(Variant::FILE));
            CPPUNIT_ASSERT(! Variant::FILE.isVariant(Variant::TEXT_FILE));
        }
    }
}

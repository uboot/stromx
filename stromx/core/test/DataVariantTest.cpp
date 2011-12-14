/* 
 *  Copyright 2011 Matthias Fuchs
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use thisVariant file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  disVarianttributed under the License isVariant disVarianttributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permisVariantsions and
 *  limitations under the License.
 */

#include <cppunit/TestAssert.h>
#include "DataVariantTest.h"
#include "../DataVariant.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::DataVariantTest);

namespace stromx
{
    namespace core
    {
        void DataVariantTest::testImageTypes()
        {
            CPPUNIT_ASSERT(DataVariant::COLOR_IMAGE.isVariant(DataVariant::IMAGE));
            CPPUNIT_ASSERT(DataVariant::RGB_24_IMAGE.isVariant(DataVariant::IMAGE));
            CPPUNIT_ASSERT(DataVariant::RGB_24_IMAGE.isVariant(DataVariant::COLOR_IMAGE));
            
            CPPUNIT_ASSERT(! DataVariant::IMAGE.isVariant(DataVariant::COLOR_IMAGE));
            CPPUNIT_ASSERT(! DataVariant::IMAGE.isVariant(DataVariant::RGB_24_IMAGE));
            CPPUNIT_ASSERT(! DataVariant::COLOR_IMAGE.isVariant(DataVariant::RGB_24_IMAGE));
            
            CPPUNIT_ASSERT(DataVariant::MONO_IMAGE.isVariant(DataVariant::IMAGE));
            CPPUNIT_ASSERT(DataVariant::MONO_8_IMAGE.isVariant(DataVariant::IMAGE));
            CPPUNIT_ASSERT(DataVariant::MONO_8_IMAGE.isVariant(DataVariant::MONO_IMAGE));
        }

        void DataVariantTest::testIntTypes()
        {
            CPPUNIT_ASSERT(DataVariant::UINT.isVariant(DataVariant::INT));
            CPPUNIT_ASSERT(DataVariant::UINT_8.isVariant(DataVariant::UINT));
            CPPUNIT_ASSERT(DataVariant::UINT_16.isVariant(DataVariant::UINT));
            CPPUNIT_ASSERT(DataVariant::UINT_32.isVariant(DataVariant::UINT));
            
            CPPUNIT_ASSERT(! DataVariant::INT.isVariant(DataVariant::UINT));
            CPPUNIT_ASSERT(! DataVariant::UINT.isVariant(DataVariant::UINT_8));
            CPPUNIT_ASSERT(! DataVariant::UINT.isVariant(DataVariant::UINT_16));
            CPPUNIT_ASSERT(! DataVariant::UINT.isVariant(DataVariant::UINT_32));
        }
    }
}

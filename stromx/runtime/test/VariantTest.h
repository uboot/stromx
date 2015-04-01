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

#ifndef STROMX_RUNTIME_VARIANTTEST_H
#define STROMX_RUNTIME_VARIANTTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

namespace stromx
{
    namespace runtime
    {
        class VariantTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (VariantTest);
            CPPUNIT_TEST (testIntTypes);
            CPPUNIT_TEST (testImageTypes);
            CPPUNIT_TEST (testInt64Types);
            CPPUNIT_TEST (testImageIsMatrix);
            CPPUNIT_TEST (testBinaryFileIsFile);
            CPPUNIT_TEST (testTextFileIsFile);
            CPPUNIT_TEST (testUInt8IsIntOrImage);
            CPPUNIT_TEST (testInt32MatrixIsInt32MatrixOrFloat32Matrix);
            CPPUNIT_TEST (testFloat32MatrixIsInt32MatrixOrFloat32Matrix);
            CPPUNIT_TEST (testFloat64MatrixIsNotInt32MatrixOrFloat32Matrix);
            CPPUNIT_TEST_SUITE_END ();

            public:
                void setUp() {}
                void tearDown() {}

            protected:
                void testIntTypes();
                void testImageTypes();
                void testInt64Types();
                void testBinaryFileIsFile();
                void testTextFileIsFile();
                void testImageIsMatrix();
                void testUInt8IsIntOrImage();
                void testInt32MatrixIsInt32MatrixOrFloat32Matrix();
                void testFloat32MatrixIsInt32MatrixOrFloat32Matrix();
                void testFloat64MatrixIsNotInt32MatrixOrFloat32Matrix();
        };
    }
}

#endif // STROMX_RUNTIME_VARIANTTEST_H
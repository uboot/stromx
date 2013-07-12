/* 
*  Copyright 2012 Matthias Fuchs
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

#ifndef STROMX_EXAMPLE_MATRIXTEST_H
#define STROMX_EXAMPLE_MATRIXTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <stromx/runtime/Version.h>

namespace stromx
{
    namespace example
    {
        class Matrix;

        class MatrixTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (MatrixTest);
            CPPUNIT_TEST (testMatrixEmpty);
            CPPUNIT_TEST (testMatrixCopyConstructor);
            CPPUNIT_TEST (testMatrixEmptyCopyConstructor);
            CPPUNIT_TEST (testMatrixDefaultCopyConstructor);
            CPPUNIT_TEST (testMatrixCvMatrix64F);
            CPPUNIT_TEST (testMatrixCvMatrix32FC2);
            CPPUNIT_TEST (testMatrixCvMatrix16SC3);
            CPPUNIT_TEST (testMatrixCvMatrix32UC4);
            CPPUNIT_TEST (testMatrixFloat32);
            CPPUNIT_TEST (testMatrixDefault);
            CPPUNIT_TEST (testSerializeInt16);
            CPPUNIT_TEST (testSerializeUInt8);
            CPPUNIT_TEST (testSerializeEmpty);
            CPPUNIT_TEST (testDeserializeFloat64);
            CPPUNIT_TEST (testDeserializeUInt16);
            CPPUNIT_TEST (testDeserializeEmpty);
            CPPUNIT_TEST (testResizeDimension);
            CPPUNIT_TEST (testResizeBuffer);
            CPPUNIT_TEST (testOpenUInt16);
            CPPUNIT_TEST (testOpenFloat64);
            CPPUNIT_TEST (testOpenEmpty);
            CPPUNIT_TEST (testSaveFloat64);
            CPPUNIT_TEST (testSaveUInt16);
            CPPUNIT_TEST (testSaveEmpty);
            CPPUNIT_TEST(testIsVariant);
            CPPUNIT_TEST(testPtrCast);
            CPPUNIT_TEST(testRefCast);
            CPPUNIT_TEST_SUITE_END ();

            public:
                MatrixTest() : m_matrix(0) {}
                
                void setUp();
                void tearDown();

            protected:
                void testMatrixEmpty();
                void testMatrixCopyConstructor();
                void testMatrixEmptyCopyConstructor();
                void testMatrixDefaultCopyConstructor();
                void testMatrixCvMatrix64F();
                void testMatrixCvMatrix32FC2();
                void testMatrixCvMatrix16SC3();
                void testMatrixCvMatrix32UC4();
                void testMatrixFloat32();
                void testMatrixInt16();
                void testMatrixDefault();
                void testSerializeInt16();
                void testSerializeUInt8();
                void testSerializeEmpty();
                void testDeserializeFloat64();
                void testDeserializeUInt16();
                void testDeserializeEmpty();
                void testResizeDimension();
                void testResizeBuffer();
                void testOpenUInt16();
                void testOpenFloat64();
                void testOpenEmpty();
                void testSaveFloat64();
                void testSaveUInt16();
                void testSaveEmpty();
                void testIsVariant();
                void testPtrCast();
                void testRefCast();
                
            private:
                const static stromx::runtime::Version VERSION;
                
                Matrix* m_matrix;
        };
    }
}

#endif // STROMX_EXAMPLE_MATRIXTEST_H
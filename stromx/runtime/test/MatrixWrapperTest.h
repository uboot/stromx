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

#ifndef STROMX_RUNTIME_MATRIXWRAPPERTEST_H
#define STROMX_RUNTIME_MATRIXWRAPPERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <stromx/runtime/Version.h>

namespace stromx
{
    namespace runtime
    {
        class MatrixWrapper;

        class MatrixWrapperTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (MatrixWrapperTest);
            CPPUNIT_TEST (testSerializeInt16);
            CPPUNIT_TEST (testSerializeUInt8);
            CPPUNIT_TEST (testSerializeEmpty);
            CPPUNIT_TEST (testDeserializeDouble);
            CPPUNIT_TEST (testDeserializeUInt16);
            CPPUNIT_TEST (testDeserializeEmpty);
            CPPUNIT_TEST (testResizeDimension);
            CPPUNIT_TEST (testResizeLength);
            CPPUNIT_TEST (testAt);
            CPPUNIT_TEST (testAtWrongPosition);
            CPPUNIT_TEST (testAtWrongType);
            CPPUNIT_TEST_SUITE_END ();

            public:
                MatrixWrapperTest() : m_matrix(0) {}
                
                void setUp();
                void tearDown();

            protected:
                void testSerializeInt16();
                void testSerializeUInt8();
                void testSerializeEmpty();
                void testDeserializeDouble();
                void testDeserializeUInt16();
                void testDeserializeEmpty();
                void testResizeDimension();
                void testResizeLength();
                void testAt();
                void testAtWrongPosition();
                void testAtWrongType();
                
            private:
                const static stromx::runtime::Version VERSION;
                
                MatrixWrapper* m_matrix;
        };
    }
}

#endif // STROMX_RUNTIME_MATRIXWRAPPERTEST_H
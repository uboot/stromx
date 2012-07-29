/* 
*  Copyright 2011 Matthias Fuchs
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
#include "MatrixTest.h"
#include "../Config.h"
#include "../Matrix.h"
#include <stromx/core/DirectoryFileInput.h>
#include <stromx/core/DirectoryFileOutput.h>
#include <opencv2/core/core.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::MatrixTest);

namespace stromx
{
    namespace base
    {
        const stromx::core::Version MatrixTest::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR, BASE_VERSION_PATCH);
        
        void MatrixTest::setUp ( void )
        {
            m_matrix = 0;
        }
        
        void MatrixTest::testMatrixDefault()
        {
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL(core::Matrix::NONE, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_matrix->stride());
            CPPUNIT_ASSERT_EQUAL((uint8_t*)(0), m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(core::DataVariant::MATRIX, m_matrix->variant());
        }

        void MatrixTest::testMatrixEmpty()
        {
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(0, 100, Matrix::DOUBLE));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(core::Matrix::DOUBLE, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(800), m_matrix->stride());
            CPPUNIT_ASSERT_EQUAL((uint8_t*)(0), m_matrix->data()); 
            CPPUNIT_ASSERT_EQUAL(core::DataVariant::DOUBLE_MATRIX, m_matrix->variant());
        }

        void MatrixTest::testMatrixFloat()
        {
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(200, 100, Matrix::FLOAT));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(core::Matrix::FLOAT, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(400), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(core::DataVariant::FLOAT_MATRIX, m_matrix->variant());
        }

        void MatrixTest::testMatrixInt16()
        {
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(200, 100, Matrix::INT_16));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(core::Matrix::INT_16, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(core::DataVariant::INT_16_MATRIX, m_matrix->variant());
        }

        void MatrixTest::testMatrixDefaultCopyConstructor()
        {
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(200, 100, Matrix::FLOAT));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(core::Matrix::FLOAT, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(400), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(core::DataVariant::FLOAT_MATRIX, m_matrix->variant());
        }

        void MatrixTest::testMatrixCopyConstructor()
        {
            Matrix matrix(200, 100, Matrix::UINT_16);
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(matrix));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(core::Matrix::UINT_16, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(core::DataVariant::UINT_16_MATRIX, m_matrix->variant());

        }

        void MatrixTest::testMatrixEmptyCopyConstructor()
        {
            Matrix matrix(100, 0, Matrix::INT_32);
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(matrix));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(core::Matrix::INT_32, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_matrix->stride());
            CPPUNIT_ASSERT_EQUAL((uint8_t*)(0), m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(core::DataVariant::INT_32_MATRIX, m_matrix->variant());
        }
        
        void MatrixTest::testImageCvImageConstructor()
        {
            cv::Mat cvMatrix(100, 200, CV_64F);
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(cvMatrix));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL(core::Matrix::DOUBLE, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1600), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(core::DataVariant::DOUBLE_MATRIX, m_matrix->variant());
        }

        void MatrixTest::testDeserialize()
        {
            m_matrix = new Matrix();
            
            core::DirectoryFileInput input(".");
            input.initialize("8 100 200", "double_matrix.bin");
            CPPUNIT_ASSERT_NO_THROW(m_matrix->deserialize(input, VERSION));
            CPPUNIT_ASSERT_EQUAL(Matrix::DOUBLE, m_matrix->valueType());
        }

        void MatrixTest::testDeserializeEmpty()
        {
            m_matrix = new Matrix();
            
            core::DirectoryFileInput input(".");
            input.initialize("7 100 0", "");
            CPPUNIT_ASSERT_NO_THROW(m_matrix->deserialize(input, VERSION));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(Matrix::FLOAT, m_matrix->valueType());
        }
        
        void MatrixTest::testSerialize()
        {
            m_matrix = new Matrix(200, 100, Matrix::INT_16);

            core::DirectoryFileOutput output(".");
            output.initialize("MatrixTest_testSerialize");
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix->serialize(output));
            CPPUNIT_ASSERT_EQUAL(std::string("3 200 100"), output.getText());
        }

        void MatrixTest::testSerializeEmpty()
        {
            m_matrix = new Matrix(0, 100, Matrix::INT_8);

            core::DirectoryFileOutput output(".");
            output.initialize("MatrixTest_testSerializeEmpty");
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix->serialize(output));
            CPPUNIT_ASSERT_EQUAL(std::string("1 0 100"), output.getText());
        }

        void MatrixTest::testResizeBuffer()
        {
            m_matrix = new Matrix();
            CPPUNIT_ASSERT_NO_THROW(m_matrix->resize(300, 200, core::Matrix::FLOAT));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(300), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_matrix->cols());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(core::Matrix::FLOAT, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL(core::DataVariant::FLOAT_MATRIX, m_matrix->variant());
        }

        void MatrixTest::testResizeDimension()
        {
            m_matrix = new Matrix();
            CPPUNIT_ASSERT_NO_THROW(m_matrix->resize(1024));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1024), m_matrix->bufferSize());
        }
     
        void MatrixTest::tearDown ( void )
        {
            delete m_matrix;
        }
    }
}

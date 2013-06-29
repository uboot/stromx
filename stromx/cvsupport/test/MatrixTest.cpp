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
#include <opencv2/core/core.hpp>
#include <stromx/runtime/DirectoryFileInput.h>
#include <stromx/runtime/DirectoryFileOutput.h>
#include <stromx/runtime/None.h>
#include "stromx/cvsupport/Config.h"
#include "stromx/cvsupport/Matrix.h"
#include "stromx/cvsupport/test/MatrixTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvsupport::MatrixTest);

namespace stromx
{
    namespace cvsupport
    {
        const stromx::runtime::Version MatrixTest::VERSION(0, 1, 0);
        
        void MatrixTest::setUp ( void )
        {
            m_matrix = 0;
        }
        
        void MatrixTest::testMatrixDefault()
        {
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::NONE, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_matrix->stride());
            CPPUNIT_ASSERT_EQUAL((uint8_t*)(0), m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::MATRIX, m_matrix->variant());
        }

        void MatrixTest::testMatrixEmpty()
        {
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(0, 100, Matrix::DOUBLE));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::DOUBLE, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(800), m_matrix->stride());
            CPPUNIT_ASSERT_EQUAL((uint8_t*)(0), m_matrix->data()); 
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::DOUBLE_MATRIX, m_matrix->variant());
        }

        void MatrixTest::testMatrixFloat()
        {
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(200, 100, Matrix::FLOAT));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::FLOAT, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(400), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::FLOAT_MATRIX, m_matrix->variant());
        }

        void MatrixTest::testMatrixInt16()
        {
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(200, 100, Matrix::INT_16));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::INT_16, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::INT_16_MATRIX, m_matrix->variant());
        }

        void MatrixTest::testMatrixDefaultCopyConstructor()
        {
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(200, 100, Matrix::FLOAT));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::FLOAT, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(400), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::FLOAT_MATRIX, m_matrix->variant());
        }

        void MatrixTest::testMatrixCopyConstructor()
        {
            Matrix matrix(200, 100, Matrix::UINT_16);
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(matrix));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::UINT_16, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::UINT_16_MATRIX, m_matrix->variant());

        }

        void MatrixTest::testMatrixEmptyCopyConstructor()
        {
            Matrix matrix(100, 0, Matrix::INT_32);
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(matrix));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::INT_32, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_matrix->stride());
            CPPUNIT_ASSERT_EQUAL((uint8_t*)(0), m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::INT_32_MATRIX, m_matrix->variant());
        }
        
        void MatrixTest::testMatrixCvMatrix64F()
        {
            cv::Mat cvMatrix(100, 200, CV_64F);
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(cvMatrix));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::DOUBLE, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1600), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::DOUBLE_MATRIX, m_matrix->variant());
        }
        
        void MatrixTest::testMatrixCvMatrix16SC3()
        {
            cv::Mat cvMatrix(100, 200, CV_16UC3);
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(cvMatrix));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(600), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::UINT_16, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1200), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::UINT_16_MATRIX, m_matrix->variant());
        }

        void MatrixTest::testMatrixCvMatrix32FC2()
        {
            cv::Mat cvMatrix(100, 200, CV_32FC2);
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(cvMatrix));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(400), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::FLOAT, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1600), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::FLOAT_MATRIX, m_matrix->variant());
        }

        void MatrixTest::testMatrixCvMatrix32UC4()
        {
            cv::Mat cvMatrix(100, 200, CV_32SC4);
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(cvMatrix));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(800), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::INT_32, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(3200), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::INT_32_MATRIX, m_matrix->variant());
        }
        
        void MatrixTest::testOpenDouble()
        {
            Matrix matrix("double_matrix.npy");
            CPPUNIT_ASSERT_EQUAL((unsigned int)(50), matrix.rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), matrix.cols());
            CPPUNIT_ASSERT_EQUAL(Matrix::DOUBLE, matrix.valueType());
        }

        void MatrixTest::testOpenUInt16()
        {
            Matrix matrix("uint16_matrix.npy");
            CPPUNIT_ASSERT_EQUAL((unsigned int)(3), matrix.rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(4), matrix.cols());
            CPPUNIT_ASSERT_EQUAL(Matrix::UINT_16, matrix.valueType());
        }

        void MatrixTest::testOpenEmpty()
        {
            Matrix matrix("empty_float_matrix.npy");
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), matrix.rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), matrix.cols());
            CPPUNIT_ASSERT_EQUAL(Matrix::FLOAT, matrix.valueType());
        }
        
        void MatrixTest::testSaveUInt16()
        {
            Matrix matrix(200, 100, Matrix::UINT_16);
            matrix.save("MatrixTest_testSaveUInt16");
        }

        void MatrixTest::testSaveDouble()
        {
            Matrix matrix(50, 10, Matrix::DOUBLE);
            matrix.save("MatrixTest_testSaveDouble");
        }

        void MatrixTest::testSaveEmpty()
        {
            Matrix matrix(0, 10, Matrix::INT_32);
            matrix.save("MatrixTest_testSaveEmpty");
        }
        
        void MatrixTest::testIsVariant()
        {
            m_matrix = new Matrix(0, 100, Matrix::INT_8);
            CPPUNIT_ASSERT(m_matrix->variant().isVariant(runtime::DataVariant::MATRIX));
            CPPUNIT_ASSERT(! m_matrix->variant().isVariant(runtime::DataVariant::NONE));
        }
        
        void MatrixTest::testPtrCast()
        {
            m_matrix = new Matrix(0, 100, Matrix::INT_8);
            CPPUNIT_ASSERT(runtime::data_cast<runtime::Data>(m_matrix));
            CPPUNIT_ASSERT(runtime::data_cast<runtime::Matrix>(m_matrix));
            CPPUNIT_ASSERT_EQUAL(runtime::data_cast<runtime::None>(m_matrix), static_cast<runtime::None*>(0));
        }
        
        void MatrixTest::testRefCast()
        {
            m_matrix = new Matrix(0, 100, Matrix::INT_8);
            CPPUNIT_ASSERT_NO_THROW(runtime::data_cast<runtime::Data>(*m_matrix));
            CPPUNIT_ASSERT_NO_THROW(runtime::data_cast<runtime::Matrix>(*m_matrix));
            CPPUNIT_ASSERT_THROW(runtime::data_cast<runtime::None>(*m_matrix), runtime::BadCast);
        }
     
        void MatrixTest::tearDown ( void )
        {
            delete m_matrix;
        }
    }
}

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
        
        void MatrixTest::testMatrixFileConstructor()
        {

        }
        
        void MatrixTest::testMatrixCvMatrix64F()
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
        
        void MatrixTest::testMatrixCvMatrix16SC3()
        {
            cv::Mat cvMatrix(100, 200, CV_16UC3);
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(cvMatrix));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(600), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL(core::Matrix::UINT_16, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1200), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(core::DataVariant::UINT_16_MATRIX, m_matrix->variant());
        }

        void MatrixTest::testMatrixCvMatrix32FC2()
        {
            cv::Mat cvMatrix(100, 200, CV_32FC2);
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(cvMatrix));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(400), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL(core::Matrix::FLOAT, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1600), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(core::DataVariant::FLOAT_MATRIX, m_matrix->variant());
        }

        void MatrixTest::testMatrixCvMatrix32UC4()
        {
            cv::Mat cvMatrix(100, 200, CV_32SC4);
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix = new Matrix(cvMatrix));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(800), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL(core::Matrix::INT_32, m_matrix->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(3200), m_matrix->stride());
            CPPUNIT_ASSERT(m_matrix->data());
            CPPUNIT_ASSERT_EQUAL(core::DataVariant::INT_32_MATRIX, m_matrix->variant());
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

        void MatrixTest::testDeserializeDouble()
        {
            m_matrix = new Matrix();
            
            core::DirectoryFileInput input(".");
            input.initialize("", "double_matrix.npy");
            CPPUNIT_ASSERT_NO_THROW(m_matrix->deserialize(input, VERSION));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(50), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(Matrix::DOUBLE, m_matrix->valueType());
            
            const uint8_t* rowPtr = m_matrix->data();
            for(unsigned int i = 0; i < m_matrix->rows(); ++i)
            {
                double* doubleData = (double*)(rowPtr);
                for(unsigned int j = 0; j < m_matrix->cols(); ++j)
                    CPPUNIT_ASSERT_EQUAL(double(0), doubleData[j]);
                rowPtr += m_matrix->stride();
            }
        }

        void MatrixTest::testDeserializeUInt16()
        {
            m_matrix = new Matrix();
            
            core::DirectoryFileInput input(".");
            input.initialize("", "uint16_matrix.npy");
            CPPUNIT_ASSERT_NO_THROW(m_matrix->deserialize(input, VERSION));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(3), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(4), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(Matrix::UINT_16, m_matrix->valueType());
            
            const uint8_t* rowPtr = m_matrix->data();
            for(unsigned int i = 0; i < m_matrix->rows(); ++i)
            {
                uint16_t* uint16Data = (uint16_t*)(rowPtr);
                for(unsigned int j = 0; j < m_matrix->cols(); ++j)
                    CPPUNIT_ASSERT_EQUAL(uint16_t(4 * i + j), uint16Data[j]);
                rowPtr += m_matrix->stride();
            }
        }

        void MatrixTest::testDeserializeEmpty()
        {
            m_matrix = new Matrix();
            
            core::DirectoryFileInput input(".");
            input.initialize("", "empty_float_matrix.npy");
            CPPUNIT_ASSERT_NO_THROW(m_matrix->deserialize(input, VERSION));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_matrix->rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_matrix->cols());
            CPPUNIT_ASSERT_EQUAL(Matrix::FLOAT, m_matrix->valueType());
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

        void MatrixTest::testSerializeInt16()
        {
            m_matrix = new Matrix(200, 100, Matrix::INT_16);
            memset(m_matrix->data(), 0, m_matrix->rows() * m_matrix->stride());

            core::DirectoryFileOutput output(".");
            output.initialize("MatrixTest_testSerializeInt16");
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix->serialize(output));
            CPPUNIT_ASSERT_EQUAL(std::string(""), output.getText());
        }
        
        void MatrixTest::testSerializeUInt8()
        {
            m_matrix = new Matrix(20, 10, Matrix::UINT_8);
            memset(m_matrix->data(), 0, m_matrix->rows() * m_matrix->stride());

            core::DirectoryFileOutput output(".");
            output.initialize("MatrixTest_testSerializeUInt8");
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix->serialize(output));
            CPPUNIT_ASSERT_EQUAL(std::string(""), output.getText());
        }

        void MatrixTest::testSerializeEmpty()
        {
            m_matrix = new Matrix(0, 100, Matrix::INT_8);

            core::DirectoryFileOutput output(".");
            output.initialize("MatrixTest_testSerializeEmpty");
            
            CPPUNIT_ASSERT_NO_THROW(m_matrix->serialize(output));
            CPPUNIT_ASSERT_EQUAL(std::string(""), output.getText());
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

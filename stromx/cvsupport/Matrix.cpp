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

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <opencv2/core/core.hpp>
#include <fstream>
#include "stromx/cvsupport/Matrix.h"
#include "stromx/cvsupport/Utilities.h"
#include <stromx/runtime/InputProvider.h>
#include <stromx/runtime/OutputProvider.h>

namespace stromx
{
    namespace cvsupport
    {
        const std::string Matrix::TYPE = "Matrix";
        const std::string Matrix::PACKAGE = STROMX_CVSUPPORT_PACKAGE_NAME;
        const runtime::Version Matrix::VERSION = runtime::Version(0, 1, 0);
                
        Matrix::Matrix()
          : m_matrix(new cv::Mat())
        {
            getDataFromCvMatrix(NONE);
        }

        Matrix::Matrix(const unsigned int rows, const unsigned int cols, const stromx::runtime::Matrix::ValueType valueType)
          : m_matrix(new cv::Mat())
        {
            allocate(rows, cols, valueType);
            getDataFromCvMatrix(valueType);
        }
        
        Matrix::Matrix(cv::Mat& cvMatrix)
          : m_matrix(new cv::Mat(cvMatrix))
        {
            getDataFromCvMatrix(valueTypeFromCvType(m_matrix->type()));
        }

        Matrix::Matrix(const stromx::runtime::Matrix& matrix)
          : m_matrix(new cv::Mat())
        {
            copy(matrix);
        }

        Matrix::Matrix(const stromx::cvsupport::Matrix& matrix)
          : runtime::MatrixWrapper(),
            m_matrix(new cv::Mat())
        {
            copy(matrix);
        }

        Matrix::Matrix(const unsigned int size)
          : m_matrix(new cv::Mat())
        {
            allocate(size, 1, UINT_8);
        }
        
        Matrix::Matrix(const std::string& filename)
          : m_matrix(new cv::Mat())
        {
            open(filename);
        }

        Matrix::~Matrix()
        {
            delete m_matrix;
        }

        runtime::Data* Matrix::clone() const
        {
            return new Matrix(*this);
        }
        
        const Matrix & Matrix::operator=(const runtime::Matrix& matrix)
        {
            copy(matrix);
            return *this;
        }

        void Matrix::copy(const stromx::runtime::Matrix& matrix)
        {
            allocate(matrix.rows(), matrix.cols(), matrix.valueType());
            
            try
            {
                cv::Mat cvInImage = getOpenCvMat(matrix);
                cvInImage.copyTo(*m_matrix);
            }
            catch(cv::Exception&)
            {
                throw stromx::runtime::Exception("Failed to copy construct matrix.");
            }
        }

        void Matrix::allocate(const unsigned int rows, const unsigned int cols, const runtime::Matrix::ValueType valueType)
        {
            try
            {
                *m_matrix = cv::Mat(rows, cols, cvTypeFromValueType(valueType));
                getDataFromCvMatrix(valueType);
            }
            catch(cv::Exception&)
            {
                throw runtime::OutOfMemory("Failed to allocate matrix.");
            }
        }
        
        void Matrix::getDataFromCvMatrix(const ValueType valueType)
        {
            setBuffer((uint8_t*)(m_matrix->data), m_matrix->step * m_matrix->rows);
            initializeMatrix(m_matrix->rows, m_matrix->cols * m_matrix->channels(), m_matrix->step,
                             (uint8_t*)(m_matrix->data), valueType);
        }
        
        int Matrix::cvTypeFromValueType(const runtime::Matrix::ValueType valueType)
        {
            switch(valueType)
            {
            case runtime::Matrix::NONE:
            case runtime::Matrix::UINT_8:
                return CV_8UC1;
            case runtime::Matrix::INT_8:
                return CV_8SC1;
            case runtime::Matrix::UINT_16:
                return CV_16UC1;
            case runtime::Matrix::INT_16:
                return CV_16SC1;
            case runtime::Matrix::INT_32:
                return CV_32SC1;
            case runtime::Matrix::FLOAT:
                return CV_32FC1;
            case runtime::Matrix::DOUBLE:
                return CV_64FC1;
            default:
                throw runtime::WrongArgument("Unsupported value type.");  
            }
        }
        
        runtime::Matrix::ValueType Matrix::valueTypeFromCvType(const int cvType)
        {
            switch(cvType)
            {
            case CV_8UC1:
            case CV_8UC2:
            case CV_8UC3:
            case CV_8UC4:
                return runtime::Matrix::UINT_8;
            case CV_8SC1:
            case CV_8SC2:
            case CV_8SC3:
            case CV_8SC4:
                return runtime::Matrix::INT_8;
            case CV_16UC1:
            case CV_16UC2:
            case CV_16UC3:
            case CV_16UC4:
                return runtime::Matrix::UINT_16;
            case CV_16SC1:
            case CV_16SC2:
            case CV_16SC3:
            case CV_16SC4:
                return runtime::Matrix::INT_16;
            case CV_32SC1:
            case CV_32SC2:
            case CV_32SC3:
            case CV_32SC4:
                return runtime::Matrix::INT_32;
            case CV_32FC1:
            case CV_32FC2:
            case CV_32FC3:
            case CV_32FC4:
                return runtime::Matrix::FLOAT;
            case CV_64FC1:
            case CV_64FC2:
            case CV_64FC3:
            case CV_64FC4:
                return runtime::Matrix::DOUBLE;
            default:
                throw runtime::WrongArgument("Unsupported OpenCV element type.");  
            }
        }
    }
}

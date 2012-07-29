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

#include "Matrix.h"
#include "Utilities.h"
#include <stromx/core/OutputProvider.h>
#include <stromx/core/InputProvider.h>
#include <opencv2/core/core.hpp>


namespace stromx
{
    namespace base
    {
        const std::string Matrix::TYPE = "Matrix";
        const std::string Matrix::PACKAGE = STROMX_BASE_PACKAGE_NAME;
        const core::Version Matrix::VERSION = core::Version(BASE_VERSION_MAJOR, BASE_VERSION_MINOR, BASE_VERSION_PATCH);
                
        Matrix::Matrix()
          : m_matrix(new cv::Mat())
        {
            getDataFromCvMatrix(NONE);
        }

        Matrix::Matrix(const unsigned int rows, const unsigned int cols, const stromx::core::Matrix::ValueType valueType)
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

        Matrix::Matrix(const stromx::core::Matrix& matrix)
          : m_matrix(new cv::Mat())
        {
            copy(matrix);
        }

        Matrix::Matrix(const stromx::base::Matrix& matrix)
          : m_matrix(new cv::Mat())
        {
            copy(matrix);
        }

        Matrix::Matrix(const unsigned int size)
          : m_matrix(new cv::Mat())
        {
            allocate(size, 1, UINT_8);
        }

        Matrix::~Matrix()
        {
            delete m_matrix;
        }

        core::Data*const Matrix::clone() const
        {
            return new Matrix(*this);
        }

        void Matrix::deserialize(core::InputProvider& input, const stromx::core::Version& version)
        {
            int rows = -1;
            int cols = -1;
            int type = -1;
            
            input.text() >> type;
            input.text() >> rows;
            input.text() >> cols;
            
            allocate(rows, cols, ValueType(type));
                
            if(rows == 0 || cols == 0)
                return;
            
            input.openFile(core::InputProvider::BINARY);
            
            // read file to matrix memory
        }

        void Matrix::serialize(core::OutputProvider& output) const
        {
            stromx::core::Data::serialize(output);
            
            output.text() << (unsigned int)(valueType()) << " ";
            output.text() << rows() << " ";
            output.text() << cols();
            
            if(rows() == 0 || cols() == 0)
                return;
            
            std::ostream & outStream = output.openFile("bin", core::OutputProvider::BINARY);
            
            // write file from matrix memory
        }

        void Matrix::copy(const stromx::core::Matrix& matrix)
        {
            allocate(matrix.rows(), matrix.cols(), matrix.valueType());
            
            try
            {
                cv::Mat cvInImage = getOpenCvMat(matrix);
                cvInImage.copyTo(*m_matrix);
            }
            catch(cv::Exception&)
            {
                throw stromx::core::Exception("Failed to copy construct matrix.");
            }
        }

        void Matrix::resize(const unsigned int rows, const unsigned int cols, const stromx::core::Matrix::ValueType valueType)
        {
            allocate(rows, cols, valueType);
        }

        void Matrix::resize(const unsigned int size)
        {
            allocate(size, 1, core::Matrix::NONE);
        }

        void Matrix::allocate(const unsigned int rows, const unsigned int cols, const core::Matrix::ValueType valueType)
        {
            try
            {
                *m_matrix = cv::Mat(rows, cols, cvTypeFromValueType(valueType));
                getDataFromCvMatrix(valueType);
            }
            catch(cv::Exception&)
            {
                throw core::OutOfMemory("Failed to allocate image.");
            }
        }
        
        void Matrix::getDataFromCvMatrix(const ValueType valueType)
        {
            setBuffer((uint8_t*)(m_matrix->data), m_matrix->step * m_matrix->rows);
            initializeMatrix(m_matrix->rows, m_matrix->cols, m_matrix->step,
                             (uint8_t*)(m_matrix->data), valueType);
        }
        
        const int Matrix::cvTypeFromValueType(const core::Matrix::ValueType valueType)
        {
            switch(valueType)
            {
            case core::Matrix::NONE:
            case core::Matrix::UINT_8:
                return CV_8UC1;
            case core::Matrix::INT_8:
                return CV_8SC1;
            case core::Matrix::UINT_16:
                return CV_16UC1;
            case core::Matrix::INT_16:
                return CV_16SC1;
            case core::Matrix::INT_32:
                return CV_32SC1;
            case core::Matrix::FLOAT:
                return CV_32FC1;
            case core::Matrix::DOUBLE:
                return CV_64FC1;
            default:
                throw core::WrongArgument("Unsupported value type.");  
            }
        }
        
        const core::Matrix::ValueType Matrix::valueTypeFromCvType(const int cvType)
        {
            switch(cvType)
            {
            case CV_8UC1:
                return core::Matrix::UINT_8;
            case CV_8SC1:
                return core::Matrix::INT_8;
            case CV_16UC1:
                return core::Matrix::UINT_16;
            case CV_16SC1:
                return core::Matrix::INT_16;
            case CV_32SC1:
                return core::Matrix::INT_32;
            case CV_32FC1:
                return core::Matrix::FLOAT;
            case CV_64FC1:
                return core::Matrix::DOUBLE;
            default:
                throw core::WrongArgument("Unsupported OpenCV element type.");  
            }
        }
    }
}

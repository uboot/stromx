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

#ifndef STROMX_BASE_MATRIX_H
#define STROMX_BASE_MATRIX_H

#include "Config.h"
#include <string>
#include <vector>
#include <stromx/core/MatrixWrapper.h>

namespace cv
{
    class Mat;
}

namespace stromx
{
    namespace base
    {
        /** \brief Matrix implementation based on OpenCV matrices. */
        class STROMX_BASE_API Matrix : public core::MatrixWrapper
        {
            friend cv::Mat getOpenCvMat(const core::Matrix& matrix);
        
        public:
            Matrix();
            explicit Matrix(const unsigned int rows, const unsigned int cols, const ValueType valueType);
            explicit Matrix(cv::Mat& cvMatrix);
            explicit Matrix(const stromx::core::Matrix& matrix);
            explicit Matrix(const stromx::base::Matrix& matrix);
            explicit Matrix(const unsigned int size);
            
            virtual ~Matrix();
            
            virtual const core::Version & version() const { return VERSION; }
            virtual const std::string & type() const { return TYPE; }
            virtual const std::string & package() const { return PACKAGE; }
            
            virtual Data* const clone() const;
            
            virtual void serialize(core::OutputProvider & output) const;
            virtual void deserialize(core::InputProvider & input, const stromx::core::Version & version);
            
            void resize(const unsigned int rows, const unsigned int cols, const ValueType valueType);
            void resize(const unsigned int size);
            
        private:
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const core::Version VERSION;
            static const char NUMPY_MAGIC_BYTE = char(0x93);
            
            static const int cvTypeFromValueType(const core::Matrix::ValueType valueType);
            static const core::Matrix::ValueType valueTypeFromCvType(const int cvType);
            static const bool isLittleEndian();
            static const char npyTypeSymbol(const core::Matrix::ValueType valueType);
            Matrix::ValueType valueTypeFromNpyHeader(const char valueType, const int wordSize);
            
            void copy(const stromx::core::Matrix & matrix);
            void allocate(const unsigned int rows, const unsigned int cols, const core::Matrix::ValueType valueType);
            void getDataFromCvMatrix(const ValueType valueType);
                
            cv::Mat* m_matrix;
        };
    }
}

#endif // STROMX_BASE_MATRIX_H
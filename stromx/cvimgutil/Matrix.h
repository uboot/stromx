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

#ifndef STROMX_IMGUTIL_MATRIX_H
#define STROMX_IMGUTIL_MATRIX_H

#include <string>
#include <vector>
#include "stromx/cvimgutil/Config.h"
#include <stromx/runtime/MatrixWrapper.h>

namespace cv
{
    class Mat;
}

namespace stromx
{
    namespace cvimgutil
    {
        /** \brief %Matrix implementation based on OpenCV matrices. */
        class STROMX_IMGUTIL_API Matrix : public runtime::MatrixWrapper
        {
            friend STROMX_IMGUTIL_API cv::Mat getOpenCvMat(const runtime::Matrix& matrix);
        
        public:
            
            /** 
             * Saves the input \c matrix to \c filename. The matrix is stored in the NPY format.
             */
            static void save(const std::string& filename, const runtime::Matrix & matrix);
            
            /** 
             * Constructs an empty matrix with zero rows and columns and value type Matrix::NONE.
             * No data is associated with the matrix, i.e. its data and buffer pointers are 0.
             */
            Matrix();
            
            /** Constructs a matrix for the given parameters. */
            explicit Matrix(const unsigned int rows, const unsigned int cols, const ValueType valueType);
            
            /** 
             * Constructs a matrix from \c cvMatrix. The data of the input matrix is not copied
             * only a lightweight wrapper is allocated which references \c cvMatrix.
             */
            explicit Matrix(cv::Mat& cvMatrix);
            
            /** Copy constructs a matrix from \c matrix. */
            explicit Matrix(const stromx::runtime::Matrix& matrix);
            
            /** Copy constructs a matrix from \c matrix. */
            explicit Matrix(const stromx::cvimgutil::Matrix& matrix);
            
            /** 
             * Allocates a matrix with a buffer of a given size in bytes. The matrix has 1 row and
             * \c size columns. Its value type is Image::NONE. The buffer size of the image is 
             * guaranteed to be at least \c size.
             */
            explicit Matrix(const unsigned int size);
            
            /** Allocates a matrix and reads its content from the file \c filename. */
            explicit Matrix(const std::string & filename);
            
            virtual ~Matrix();
            
            virtual const runtime::Version & version() const { return VERSION; }
            virtual const std::string & type() const { return TYPE; }
            virtual const std::string & package() const { return PACKAGE; }
            
            virtual Data* clone() const;
            
            virtual void serialize(runtime::OutputProvider & output) const;
            virtual void deserialize(runtime::InputProvider & input, const stromx::runtime::Version & version);
            
            /** Resizes the matrix and changes the value type of the matrix. */
            void resize(const unsigned int rows, const unsigned int cols, const ValueType valueType);
            
            /**
             * Resizes the buffer of the matrix to the given size in bytes. The resized matrix has
             * 1 row and \c size columns. Its value type is Matrix::NONE. The buffer size of the matrix
             * is guaranteed to be at least \c size.
             */
            void resize(const unsigned int size);
            
            /** 
             * Reads the file \c filename. The data of the current matrix is replaced 
             * by the data of the new image.
             */ 
            void open(const std::string& filename);
            
            /** 
             * Saves the matrix to the file \c filename. The matrix is stored in the NPY format.
             */
            void save(const std::string& filename) const;
            
        private:
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION;
            static const char NUMPY_MAGIC_BYTE = char(0x93);
            
            static void doSerialize(std::ostream & out, const runtime::Matrix & matrix);
            static void doDeserialize(std::istream & in, cvimgutil::Matrix & matrix);
            static int cvTypeFromValueType(const runtime::Matrix::ValueType valueType);
            static runtime::Matrix::ValueType valueTypeFromCvType(const int cvType);
            static bool isLittleEndian();
            static char npyTypeSymbol(const runtime::Matrix::ValueType valueType);
            static Matrix::ValueType valueTypeFromNpyHeader(const char valueType, const int wordSize);
            
            void copy(const stromx::runtime::Matrix & matrix);
            void allocate(const unsigned int rows, const unsigned int cols, const runtime::Matrix::ValueType valueType);
            void getDataFromCvMatrix(const ValueType valueType);
            
                
            cv::Mat* m_matrix;
        };
    }
}

#endif // STROMX_IMGUTIL_MATRIX_H
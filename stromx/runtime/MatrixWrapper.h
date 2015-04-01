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

#ifndef STROMX_RUNTIME_MATRIXWRAPPER_H
#define STROMX_RUNTIME_MATRIXWRAPPER_H

#include "stromx/runtime/Matrix.h"

namespace stromx
{
        namespace runtime
    {
        /** \brief Concrete matrix without memory management. */
        class STROMX_RUNTIME_API MatrixWrapper : public Matrix
        {
        public:
            /** 
             * Saves the input \c matrix to \c filename. The matrix is stored in the NPY format.
             */
            static void save(const std::string& filename, const runtime::Matrix & matrix);
            
            /** 
             * Constructs an matrix wrapper from a given memory buffer. The buffer is not
             * owned by the matrix and is thus not freed upon destruction of the matrix wrapper.
             */
            MatrixWrapper(const unsigned int bufferSize, uint8_t* const buffer);
            
            /** 
             * Constructs an empty matrix wrapper, i.e. is an matrix wrapper without associated
             * memory buffer.
             */
            MatrixWrapper();
            
            // Implementation of stromx::runtime::Matrix
            virtual const VariantHandle & variant() const { return m_variant; }
            virtual uint8_t* buffer() { return m_buffer; }
            virtual unsigned int bufferSize() const { return m_bufferSize; }
            virtual unsigned int rows() const { return m_rows; }
            virtual unsigned int cols() const { return m_cols; }
            virtual unsigned int stride() const { return m_stride; }
            virtual ValueType valueType() const { return m_valueType; }
            virtual uint8_t* data() { return m_data; }
            virtual const uint8_t* data() const { return m_data; }
            virtual void initializeMatrix(const unsigned int rows, 
                                          const unsigned int cols, 
                                          const unsigned int stride, 
                                          uint8_t* data, 
                                          const ValueType valueType);
            virtual void serialize(OutputProvider & output) const;
            virtual void deserialize(InputProvider & input, const Version & version);
            
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
            
        protected:
            /** 
             * Sets a new matrix buffer. Note that the matrix data defined by dimensions of 
             * the matrix value type and data must always be contained in the matrix buffer.
             * 
             * \param buffer The new buffer. Note that this memory is \em not released by
             *               the MatrixWrapper.
             * \param bufferSize The size of \a buffer in bytes.
             */
            void setBuffer(uint8_t* const buffer, const unsigned int bufferSize);
            
            /**
             * Allocates or resizes the matrix to the given dimension. Must be defined
             * in derived class which implement the memory management. Implementations
             * of this function should call setBuffer() and initializeMatrix().
             * 
             * \since 0.2
             */
            virtual void allocate(const unsigned int rows, const unsigned int cols, const runtime::Matrix::ValueType valueType) = 0;
            
            
            
        private:
            static const char NUMPY_MAGIC_BYTE = char(0x93);
            
            static void doSerialize(std::ostream & out, const runtime::Matrix & matrix);
            static void doDeserialize(std::istream & in, MatrixWrapper & matrix);
            static bool isLittleEndian();
            static char npyTypeSymbol(const runtime::Matrix::ValueType valueType);
            static Matrix::ValueType valueTypeFromNpyHeader(const char valueType, const int wordSize);
            
            void validate(const unsigned int rows,
                        const unsigned int cols,
                        const unsigned int stride,
                        const uint8_t* data,
                        const ValueType valueType) const;
                       
            unsigned int m_rows;
            unsigned int m_cols;
            unsigned int m_stride;
            unsigned int m_bufferSize;
            ValueType m_valueType;
            uint8_t* m_data;
            uint8_t* m_buffer;
            VariantHandle m_variant;
        };
    }
}

#endif // STROMX_RUNTIME_MATRIXWRAPPER_H
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

#ifndef STROMX_CORE_MATRIXWRAPPER_H
#define STROMX_CORE_MATRIXWRAPPER_H

#include "Matrix.h"

namespace stromx
{
        namespace core
    {
        /** \brief Concrete matrix without memory management. */
        class STROMX_CORE_API MatrixWrapper : public Matrix
        {
        public:
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
            
            // Implementation of stromx::core::Matrix
            virtual const DataVariant & variant() const { return m_variant; }
            virtual uint8_t* buffer() { return m_buffer; }
            virtual unsigned int bufferSize() const { return m_bufferSize; }
            virtual unsigned int rows() const { return m_rows; }
            virtual unsigned int cols() const { return m_cols; }
            virtual unsigned int stride() const { return m_stride; }
            virtual ValueType valueType() const { return m_valueType; }
            virtual unsigned int valueSize() const;
            virtual uint8_t* data() { return m_data; }
            virtual const uint8_t* data() const { return m_data; }
            virtual void initializeMatrix(const unsigned int rows, 
                                          const unsigned int cols, 
                                          const unsigned int stride, 
                                          uint8_t* data, 
                                          const ValueType valueType);
            
        protected:
            /** 
             * Sets a new matrix buffer. Note that the matrix data defined by dimensions of 
             * the matrix value type and data must always be contained in the matrix buffer.
             * 
             * \param buffer The new buffer. Note that this memory is \em not released by
             *               the MatrixWrapper.
             * \param bufferSize The size of \a buffer in bytes.
             */
            virtual void setBuffer(uint8_t* const buffer, const unsigned int bufferSize);
            
        private:
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
            DataVariant m_variant;
        };
    }
}

#endif // STROMX_CORE_MATRIXWRAPPER_H
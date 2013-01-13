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

#include "Exception.h"
#include "MatrixWrapper.h"

namespace stromx
{
    namespace runtime
    {
        MatrixWrapper::MatrixWrapper(const unsigned int bufferSize, uint8_t* const buffer)
          : m_rows(0),
            m_cols(0),
            m_stride(0),
            m_bufferSize(bufferSize),
            m_valueType(NONE),
            m_data(0),
            m_buffer(buffer),
            m_variant(DataVariant::IMAGE)
        {
        }
        
        MatrixWrapper::MatrixWrapper()
          : m_rows(0),
            m_cols(0),
            m_stride(0),
            m_bufferSize(0),
            m_valueType(NONE),
            m_data(0),
            m_buffer(0),
            m_variant(DataVariant::IMAGE)
        {
        }

        void MatrixWrapper::setBuffer(uint8_t*const buffer, const unsigned int bufferSize)
        {
            m_bufferSize = bufferSize;
            m_buffer = buffer;
        }
        
        unsigned int MatrixWrapper::valueSize() const
        {
            return Matrix::valueSizeFromValueType(m_valueType);
        }
        
        void MatrixWrapper::initializeMatrix(const unsigned int rows, const unsigned int cols,
                                             const unsigned int stride, uint8_t* data, 
                                             const ValueType valueType) 
        {
            validate(rows, cols, stride, data, valueType);
            
            m_rows = rows;
            m_cols = cols;
            m_stride = stride;
            m_data = data;
            m_valueType = valueType;
            m_variant = dataVariantFromValueType(valueType);
        }
        
        void MatrixWrapper::validate(const unsigned int rows,
                                     const unsigned int cols,
                                     const unsigned int stride,
                                     const uint8_t*const data,
                                     const ValueType valueType) const
        {
            if(cols == 0 || rows == 0)
                return;
            
            // check row length
            if(cols * Matrix::valueSizeFromValueType(valueType) > stride)
                throw WrongArgument("Too small stride.");
            
            // check total data size
            unsigned int dataSize = stride * (rows - 1) + cols;
            if(data + dataSize > m_buffer + m_bufferSize)
                throw WrongArgument("Too small buffer.");
        }
    }
}

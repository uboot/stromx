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

#ifndef STROMX_RUNTIME_MATRIX_H
#define STROMX_RUNTIME_MATRIX_H

#include <stdint.h>
#include "stromx/runtime/Data.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief Abstract image. */
        class STROMX_RUNTIME_API Matrix : public Data
        {
        public:
            enum ValueType
            {
                NONE,
                INT_8,
                UINT_8,
                INT_16,
                UINT_16,
                INT_32,
                UINT_32,
                FLOAT,
                DOUBLE
            };
            
            virtual ~Matrix() {}

            /** 
             * Returns the address of the buffer of the matrix. This is not necessarily
             * a pointer to the matrix data which can be located at a different position
             * in the buffer.
             */
            virtual uint8_t* buffer() = 0;
            
            /** Returns the size of the buffer. */
            virtual unsigned int bufferSize() const = 0;
            
            /** Returns the number of rows of the matrix. */
            virtual unsigned int rows() const = 0;
            
            /** Returns the number columns of the matrix. */
            virtual unsigned int cols() const = 0;
            
            /** Returns the size of a row in the matrix in bytes. */
            virtual unsigned int stride() const = 0;
            
            /** Returns the value type of the data of the matrix. */
            virtual ValueType valueType() const = 0;
            
            /** Returns the size of a single value in bytes. */
            virtual unsigned int valueSize() const;
            
            /** Returns the address of the matrix data. */
            virtual uint8_t* data() = 0;
            
            /** Returns the address of the matrix data as a constant pointer. */
            virtual const uint8_t* data() const = 0;
            
            /** 
             * Initializes the matrix to the given data. Note that this function does not
             * change the matrix buffer but merely changes the description of the data
             * contained in the matrix buffer.
             */
            virtual void initializeMatrix(const unsigned int rows, 
                                          const unsigned int cols, 
                                          const unsigned int stride, 
                                          uint8_t* data, 
                                          const ValueType valueType) = 0;
                                     
            /** Returns the size of a single value of type \c valueType in bytes. */  
            static unsigned int valueSize(const ValueType valueType);  
            
        protected:
            
            /** Returns the data variant defined by the input \c valueType. */
            static const runtime::DataVariant dataVariantFromValueType(const ValueType valueType);
        };
        
        /** \cond */
        template <>
        class data_traits<Matrix>
        {
        public:
            static const DataVariant & variant() { return DataVariant::MATRIX; }
        };
        /** \endcond */
    }
}

#endif // STROMX_RUNTIME_MATRIX_H
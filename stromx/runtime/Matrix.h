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
                FLOAT_32,
                FLOAT_64
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
            
            /** 
             * Returns a reference of type \c T to the value at the position (\c row, \c col).
             * \throws WrongArgument If the size of \c T does not match the value size 
             *                       of the matrix or if the given position is not within 
             *                       the matrix bounds.
             **/
            template <class T>
            T & at(const unsigned row, const unsigned col)
            {
                const Matrix & mat = const_cast<const Matrix &>(*this);
                
                return const_cast<T &>(mat.at<T>(row, col));
            }
            
            /** 
             * Returns a constant reference of type \c T to the value at the position
             * (\c row, \c col).
             * \throws WrongArgument If the size of \c T does not match the value size 
             *                       of the matrix or if the given position is not within 
             *                       the matrix bounds.
             **/
            template <class T>
            const T & at(const unsigned int row, const unsigned col) const
            {
                if(sizeof(T) != valueSize())
                    throw WrongArgument("Size of access type does not match value size.");
                
                if(row >= rows())
                    throw WrongArgument("Row index out of bounds.");
                
                if(col >= cols())
                    throw WrongArgument("Column index out of bounds.");
                
                unsigned int byteOffset = row * stride() + col * valueSize();
                
                return *(reinterpret_cast<const T*>(data() + byteOffset));
            }
            
            /** 
             * Returns a copy of type \c T of the value at the position
             * (\c row, \c col). The value is casted to \c T by a static cast.
             **/
            template <class T>
            const T value(const unsigned int row, const unsigned int col) const
            {
                switch(valueType())
                {
                case UINT_8:
                    return static_cast<T>(at<uint8_t>(row, col));
                case INT_8:
                    return static_cast<T>(at<int8_t>(row, col));
                case UINT_16:
                    return static_cast<T>(at<uint16_t>(row, col));
                case INT_16:
                    return static_cast<T>(at<int16_t>(row, col));
                case UINT_32:
                    return static_cast<T>(at<uint32_t>(row, col));
                case INT_32:
                    return static_cast<T>(at<int32_t>(row, col));
                case FLOAT_32:
                    return static_cast<T>(at<float>(row, col));
                case FLOAT_64:
                    return static_cast<T>(at<double>(row, col));
                default:
                    throw InternalError("Unknown value type.");
                };
            }
            
        protected:
            
            /** Returns the data variant defined by the input \c valueType. */
            static const runtime::VariantHandle dataVariantFromValueType(const ValueType valueType);
        };
        
        /** 
         * Returns true if the sizes, value types and entries of both matrices
         * are equal.
         */
        STROMX_RUNTIME_API bool operator==(const Matrix & rhs, const Matrix & lhs);
        
        /**  Returns true if the matrices are not equal. */
        STROMX_RUNTIME_API bool operator!=(const Matrix & rhs, const Matrix & lhs);
        
        /** \cond */
        template <>
        class data_traits<Matrix>
        {
        public:
            static const VariantHandle & variant() { return Variant::MATRIX; }
        };
        /** \endcond */
    }
}

#endif // STROMX_RUNTIME_MATRIX_H
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

namespace stromx
{
    namespace core
    {
        const unsigned int Matrix::valueSizeFromValueType(const stromx::core::Matrix::ValueType valueType)
        {
            switch(valueType)
            {
            case core::Matrix::NONE:
            case core::Matrix::INT_8:
            case core::Matrix::UINT_8:
                return 1;
            case core::Matrix::INT_16:
            case core::Matrix::UINT_16:
                return 2;
            case core::Matrix::INT_32:
            case core::Matrix::UINT_32:
            case core::Matrix::FLOAT:
                return 4;
            case core::Matrix::DOUBLE:
                return 8;
            default:
                throw core::WrongArgument("Unknown value type.");    
            }
        }
        
        const core::DataVariant Matrix::dataVariantFromValueType(const ValueType valueType)
        {
            switch(valueType)
            {
            case core::Matrix::NONE:
                return core::DataVariant::MATRIX;
            case core::Matrix::INT_8:
                return core::DataVariant::INT_8_MATRIX;
            case core::Matrix::INT_16:
                return core::DataVariant::INT_16_MATRIX;
            case core::Matrix::INT_32:
                return core::DataVariant::INT_32_MATRIX;
            case core::Matrix::UINT_8:
                return core::DataVariant::INT_8_MATRIX;
            case core::Matrix::UINT_16:
                return core::DataVariant::UINT_16_MATRIX;
            case core::Matrix::UINT_32:
                return core::DataVariant::UINT_32_MATRIX;
            case core::Matrix::FLOAT:
                return core::DataVariant::FLOAT_MATRIX;
            case core::Matrix::DOUBLE:
                return core::DataVariant::DOUBLE_MATRIX;
            default:
                throw core::WrongArgument("Unknown pixel type.");  
            }
        }
    }
}
           
            
            
            
            
            
            

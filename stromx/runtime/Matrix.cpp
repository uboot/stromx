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

#include "stromx/runtime/Matrix.h"

namespace stromx
{
    namespace runtime
    {
        unsigned int Matrix::valueSizeFromValueType(const stromx::runtime::Matrix::ValueType valueType)
        {
            switch(valueType)
            {
            case runtime::Matrix::NONE:
            case runtime::Matrix::INT_8:
            case runtime::Matrix::UINT_8:
                return 1;
            case runtime::Matrix::INT_16:
            case runtime::Matrix::UINT_16:
                return 2;
            case runtime::Matrix::INT_32:
            case runtime::Matrix::UINT_32:
            case runtime::Matrix::FLOAT:
                return 4;
            case runtime::Matrix::DOUBLE:
                return 8;
            default:
                throw runtime::WrongArgument("Unknown value type.");    
            }
        }
        
        const runtime::DataVariant Matrix::dataVariantFromValueType(const ValueType valueType)
        {
            switch(valueType)
            {
            case runtime::Matrix::NONE:
                return runtime::DataVariant::MATRIX;
            case runtime::Matrix::INT_8:
                return runtime::DataVariant::INT_8_MATRIX;
            case runtime::Matrix::INT_16:
                return runtime::DataVariant::INT_16_MATRIX;
            case runtime::Matrix::INT_32:
                return runtime::DataVariant::INT_32_MATRIX;
            case runtime::Matrix::UINT_8:
                return runtime::DataVariant::INT_8_MATRIX;
            case runtime::Matrix::UINT_16:
                return runtime::DataVariant::UINT_16_MATRIX;
            case runtime::Matrix::UINT_32:
                return runtime::DataVariant::UINT_32_MATRIX;
            case runtime::Matrix::FLOAT:
                return runtime::DataVariant::FLOAT_MATRIX;
            case runtime::Matrix::DOUBLE:
                return runtime::DataVariant::DOUBLE_MATRIX;
            default:
                throw runtime::WrongArgument("Unknown pixel type.");  
            }
        }
    }
}
           
            
            
            
            
            
            

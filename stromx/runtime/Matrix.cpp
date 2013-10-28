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

#include <string.h>

namespace stromx
{
    namespace runtime
    {
        unsigned int Matrix::valueSize() const
        {
            return valueSize(valueType());
        }
        
        unsigned int Matrix::valueSize(const stromx::runtime::Matrix::ValueType valueType)
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
            case runtime::Matrix::FLOAT_32:
                return 4;
            case runtime::Matrix::FLOAT_64:
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
            case runtime::Matrix::FLOAT_32:
                return runtime::DataVariant::FLOAT_32_MATRIX;
            case runtime::Matrix::FLOAT_64:
                return runtime::DataVariant::FLOAT_64_MATRIX;
            default:
                throw runtime::WrongArgument("Unknown pixel type.");  
            }
        }
        
        bool operator==(const Matrix& rhs, const Matrix& lhs)
        {
            if (rhs.valueType() != lhs.valueType())
                return false;
            
            if (rhs.rows() != lhs.rows())
                return false;
            
            if (rhs.cols() != lhs.cols())
                return false;
            
            for (unsigned int i = 0; i < rhs.rows(); ++i)
            {
                const uint8_t* lhsRowPtr = lhs.data() + i * lhs.stride();
                const uint8_t* rhsRowPtr = rhs.data() + i * rhs.stride();
                unsigned int rowBytes = rhs.cols() * rhs.valueSize();
                int result = memcmp(lhsRowPtr, rhsRowPtr, rowBytes);
                
                if (result)
                    return false;
            }
            
            return true;
        }
        
        bool operator!=(const Matrix& rhs, const Matrix& lhs)
        {
            return ! (rhs == lhs);
        }
    }
}
           
            
            
            
            
            
            

/* 
*  Copyright 2013 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_MATRIXDESCRIPTION_H
#define STROMX_RUNTIME_MATRIXDESCRIPTION_H

#include "stromx/runtime/Parameter.h"
#include "stromx/runtime/Exception.h"

namespace stromx
{
    namespace runtime
    {
        class Data;
        
        /** 
         * \brief %Description of a matrix description 
         * 
         * \since 0.2
         */
        class MatrixDescription : public Description
        {
        public:
            /** 
             * Constructs a numeric parameter. 
             * \throws WrongArgument If \c variant is not a matrix variant.
             */
            MatrixDescription(const unsigned int id, const VariantHandle& variant)
              : Description(id, variant),
                m_rows(0),
                m_cols(0)
            {
                if(! variant.isVariant(Variant::MATRIX))
                    throw WrongArgument("The variant of a matrix description must be a matrix variant.");
            }
            
            virtual unsigned int rows() const { return m_rows; }
            virtual unsigned int cols() const { return m_cols; }
            
            /** 
             * Sets the required number of matrix rows of this parameter. 
             * Passing 0 for \c value allows any number of rows.
             */
            virtual void setRows(const unsigned int value)
            {
                m_rows = value;
            }
            
            /** 
             * Sets the required number of matrix rows of this parameter. 
             * Passing 0 for \c value allows any number of rows.
             */
            virtual void setCols(const unsigned int value)
            {
                m_cols = value;
            }
            
        private:
            unsigned int m_rows;
            unsigned int m_cols;
        };
    }
}

#endif // STROMX_RUNTIME_MATRIXDESCRIPTION_H

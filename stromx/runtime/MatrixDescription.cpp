/* 
*  Copyright 2016 Matthias Fuchs
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

#include "stromx/runtime/MatrixDescription.h"

#include "stromx/runtime/Exception.h"
#include "stromx/runtime/Variant.h"

namespace stromx
{
    namespace runtime
    {
        MatrixDescription::MatrixDescription(const unsigned int id, const VariantHandle& variant)
          : Description(id, variant),
            m_rows(0),
            m_cols(0)
        {
            if(! variant.isVariant(Variant::MATRIX))
                throw WrongArgument("The variant of a matrix description must be a matrix variant.");
        }
    }
}

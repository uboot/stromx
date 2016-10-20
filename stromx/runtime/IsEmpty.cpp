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

#include "stromx/runtime/IsEmpty.h"

#include "stromx/runtime/Locale.h"
#include "stromx/runtime/Matrix.h"
#include "stromx/runtime/Variant.h"

namespace stromx
{
    namespace runtime
    {
        const std::string IsEmpty::TITLE("IsEmpty");
        
        IsEmpty::IsEmpty()
          : MatrixPropertyBase(TITLE)
        {
        }
        
        Data* IsEmpty::getProperty(const Matrix & matrix)
        {
            return new Bool(matrix.rows() == 0 || matrix.cols() == 0);
        }
        
        Output* IsEmpty::createOutput()
        {
            Output* output = new Output(PROPERTY, Variant::BOOL);
            output->setTitle(L_("Is empty"));
            return output;
        }
    } 
}

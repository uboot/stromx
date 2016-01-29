/* 
*  Copyright 2015 Matthias Fuchs
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

#ifndef STROMX_CVML_H
#define STROMX_CVML_H

#include <locale>

#include "stromx/cvml/Config.h"

namespace stromx
{
    namespace runtime
    {
        class Registry;
    }
    
    namespace cvml
    {
        extern std::locale locale;
    }
}

extern "C"
{
    STROMX_CVML_API int stromxRegisterCvml(stromx::runtime::Registry* registry);
}

#endif // STROMX_CVML_H

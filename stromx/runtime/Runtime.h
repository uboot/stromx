/* 
*  Copyright 2011 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_RUNTIME_H
#define STROMX_RUNTIME_RUNTIME_H

#include <locale>

#include "stromx/runtime/Config.h"
#include "stromx/runtime/Version.h"

namespace stromx
{
    namespace runtime
    {
        class Registry;
        extern std::locale locale;
        
        /** 
         * \brief Returns the version of the stromx runtime library.
         * 
         * \since 0.2
         */
        STROMX_RUNTIME_API Version version();
    }
}

extern "C"
{
    STROMX_RUNTIME_API void stromxRuntimeRegister(stromx::runtime::Registry& registry);
}

#endif // STROMX_RUNTIME_RUNTIME_H

/* 
*  Copyright 2014 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_LOCALE_H
#define STROMX_RUNTIME_LOCALE_H

#include <locale>
#include "stromx/runtime/Config.h"

#define L_(id) stromx::runtime::Locale::gettext(id, locale)

namespace stromx
{
    namespace runtime
    {
        extern std::locale locale;
        
        class STROMX_RUNTIME_API Locale
        {
        public:
            static std::string gettext(const char* const id, const std::locale & locale);
            static std::locale generate(const char* const path, const char* const domain);
        };
    }
}

#endif // STROMX_RUNTIME_LOCALE_H

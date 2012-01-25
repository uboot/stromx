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

#ifndef STROMX_CORE_OUTPUTPROVIDER_H
#define STROMX_CORE_OUTPUTPROVIDER_H

#include <ostream>

namespace stromx
{
    namespace core
    {
        class OutputProvider
        {
        public:
            enum OpenMode { BINARY, TEXT };
            
            virtual std::ostream & text() = 0;
            virtual std::ostream & openFile(const std::string & ext = "", const OpenMode mode = BINARY) = 0;
            virtual std::ostream & file() = 0;
        };
    }
}

#endif // STROMX_CORE_OUTPUTPROVIDER_H

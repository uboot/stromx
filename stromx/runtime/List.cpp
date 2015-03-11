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

#include "stromx/runtime/List.h"

#include "stromx/runtime/InputProvider.h"
#include "stromx/runtime/OutputProvider.h"
#include "stromx/runtime/Version.h"

namespace stromx
{
    namespace runtime
    {
        const std::string List::TYPE = "List";
        const std::string List::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        const Version List::VERSION = Version(0, 1, 0);
        
        List::~List()
        {
            for(std::vector<Data*>::iterator iter = m_content.begin();
                iter != m_content.end(); ++iter)
            {
                delete *iter;
            }
        }
        
        const  VariantHandle & List::variant() const 
        {
            return Variant::LIST;
        }
        
        Data* List::clone() const
        {
            throw NotImplemented();
        }
    }
}


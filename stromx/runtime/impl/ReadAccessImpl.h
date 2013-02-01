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

#ifndef STROMX_RUNTIME_IMPL_READACCESSIMPL_H
#define STROMX_RUNTIME_IMPL_READACCESSIMPL_H

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Runtime.h"

namespace stromx
{
    namespace runtime
    {    
        class Data;
        
        namespace impl
        {
            class STROMX_RUNTIME_API ReadAccessImpl
            {
            public:
                ReadAccessImpl(const DataContainer& data, const bool waitWithTimeout, const unsigned int timeout = 0);
                
                ~ReadAccessImpl();
                
                const Data & get() const;
                
            private:
                DataContainer m_data;
            };
        }
    }
}

#endif // STROMX_RUNTIME_IMPL_READACCESSIMPL_H

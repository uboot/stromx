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

#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/impl/ReadAccessImpl.h"

namespace stromx
{
    namespace runtime
    {

        ReadAccess::ReadAccess(const DataContainer & data)
          : m_impl(new impl::ReadAccessImpl(data, false))
        {
        }
        
        ReadAccess::ReadAccess(const DataContainer & data, const unsigned int timeout)
          : m_impl(new impl::ReadAccessImpl(data, true, timeout))
        {
        }
        
        const Data & ReadAccess::get() const
        {
            if(empty())
                throw AccessEmpty();
            
            return m_impl->get();
        }
        
        void ReadAccess::release()
        {
            m_impl.reset();
        }
    }
}


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

#include "DataContainerImpl.h"
#include "ReadAccessImpl.h"
#include "../DataContainer.h"
#include "../Exception.h"



namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            ReadAccessImpl::ReadAccessImpl(const DataContainer& data, const bool waitWithTimeout, const unsigned int timeout)
              : m_data(data)
            {
                if(data.empty())
                    throw WrongArgument("Data container is empty.");
                
                m_data.m_impl->getReadAccess(waitWithTimeout, timeout);
            }

            const Data & ReadAccessImpl::get() const
            {
                return *m_data.m_impl->data();
            }
            
            ReadAccessImpl::~ReadAccessImpl()
            {
                m_data.m_impl->returnReadAccess();
            }
        }
    } 
}

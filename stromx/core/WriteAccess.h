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

#ifndef STROMX_CORE_WRITEACCESS_H
#define STROMX_CORE_WRITEACCESS_H

#include <tr1/memory>
#include "DataContainer.h"
#include "Exception.h"
#include "impl/WriteAccessImpl.h"

namespace stromx
{
        namespace core
    {
        class Data;
        class DataContainer;
        
        /** \brief Write access to a data container */
        template<typename data_t = Data>
        class WriteAccess
        {
        public:
            WriteAccess(DataContainer data)
            : m_impl(new impl::WriteAccessImpl(data))
            {
            }
            
            data_t & get()
            {
                try
                {
                    return dynamic_cast<data_t &>((*m_impl)());
                }
                catch(std::bad_cast &)
                {
                    throw BadCast();
                }
            }
            
            data_t & operator()() { return get(); }
            
        private:
            WriteAccess();

            std::tr1::shared_ptr<impl::WriteAccessImpl> m_impl;
        };
    }
}

#endif // STROMX_CORE_WRITEACCESS_H

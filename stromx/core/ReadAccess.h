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

#ifndef STROMX_CORE_READACCESS_H
#define STROMX_CORE_READACCESS_H

#include <memory>
#include "Data.h"
#include "DataContainer.h"
#include "Exception.h"
#include "impl/ReadAccessImpl.h"

namespace stromx
{
    namespace core
    {
        /** 
         *\brief Read access to a data container.
         *
         * A read access allows to read the content of a data container
         * which contains data of the type \c data_t.
         */
        template<typename data_t = Data>
        class ReadAccess
        {
        public:
            /** Constructs an empty read access. */ 
            ReadAccess() {}
            
            /** 
             * Constructs a read access from a data container. This functions
             * waits until read access is possible, i.e. until no
             * write access  to \c data exists.
             * 
             * \param data The container which contains the data to be read.
             */
            ReadAccess(const DataContainer & data)
              : m_impl(new impl::ReadAccessImpl(data, false))
            {
            }
            
            /** 
             * Constructs a read access from a data container. This functions
             * waits until read access is possible, i.e. until no
             * write access  to \c data exists, or until the timeout expires.
             * 
             * \param data The container which contains the data to be read.
             * \param timeout The maximal time to wait in milliseconds.
             * 
             * \throws Timeout If no read access could be obtained during the timeout.
             */
            ReadAccess(const DataContainer & data, const unsigned int timeout)
              : m_impl(new impl::ReadAccessImpl(data, true, timeout))
            {
            }
            
            /** Returns \c true if the read access is empty. */
            bool empty() const { return m_impl.get() == 0; }
            
            /** 
             * Returns a constant reference to the content of the data container.
             * 
             * \throws EmptyAccess If the read access is empty.
             */
            const data_t & get() const
            {
                if(empty())
                    throw AccessEmpty();
                
                try
                {
                    return dynamic_cast<const data_t &>(m_impl->get());
                }
                catch(std::bad_cast &)
                {
                    throw BadCast();
                }
            }
            
            /** Returns a constant reference to the content of the data container. */
            const data_t & operator()() const { return get(); }
            
        private:
            std::tr1::shared_ptr<impl::ReadAccessImpl> m_impl;
        };
    }
}

#endif // STROMX_CORE_READACCESS_H

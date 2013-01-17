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

#ifndef STROMX_RUNTIME_WRITEACCESS_H
#define STROMX_RUNTIME_WRITEACCESS_H

#ifdef __GNUG__
    #include <tr1/memory>
#else
    #include <memory>
#endif

#include "Data.h"
#include "DataContainer.h"
#include "Exception.h"
#include "impl/WriteAccessImpl.h"

namespace stromx
{
    namespace runtime
    {
        /** 
         * \brief Write access to a data container.
         * 
         * A read access allows to read and change the content of a data container
         * which contains data of the type \c data_t.
         */
        template<typename data_t = Data>
        class WriteAccess
        {
        public:
            /** Constructs an empty write access. */ 
            WriteAccess() {}
            
            /** 
             * Constructs a write access from a data container. This functions
             * waits until write access is possible, i.e. until no other
             * read or write access  to \c data exists.
             * 
             * \param data The container which contains the data to be accessed.
             */
            WriteAccess(const DataContainer & data)
              : m_impl(new impl::WriteAccessImpl(data, false))
            {
            }
            
            /** 
             * Constructs a write access from a data container. This functions
             * waits until write access is possible, i.e. until no other
             * read or write access  to \c data exists.
             * 
             * \param data The container which contains the data to be accessed.
             * \param timeout The maximal time to wait in milliseconds.
             * 
             * \throws Timeout If no read access could be obtained during the timeout.
             */
            WriteAccess(const DataContainer & data, const unsigned int timeout)
              : m_impl(new impl::WriteAccessImpl(data, true, timeout))
            {
            }
            
            /** Returns \c true if the write access is empty. */
            bool empty() const { return m_impl.get() == 0; }
            
            /** Returns a reference to the content of the data container. */
            data_t & get() const
            {
                if(empty())
                    throw AccessEmpty();
                
                try
                {
                    return dynamic_cast<data_t &>(m_impl->get());
                }
                catch(std::bad_cast &)
                {
                    throw BadCast();
                }
            }
            
            /** Returns a reference to the content of the data container. */
            data_t & operator()() const { return get(); }
            
        private:
            std::tr1::shared_ptr<impl::WriteAccessImpl> m_impl;
        };
    }
}

#endif // STROMX_RUNTIME_WRITEACCESS_H

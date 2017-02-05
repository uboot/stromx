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

#include "stromx/runtime/Data.h"
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Exception.h"

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            class WriteAccessImpl;
        }
        
        /** 
         * \brief Write access to a data container.
         * 
         * A read access allows to read and change the content of a data container
         * which contains data of the type \c data_t.
         */
        class STROMX_RUNTIME_API WriteAccess
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
             * \throws WrongArgument The container is read-only.
             */
            explicit WriteAccess(const DataContainer & data);
            
            /** 
             * Constructs a write access from a data container. This functions
             * waits until write access is possible, i.e. until no other
             * read or write access  to \c data exists.
             * 
             * \param data The container which contains the data to be accessed.
             * \param timeout The maximal time to wait in milliseconds.
             * 
             * \throws Timeout If no read access could be obtained during the timeout.
             * \throws WrongArgument The container is read-only.
             */
            WriteAccess(const DataContainer & data, const unsigned int timeout);
            
            /** Returns \c true if the write access is empty. */
            bool empty() const { return m_impl.get() == 0; }
            
            /** 
             * Returns a casted reference to the content of the data container.
             * 
             * \throws AccessEmpty If the read access is empty.
             * \throws BadCast If the data can not be casted to \c data_t.
             */
            template<typename data_t>
            data_t & get() const
            {
                return data_cast<data_t>(get());
            }
            
            /** 
             * Returns a reference to the content of the data container.
             * 
             * \throws AccessEmpty If the read access is empty.
             */
            Data & get() const;
            
            /** Releases the write access. The access is empty after calling this function. */
            void release();
            
        private:
            std::tr1::shared_ptr<impl::WriteAccessImpl> m_impl;
        };
    }
}

#endif // STROMX_RUNTIME_WRITEACCESS_H

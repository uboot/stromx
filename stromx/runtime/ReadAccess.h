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

#ifndef STROMX_RUNTIME_READACCESS_H
#define STROMX_RUNTIME_READACCESS_H

#include <memory>
#include "stromx/runtime/Data.h"
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Exception.h"

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            class ReadAccessImpl;
        }
        
        /** 
         *\brief Read access to a data container.
         *
         * A read access allows to read the content of a data container
         * which contains data of the type \c data_t.
         */
        class STROMX_RUNTIME_API ReadAccess
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
            explicit ReadAccess(const DataContainer & data);
            
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
            ReadAccess(const DataContainer & data, const unsigned int timeout);
            
            /** Returns \c true if the read access is empty. */
            bool empty() const { return m_impl.get() == 0; }
            
            /** 
             * Returns a casted constant reference to the content of the data container.
             * 
             * \throws EmptyAccess If the read access is empty.
             * \throws BadCast If the data can not be casted to \c data_t.
             */
            template<typename data_t>
            const data_t & get() const
            {
                return data_cast<data_t>(get());
            }
            
            /** 
             * Returns a constant reference to the content of the data container.
             * 
             * \throws EmptyAccess If the read access is empty.
             */
            const Data & get() const;
            
            /** Releases the recycle access. The access is empty after calling this function. */
            void release();
            
        private:
            std::tr1::shared_ptr<impl::ReadAccessImpl> m_impl;
        };
    }
}

#endif // STROMX_RUNTIME_READACCESS_H

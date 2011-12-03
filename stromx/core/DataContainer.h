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

#ifndef STROMX_CORE_DATACONTAINER_H
#define STROMX_CORE_DATACONTAINER_H

#include "Config.h"

#ifdef __GNUG__
    #include <tr1/memory>
#else
    #include <memory>
#endif

#include <ostream>
#include "Config.h"

namespace stromx
{
    namespace core
    {
        class Data;
        class DataVariant;
        
        namespace impl
        {
            class ReadAccessImpl;
            class WriteAccessImpl;
            class DataContainerImpl;
            class RecycleAccessImpl;
        }
        
        /** \brief Container which manages the life-cycle of data objects. */
        class STROMX_CORE_API DataContainer
        {
            friend class impl::WriteAccessImpl;
            friend class impl::ReadAccessImpl;
            friend class impl::RecycleAccessImpl;
            
            friend STROMX_CORE_API const bool operator==(const DataContainer & lhs, const DataContainer & rhs); 
            friend STROMX_CORE_API const bool operator!=(const DataContainer & lhs, const DataContainer & rhs); 
            friend STROMX_CORE_API std::ostream& operator<< (std::ostream& out, const DataContainer & container);
            
        public:
            /**
             * Constructs an empty data container.
             */
            DataContainer() {}
            
            /**
             * Constructs a data container holding \c data.
             * \param data A non-zero pointer to a Data object
             * \throws WrongArgument
             */
            explicit DataContainer(core::Data*const data);
            
            /** Returns whether to DataContainer holds any data. */
            const bool empty() const { return m_impl.get() == 0; }
            
        private:
            std::tr1::shared_ptr<impl::DataContainerImpl> m_impl;
        };     
        
        /** Returns \c true if the addresses in \c lhs and \c rhs are the same. */
        STROMX_CORE_API const bool operator==(const DataContainer & lhs, const DataContainer & rhs); 
        
        /** Returns \c true if the addresses in \c lhs and \c rhs are not the same. */
        STROMX_CORE_API const bool operator!=(const DataContainer & lhs, const DataContainer & rhs); 
        
        /** Returns the address of the data held by \c container. */
        STROMX_CORE_API std::ostream& operator<< (std::ostream& out, const DataContainer & container);
    }
}

#endif // STROMX_CORE_DATACONTAINER_H

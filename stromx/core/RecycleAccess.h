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

#ifndef STROMX_CORE_RECYCLEACCESS_H
#define STROMX_CORE_RECYCLEACCESS_H

#ifdef __GNUG__
    #include <tr1/memory>
#else
    #include <memory>
#endif

#include "DataContainer.h"
#include "Exception.h"

namespace stromx
{
    namespace core
    {
        namespace impl
        {
            class RecycleAccessImpl;
        }
        
        class Data;
        class DataContainer;
        
        /** 
         * \brief Recycle access to a data container.
         *
         * A recycle access allows to \em recycle data which is not used anymore.
         * In other words, it provides a mechanism to reclaim objects which are ready 
         * to be destroyed and to reuse the memory the occupy. This avoids frequent
         * allocation and deallocation of these objects.
         */
        class STROMX_CORE_API RecycleAccess
        {
        public:
            /**
             * Constructs a recycle access and adds data to it.
             * 
             * \param data This data will be recycled by this access.
             */
            RecycleAccess(const DataContainer & data);
            
            /** Constructs an empty recycle access. */
            RecycleAccess() {}
            
            /**
             * Adds data to the recycle access.
             * 
             * \param data This data will be recycled by this access.
             */
            void add(const DataContainer data);  
            
            /**
             * Returns true if the recycle access does not reference any data
             * which can be recycled.
             */
            bool empty() const;
            
            /**
             * Waits for data which has been added to the recycle access and 
             * is ready to be recycled, i.e. no other objects reference it.
             * Returns 0 if the recycle access does not reference any data
             * which can be recycled.
             */
            Data* operator()() const { return get(); }
            
            /**
             * Waits for data which has been added to the recycle access and 
             * is ready to be recycled, i.e. no other objects reference it.
             * Returns 0 if the recycle access does not reference any data
             * which can be recycled.
             */
            Data* get() const;  
            
            /**
             * Waits for data which has been added to the recycle access and 
             * is ready to be recycled, i.e. no other objects reference it.
             * Returns 0 if the recycle access does not reference any data
             * which can be recycled.
             * 
             * \param timeout The maximal time to wait in milliseconds.
             * \throws Timeout If no data was recycled during the timeout.
             */
            Data* operator()(const unsigned int timeout) const { return get(timeout); }
            
            /**
             * Waits for data which has been added to the recycle access and 
             * is ready to be recycled, i.e. no other objects reference it.
             * Returns 0 if the recycle access does not reference any data
             * which can be recycled.
             * 
             * \param timeout The maximal time to wait in milliseconds.
             * \throws Timeout If no data was recycled during the timeout.
             */
            Data* get(const unsigned int timeout) const;
            
        private:
            std::tr1::shared_ptr<impl::RecycleAccessImpl> m_impl;      
        };
    }
}

#endif // STROMX_CORE_RECYCLEACCESS_H

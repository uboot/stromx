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
        
        /** \brief Recycle access to a data container */
        class RecycleAccess
        {
        public:
            RecycleAccess(const DataContainer data);
            RecycleAccess() {}
            
            void add(const DataContainer data);  
            Data* const operator()() const { return get(); }
            Data* const get() const;  
            Data* const operator()(const unsigned int timeout) const { return get(timeout); }
            Data* const get(const unsigned int timeout) const;
            
        private:
            std::tr1::shared_ptr<impl::RecycleAccessImpl> m_impl;      
        };
    }
}

#endif // STROMX_CORE_RECYCLEACCESS_H

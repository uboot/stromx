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

#ifndef STROM_RECYCLEACCESS_H
#define STROM_RECYCLEACCESS_H

#include <tr1/memory>
#include "DataContainer.h"
#include "Exception.h"

namespace strom
{
    namespace impl
    {
        class RecycleAccessImpl;
    }
    
    class Data;
    class DataContainer;
    
    class RecycleAccess
    {
    public:
        RecycleAccess(const DataContainer data);
        RecycleAccess() {}
        
        void add(const DataContainer data);  
        Data* const operator()() const;  
        Data* const operator()(const unsigned int timeout) const;
        
    private:
        std::tr1::shared_ptr<impl::RecycleAccessImpl> m_impl;      
    };
}

#endif // STROM_RECYCLEACCESS_H

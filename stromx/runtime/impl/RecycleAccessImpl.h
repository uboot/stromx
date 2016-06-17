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

#ifndef STROMX_RUNTIME_IMPL_RECYCLEACCESSIMPL_H
#define STROMX_RUNTIME_IMPL_RECYCLEACCESSIMPL_H

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <deque>
#include <set>
#include "stromx/runtime/Recycler.h"

namespace stromx
{
    namespace runtime
    {
        class Data;
        class DataContainer;
    
        namespace impl
        {
            class DataContainerImpl;
        
            class RecycleAccessImpl : public Recycler
            {
        public:
                explicit RecycleAccessImpl(const DataContainer& data);
                ~RecycleAccessImpl();    
                
                void recycle(impl::DataContainerImpl* const container);
                Data* get(const bool waitWithTimeout, const unsigned int timeout = 0);
                void add(const DataContainer& data);
                bool empty() const;
                
            private:
                RecycleAccessImpl();
                
                typedef boost::lock_guard<boost::mutex> lock_t;
                typedef boost::unique_lock<boost::mutex> unique_lock_t;
                
                boost::mutex m_mutex;
                boost::condition_variable_any m_cond;

                std::deque<Data*> m_data;
                std::set<DataContainerImpl*> m_dataContainer;
            };
        }
    }
}

#endif // STROMX_RUNTIME_IMPL_RECYCLEACCESSIMPL_H

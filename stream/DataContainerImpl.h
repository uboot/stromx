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

#ifndef STREAM_DATACONTAINERIMPL_H
#define STREAM_DATACONTAINERIMPL_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace stream
{
    class Data;
    class Recycler;
    
    class DataContainerImpl
    {
    public:
        DataContainerImpl(Data* data);
        ~DataContainerImpl();
        
        const Data* const getReadAccess();
        void returnReadAccess();
        
        Data* const getWriteAccess();
        void returnWriteAccess();
        
        void getRecycleAccess(Recycler* const recycler);
        void returnRecycleAccess();
        
        void recycle();
        Data* const data() const { return m_data; }
        
    private:
        typedef boost::lock_guard<boost::mutex> lock_t;
        typedef boost::unique_lock<boost::mutex> unique_lock_t;
        
        boost::mutex m_mutex;
        boost::condition_variable_any m_cond;
        
        unsigned int m_readAccessCounter;
        bool m_writeAccess;
        Recycler* m_recycleAccess;
        Data* m_data;
    };
}

#endif // STREAM_DATACONTAINERIMPL_H

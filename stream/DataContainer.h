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

#ifndef STREAM_DATACONTAINER_H
#define STREAM_DATACONTAINER_H

#include <typeinfo>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include "Data.h"

namespace stream
{
    class DataOwner;
    class DataType;
    
    class DataContainer
    {
    public:
        DataContainer(stream::Data*const data, stream::DataOwner* const owner = 0);
        virtual ~DataContainer();
        
        const DataType & type() { return m_data->type(); }
        
        void reference();
        void dereference();

        const Data* const getReadAccess();
        Data* const getWriteAccess();
        void clearWriteAccess();
        
    private:
        typedef boost::lock_guard<boost::mutex> lock_t;
        typedef boost::unique_lock<boost::mutex> unique_lock_t;
        
        void destroy();
        
        boost::mutex m_mutex;
        boost::condition_variable_any m_cond;
        Data* m_data;
        DataOwner* m_owner;
        unsigned int m_refCount;
        bool m_hasWriteAccess;
    };     
}

#endif // STREAM_DATACONTAINER_H

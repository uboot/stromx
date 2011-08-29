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

#include <boost/shared_ptr.hpp>
#include "impl/DataContainerImpl.h"

namespace stream
{
    class Data;
    class DataVariant;
    
    namespace impl
    {
        class ReadAccessImpl;
        class RecycleAccessImpl;
        class WriteAccessImpl;
    }
    
    class DataContainer
    {
        friend class impl::WriteAccessImpl;
        friend class impl::ReadAccessImpl;
        friend class impl::RecycleAccessImpl;
        friend const bool operator==(const DataContainer & lhs, const DataContainer & rhs); 
        friend const bool operator!=(const DataContainer & lhs, const DataContainer & rhs); 
        friend std::ostream& operator<< (std::ostream& out, const DataContainer & container);
        
    public:
        DataContainer() {}
        explicit DataContainer(stream::Data*const data);
        
        const DataVariant & type();
        const bool empty() const { return m_impl.get() == 0; }
        
    private:
        boost::shared_ptr<impl::DataContainerImpl> m_impl;
    };     
    
    const bool operator==(const DataContainer & lhs, const DataContainer & rhs); 
    const bool operator!=(const DataContainer & lhs, const DataContainer & rhs); 
    std::ostream& operator<< (std::ostream& out, const DataContainer & container);
}

#endif // STREAM_DATACONTAINER_H

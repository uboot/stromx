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

#ifndef STREAM_READACCESS_H
#define STREAM_READACCESS_H

#include "ReadAccessImpl.h"

#include <boost/shared_ptr.hpp>

namespace stream
{
    class Data;
    
    class ReadAccess
    {
    public:
        ReadAccess(DataContainer data);
        
        const Data* const operator()();
        
    private:
        ReadAccess();
        boost::shared_ptr<ReadAccessImpl> m_impl;
    };
}

#endif // STREAM_READACCESS_H

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

#ifndef STREAM_READACCESSIMPL_H
#define STREAM_READACCESSIMPL_H

namespace stream
{    
    class DataContainer;
    class Data;
    
    class ReadAccessImpl
    {
    public:
        ReadAccessImpl(DataContainer& data);
        ~ReadAccessImpl();
        
        const Data & operator()();
        
    private:
        DataContainer& m_data;
    };
}

#endif // STREAM_READACCESSIMPL_H

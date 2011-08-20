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

#ifndef STREAM_IMPL_WRITEACCESSIMPL_H
#define STREAM_IMPL_WRITEACCESSIMPL_H

namespace stream
{
    class DataContainer;
    class Data;
    
    namespace impl
    {
        class WriteAccessImpl
        {
        public:
            WriteAccessImpl(DataContainer& data);
            ~WriteAccessImpl();
            
            Data& operator()() const;
            
        private:
            DataContainer& m_data;
        }; 
    }
}

#endif // STREAM_IMPL_WRITEACCESSIMPL_H

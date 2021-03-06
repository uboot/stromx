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

#ifndef STROMX_RUNTIME_TRY_H
#define STROMX_RUNTIME_TRY_H

#include "stromx/runtime/Config.h"
#include "stromx/runtime/Id2DataMapper.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief %Operator which renders ID-to-data maps optional.  */
        class STROMX_RUNTIME_API Try : public Id2DataMapper
        {
        public:
            explicit Try(const Id2DataMapper& mapper)
            : m_mapper(mapper)
            {}
            
            virtual bool trySet(const impl::Id2DataMap& id2DataMap) const;
            virtual bool tryGet(const impl::Id2DataMap& id2DataMap) const;
            virtual void get(impl::Id2DataMap& id2DataMap) const; 
            virtual void set(impl::Id2DataMap& id2DataMap) const;
            
        private:
            const Id2DataMapper& m_mapper;
        };
    }
}

#endif // STROMX_RUNTIME_TRY_H
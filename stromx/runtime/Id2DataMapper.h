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

#ifndef STROMX_RUNTIME_ID2DATAMAPPER_H
#define STROMX_RUNTIME_ID2DATAMAPPER_H

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            class Id2DataMap; 
        }
        
        /** \brief Abstract base class of ID-to-data mappers. */
        class Id2DataMapper
        { 
        public:
            virtual ~Id2DataMapper() {}
            
            virtual bool trySet(const impl::Id2DataMap& id2DataMap) const = 0;
            virtual bool tryGet(const impl::Id2DataMap& id2DataMap) const = 0;
            virtual void get(impl::Id2DataMap& id2DataMap) const = 0; 
            virtual void set(impl::Id2DataMap& id2DataMap) const = 0; 
        };
    }
}

#endif // STROMX_RUNTIME_ID2DATAMAPPER_H
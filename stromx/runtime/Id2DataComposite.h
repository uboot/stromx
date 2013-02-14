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

#ifndef STROMX_RUNTIME_ID2DATACOMPOSITE_H
#define STROMX_RUNTIME_ID2DATACOMPOSITE_H

#include "stromx/runtime/Config.h"
#include "stromx/runtime/Id2DataMapper.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief A composite expression of two ID-to-data mappers. */
        class STROMX_RUNTIME_API Id2DataComposite : public Id2DataMapper
        {
        public:
            enum NodeType
            {
                AND,
                OR
            };
            
            STROMX_RUNTIME_API friend const Id2DataComposite operator&&(const Id2DataMapper & lhs, const Id2DataMapper & rhs);
            STROMX_RUNTIME_API friend const Id2DataComposite operator||(const Id2DataMapper & lhs, const Id2DataMapper & rhs);
            
            Id2DataComposite(const Id2DataMapper& lhs, const Id2DataMapper& rhs, const NodeType type)
              : m_lhs(lhs),
                m_rhs(rhs),
                m_type(type)
            {}
            
            virtual bool trySet(const impl::Id2DataMap& id2DataMap) const;
            virtual bool tryGet(const impl::Id2DataMap& id2DataMap) const;
            virtual void get(impl::Id2DataMap& id2DataMap) const; 
            virtual void set(impl::Id2DataMap& id2DataMap) const;
            
        private:  
            Id2DataComposite();
            
            const Id2DataMapper& m_lhs;
            const Id2DataMapper& m_rhs;
            NodeType m_type;
        };
        
        /** Constructs an ID-to-data mapper which requires both \c lhs and \c rhs to succeed. */
        STROMX_RUNTIME_API const Id2DataComposite operator&&(const Id2DataMapper & lhs, const Id2DataMapper & rhs);
        
        /** Constructs an ID-to-data mapper which requires either \c lhs or \c rhs to succeed. */
        STROMX_RUNTIME_API const Id2DataComposite operator||(const Id2DataMapper & lhs, const Id2DataMapper & rhs);
    }
}

#endif // STROMX_RUNTIME_ID2DATACOMPOSITE_H
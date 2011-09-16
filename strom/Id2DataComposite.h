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

#ifndef STREAM_ID2DATACOMPOSITE_H
#define STREAM_ID2DATACOMPOSITE_H

#include "Id2DataMapper.h"

namespace strom
{
    class Id2DataComposite : public Id2DataMapper
    {
    private:
        enum NodeType
        {
            AND,
            OR
        };
        
    public:
        friend const Id2DataComposite operator&&(const Id2DataMapper & lhs, const Id2DataMapper & rhs);
        friend const Id2DataComposite operator||(const Id2DataMapper & lhs, const Id2DataMapper & rhs);
        
        Id2DataComposite(const Id2DataMapper& lhs, const Id2DataMapper& rhs, const NodeType type)
          : m_lhs(lhs),
            m_rhs(rhs),
            m_type(type)
        {}
        
        virtual const bool trySet(const impl::Id2DataMap& id2DataMap) const;
        virtual const bool tryGet(const impl::Id2DataMap& id2DataMap) const;
        virtual void get(impl::Id2DataMap& id2DataMap) const; 
        virtual void set(impl::Id2DataMap& id2DataMap) const;
        
    private:  
        const Id2DataMapper& m_lhs;
        const Id2DataMapper& m_rhs;
        NodeType m_type;
    };
    
    const Id2DataComposite operator&&(const Id2DataMapper & lhs, const Id2DataMapper & rhs);
    const Id2DataComposite operator||(const Id2DataMapper & lhs, const Id2DataMapper & rhs);

}

#endif // STREAM_ID2DATACOMPOSITE_H
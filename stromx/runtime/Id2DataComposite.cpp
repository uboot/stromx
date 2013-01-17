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

#include <boost/assert.hpp>
#include "Id2DataComposite.h"

namespace stromx
{
    namespace runtime
    {
        bool Id2DataComposite::trySet(const impl::Id2DataMap& id2DataMap) const
        {
            switch(m_type)
            {
            case AND:
                return m_lhs.trySet(id2DataMap) && m_rhs.trySet(id2DataMap);
            case OR:
                return m_lhs.trySet(id2DataMap) || m_rhs.trySet(id2DataMap);
            default:
                BOOST_ASSERT(false);
                return false;
            } 
        }
        
        bool Id2DataComposite::tryGet(const impl::Id2DataMap& id2DataMap) const
        {
            switch(m_type)
            {
            case AND:
                return m_lhs.tryGet(id2DataMap) && m_rhs.tryGet(id2DataMap);
            case OR:
                return m_lhs.tryGet(id2DataMap) || m_rhs.tryGet(id2DataMap);
            default:
                BOOST_ASSERT(false);
                return false;
            } 
        }
        
        void Id2DataComposite::get(impl::Id2DataMap& id2DataMap) const
        {
            switch(m_type)
            {
            case AND:
                m_lhs.get(id2DataMap);
                m_rhs.get(id2DataMap);
                break;
            case OR:
                if(m_lhs.tryGet(id2DataMap))
                    m_lhs.get(id2DataMap);
                else
                    m_rhs.get(id2DataMap);
                break;
            default:
                BOOST_ASSERT(false);
            }
        }
        
        void Id2DataComposite::set(impl::Id2DataMap& id2DataMap) const
        {
            switch(m_type)
            {
            case AND:
                m_lhs.set(id2DataMap);
                m_rhs.set(id2DataMap);
                break;
            case OR:
                if(m_lhs.trySet(id2DataMap))
                    m_lhs.set(id2DataMap);
                else
                    m_rhs.set(id2DataMap);
                break;
            default:
                BOOST_ASSERT(false);
            }
        }
        
        const Id2DataComposite operator&&(const Id2DataMapper & lhs, const Id2DataMapper & rhs)
        {
            return Id2DataComposite(lhs, rhs, Id2DataComposite::AND);
        }
        
        const Id2DataComposite operator||(const Id2DataMapper & lhs, const Id2DataMapper & rhs)  
        {
            return Id2DataComposite(lhs, rhs, Id2DataComposite::OR);
        }
    }
}

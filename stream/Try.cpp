#include "Try.h"

#include <boost/assert.hpp>

namespace stream
{
    const bool Try::trySet(const stream::impl::Id2DataMap& id2DataMap) const
    {
        return true;
    }

    const bool Try::tryGet(const stream::impl::Id2DataMap& id2DataMap) const
    {
        return true;
    }

    void Try::set(impl::Id2DataMap& id2DataMap) const
    {
        if(m_mapper.trySet(id2DataMap))
            m_mapper.set(id2DataMap);
    }

    void Try::get(impl::Id2DataMap& id2DataMap) const
    {
        if(m_mapper.tryGet(id2DataMap))
            m_mapper.get(id2DataMap);
    }
}

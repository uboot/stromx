#include "Id2DataMapper.h"

namespace stream
{
    bool trySet(const Id2DataMapper& mapper, const impl::Id2DataMap& id2DataMap)
    {
        return mapper.trySet(id2DataMap);
    }  
    
    bool tryGet(const Id2DataMapper& mapper, const impl::Id2DataMap& id2DataMap)
    {
        return mapper.tryGet(id2DataMap);
    }
    
    void get(const Id2DataMapper& mapper, impl::Id2DataMap& id2DataMap)
    {
        return mapper.get(id2DataMap);    
    }
    
    void set(const Id2DataMapper& mapper, impl::Id2DataMap& id2DataMap)
    {
        return mapper.set(id2DataMap);
    }
}

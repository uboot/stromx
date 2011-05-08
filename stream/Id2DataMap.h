#ifndef STREAM_ID2DATAMAP_H
#define STREAM_ID2DATAMAP_H


#include "Descriptor.h"

#include <vector>
#include <map>

namespace stream
{
    class DataContainer;
    
    class Id2DataMap
    {
    public:
        Id2DataMap(const std::vector<Descriptor> & ids);
        
        DataContainer* const operator[](const unsigned int id) const;
        DataContainer*& operator[](const unsigned int id);
        
    private:
        std::map<unsigned int, DataContainer*> m_map;
    };
}

#endif // STREAM_ID2DATAMAP_H

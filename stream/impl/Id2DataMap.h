#ifndef STREAM_IMPL_ID2DATAMAP_H
#define STREAM_IMPL_ID2DATAMAP_H


#include "../Description.h"

#include <vector>
#include <map>

namespace stream
{
    class DataContainer;
    
    namespace impl
    {
        class Id2DataMap
        {
        public:
            Id2DataMap();
            Id2DataMap(const std::vector<const Description*> & descriptions);
            
            DataContainer operator[](const unsigned int id) const;
            DataContainer& operator[](const unsigned int id);
            void clear();
            const bool isEmpty() const;
            
        private:
            std::map<unsigned int, DataContainer> m_map;
        };
    }
}

#endif // STREAM_IMPL_ID2DATAMAP_H

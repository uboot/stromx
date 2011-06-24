#include "DataContainer.h"

#include "Recycler.h"

namespace stream
{
    DataContainer::DataContainer(Data* const data)
    : m_impl(new DataContainerImpl(data))
    {
    }

    DataContainer::~DataContainer()
    {
        if(m_impl.unique())
            m_impl->recycle();
    }
}
 
#include "EnumParameter.h"

#include "Exception.h"

namespace stream
{   
    void EnumParameter::add(const stream::EnumDescription& description)
    {
        for(std::vector<EnumDescription>::const_iterator iter = m_descriptions.begin();
            iter != m_descriptions.end();
            ++iter)
        {
            if(iter->value() == description.value())
                throw ArgumentException("A description with this value has already been added.");
        }
        
        m_descriptions.push_back(description);
    }

    void EnumParameter::remove(const stream::EnumDescription& description)
    {
        for(std::vector<EnumDescription>::iterator iter = m_descriptions.begin();
            iter != m_descriptions.end();
            ++iter)
        {
            if(iter->value() == description.value())
                m_descriptions.erase(iter);
            
            return;
        }
        
        throw ArgumentException("No description with this value exists.");
    }
}

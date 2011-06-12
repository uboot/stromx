#ifndef STREAM_PARAMETER_H
#define STREAM_PARAMETER_H

#include "Description.h"

namespace stream
{
    class Parameter : public Description
    {
    public:
        enum AccessMode
        {
            NONE = 0x0,
            READ = 0x1,
            WRITE = 0x2,
            READ_WRITE = READ | WRITE
        };
        
        Parameter(const unsigned int id)
          : Description(id),
            m_inactiveAccess(NONE),
            m_activeAccess(NONE)
        {}
        
        void setInactiveAccessMode(const AccessMode mode) { m_inactiveAccess = mode; }
        void setActiveAccessMode(const AccessMode mode) { m_activeAccess = mode; }
        
    private:
        AccessMode m_inactiveAccess;
        AccessMode m_activeAccess;
    };
}

#endif // STREAM_PARAMETER_H
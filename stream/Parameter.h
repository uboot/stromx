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
            NO_ACCESS,
            NONE_READ,
            NONE_WRITE,
            INITIALIZED_READ,
            INITIALIZED_WRITE,
            ACTIVATED_READ,
            ACTIVATED_WRITE
        };
        
        Parameter(const unsigned int id, const DataType& type)
          : Description(id, type),
            m_access(NO_ACCESS)
        {}
        
        const AccessMode accessMode() const { return m_access; }
        
        void setAccessMode(const AccessMode mode) { m_access = mode; }
        
    private:
        AccessMode m_access;
    };
}

#endif // STREAM_PARAMETER_H
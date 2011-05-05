#ifndef STREAM_VERSION_H
#define STREAM_VERSION_H

#include "Parameter.h"

namespace stream
{
    class Version
    {
    public:
        Version(const unsigned int major, const unsigned int minor)
          : m_major(major),
            m_minor(minor)
        {}
        
    private:
        unsigned int m_major;
        unsigned int m_minor;
    };
}

#endif // STREAM_VERSION_H
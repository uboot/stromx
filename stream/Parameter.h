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
        
        Parameter(const std::string & name, const unsigned int id, const DataType & type,
                  const AccessMode inactiveAccess, const AccessMode activeAccess)
          : Description(name, id, type),
            m_inactiveAccess(inactiveAccess),
            m_activeAccess(activeAccess)
        {}
        
    private:
        AccessMode m_inactiveAccess;
        AccessMode m_activeAccess;
    };
}

#endif // STREAM_PARAMETER_H
#ifndef STREAM_OPERATOR_H
#define STREAM_OPERATOR_H

#include "Parameter.h"
#include "Version.h"

namespace stream
{
    class Operator
    {
    public:
        Operator(const std::string & name, const Version & version);
        
    private:
        std::string m_name;
        Version m_version;
        
    };
}

#endif // STREAM_OPERATOR_H
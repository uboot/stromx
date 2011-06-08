#ifndef STREAM_OPERATORINFO_H
#define STREAM_OPERATORINFO_H

#include "Parameter.h"
#include "Version.h"

#include <vector>

namespace stream
{
    class OperatorInfo
    {
    public:
        virtual const std::string& name() const = 0;
        virtual const std::string& package() const = 0;
        virtual const Version& version() const = 0;
        virtual const std::vector<Description>& inputs() const = 0;
        virtual const std::vector<Description>& outputs() const = 0;
        virtual const std::vector<Parameter>& parameters() const = 0;
    };
}

#endif // STREAM_OPERATORINFO_H
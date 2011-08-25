/* 
 *  Copyright 2011 Matthias Fuchs
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

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
        virtual const std::string& type() const = 0;
        virtual const std::string& package() const = 0;
        virtual const Version& version() const = 0;
        virtual const std::vector<const Description*>& inputs() const = 0;
        virtual const std::vector<const Description*>& outputs() const = 0;
        virtual const std::vector<const Parameter*>& parameters() const = 0;
        virtual const Parameter & parameter(const unsigned int id) const = 0;
    };
}

#endif // STREAM_OPERATORINFO_H
/* 
 *  Copyright 2013 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_IMPL_SERIALIZATIONHEADER_H
#define STROMX_RUNTIME_IMPL_SERIALIZATIONHEADER_H

#include "stromx/runtime/Version.h"

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>

namespace boost {
    namespace serialization {
        template<class Archive>
        void serialize(Archive & ar, stromx::runtime::Version & v, const unsigned int /*classVersion*/)
        {
            ar & v.m_major;
            ar & v.m_minor;
            ar & v.m_revision;
        }
    }
}


namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            struct SerializationHeader
            {
                Version serverVersion;
                std::string package;
                std::string type;
                Version version;

                template <typename Archive>
                void serialize(Archive& ar, const unsigned int /*classVersion*/)
                {
                    ar & serverVersion;
                    ar & package;
                    ar & type;
                    ar & version;
                }
                
                const static unsigned int NUM_SIZE_DIGITS = 8;
                
            };
            
            bool operator==(const SerializationHeader & lhs, const SerializationHeader & rhs);
            std::ostream & operator<<(std::ostream & out, const SerializationHeader & header);
        }
    }
}

#endif // STROMX_RUNTIME_IMPL_SERIALIZATIONHEADER_H

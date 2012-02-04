/* 
*  Copyright 2011 Thomas Fidler
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

#ifndef STROMX_CORE_XMLWRITER_H
#define STROMX_CORE_XMLWRITER_H

#include <string>
#include <vector>
#include "Config.h"

namespace stromx
{
    namespace core
    {
        class FileOutput;
        class Operator;
        class Stream;
        
        /** \brief Writer for XML encoded streams. */
        class STROMX_CORE_API XmlWriter
        {
        public:
            void writeStream(const std::string& filepath, const Stream& stream) const;
            
            void writeStream(FileOutput & output, const std::string basename, const Stream& stream) const;
            
            void writeParameters(const std::string& filepath,
                                 const std::vector<const stromx::core::Operator*>& operators) const;
                                 
            void writeParameters(FileOutput & output, const std::string basename,
                                 const std::vector<const stromx::core::Operator*>& operators) const; 
        };
    }
}

#endif // STROMX_CORE_XMLWRITER_H

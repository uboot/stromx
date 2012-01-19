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

#ifndef STROMX_CORE_XMLREADER_H
#define STROMX_CORE_XMLREADER_H

#include <string>
#include "Config.h"

namespace stromx
{
    namespace core
    {
        class Factory;
        class Stream;
        
        /** \brief Reader for XML encoded streams. */
        class STROMX_CORE_API XmlReader
        {
        public:
            /** 
             * Reads a stream from an XML file.
             * 
             * \param filename The path of the file to be read.
             * \param factory The factory is used to instantiate the operators and data
             *                object in the stream. I.e. all required operator and data types
             *                must have been registered with the factory.
             */
            Stream* const readStream(const std::string & filename, const Factory& factory) const;
        };
    }
}

#endif // STROMX_CORE_XMLREADER_H

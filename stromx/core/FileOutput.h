/* 
*  Copyright 2012 Matthias Fuchs
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

#ifndef STROMX_CORE_FILEOUTPUT_H
#define STROMX_CORE_FILEOUTPUT_H

#include "OutputProvider.h"

namespace stromx
{
    namespace core
    {
        /** \brief Provides functions to initialize an output provider. */
        class FileOutput : public OutputProvider
        {
        public:
            /**
             * Initializes the output provider.
             * \param filename The name of the file which accepts the file data without file 
             *                 extension.
             */
            virtual void initialize(const std::string & filename) = 0;
            
            /**
             * Returns the name of the current file including the correct file extension.
             * \throws WrongState If no file has been opened by the client.
             */
            virtual const std::string & getFilename() const = 0;
            
            /**
             * Returns the text representation of the data if it has already been set.
             * Otherwise an empty string is returned.
             */
            virtual const std::string getText() const = 0;
            
            virtual ~FileOutput() {}
        };
    }
}

#endif // STROMX_CORE_FILEOUTPUT_H
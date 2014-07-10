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

#ifndef STROMX_RUNTIME_FILEINPUT_H
#define STROMX_RUNTIME_FILEINPUT_H

#include "stromx/runtime/InputProvider.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief Provides functions to initialize an input provider. */
        class FileInput : public InputProvider
        {
        public:
            /**
             * Initializes the file input to text and file data.
             * \param text The text data.
             * \param filename The name of the file which contains the file data. Pass an empty
             *                 string if no file should be provided.
             */
            virtual void initialize(const std::string & text, const std::string & filename = "") = 0;
            
            /**
             * Closes the file input. If this functions is not called the input is closed 
             * by the destructor.
             */
            virtual void close() = 0;
            
            virtual ~FileInput() {}
        };
    }
}

#endif // STROMX_RUNTIME_FILEINPUT_H

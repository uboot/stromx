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

#ifndef STROMX_CORE_OUTPUTPROVIDER_H
#define STROMX_CORE_OUTPUTPROVIDER_H

#include <ostream>

namespace stromx
{
    namespace core
    {
        class OutputProvider
        {
        public:
            /** The possible modes for file access. */
            enum OpenMode
            { 
                /** Access the file as binary file. */
                BINARY,
                /** Access the file as text file. */
                TEXT 
            };
            
            /** 
             * Returns an output stream which accepts the text representation of the data.
             * \throws WrongState If the input provider has not been correctly initialized.
             */
            virtual std::ostream & text() = 0;
            
            /** 
             * Opens the file representation of the data and returns the associated output stream.
             * \throws WrongState If the input provider has not been correctly initialized
             *                    or if this function has already been called.
             * \throws FileAccessFailed If the file could not be opened.
             */
            virtual std::ostream & openFile(const std::string & ext = "", const OpenMode mode = BINARY) = 0;
            
            /** 
             * Returns the output stream which accepts the file representation of the data.
             * \throws WrongState If the input provider has not been correctly initialized
             *                    or if openFile() has not been called before calling this function.
             */
            virtual std::ostream & file() = 0;
        };
    }
}

#endif // STROMX_CORE_OUTPUTPROVIDER_H

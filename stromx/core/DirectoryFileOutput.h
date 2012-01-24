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

#ifndef STROMX_CORE_DIRECTORYFILEOUTPUT_H
#define STROMX_CORE_DIRECTORYFILEOUTPUT_H

#include "FileOutput.h"
#include <fstream>
#include <sstream>

namespace stromx
{
    namespace core
    {
        class DirectoryFileOutput : public FileOutput
        {
        public:
            DirectoryFileOutput(const std::string & filename) {}
            virtual ~DirectoryFileOutput();
            
            virtual void selectFile(const std::string & filename);
            virtual void setText(const std::string & text);
            
            virtual std::ostream & text();
            virtual std::ostream & openFile(const std::string & ext, std::ios_base::openmode mode);
            virtual std::ostream & file();
        };
    }
}

#endif // STROMX_CORE_DIRECTORYFILEOUTPUT_H

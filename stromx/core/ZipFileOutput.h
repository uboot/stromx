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

#ifndef STROMX_CORE_ZIPFILEOUTPUT_H
#define STROMX_CORE_ZIPFILEOUTPUT_H

#include <sstream>
#include "Config.h"
#include "FileOutput.h"

struct zip;

namespace stromx
{
    namespace core
    {
        /** \brief File output which stores the output data in a zip file. */
        class STROMX_CORE_API ZipFileOutput : public FileOutput
        {
        public:
            /** Constructs a directory file output which stores all files in \c archive. */
            ZipFileOutput(const std::string & archive);
            virtual ~ZipFileOutput();
            
            virtual void initialize(const std::string & filename);
            virtual const std::string & getFilename() const;
            virtual const std::string getText() const;
            
            virtual std::ostream & text();
            virtual std::ostream & openFile(const std::string & ext, const OpenMode mode);
            virtual std::ostream & file();
            
        private:
            zip* m_archive;
            bool m_initialized;
            std::ostringstream m_fileOutput;
        };
    }
}

#endif // STROMX_CORE_ZIPFILEOUTPUT_H

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

#ifndef STROMX_RUNTIME_ZIPFILEINPUT_H
#define STROMX_RUNTIME_ZIPFILEINPUT_H

#include <sstream>
#include "stromx/runtime/Config.h"
#include "stromx/runtime/FileInput.h"

struct zip;

namespace stromx
{
    namespace runtime
    {
        /** \brief File input which reads the input files from a zip file. */
        class STROMX_RUNTIME_API ZipFileInput : public FileInput
        {
        public:
            /** 
             * Constructs a zip file input which reads files from a zip file.
             * 
             * \param archive The path to the zip file.
             * \throws FileAccessFailed If the zip file could not be opened.
             */
            explicit ZipFileInput(const std::string & archive);
            virtual ~ZipFileInput();
            
            virtual void initialize(const std::string & text, const std::string & filename);
            
            virtual std::istream & text();
            virtual bool hasFile() const;
            virtual std::istream & openFile(const OpenMode mode);
            virtual std::istream & file();
            virtual void close();
            
        private:
            zip* m_archiveHandle;
            bool m_initialized;
            std::string m_archive;
            std::string m_currentFilename;
            std::istringstream *m_currentFile;
            std::istringstream m_currentText;
        };
    }
}

#endif // STROMX_RUNTIME_ZIPFILEINPUT_H

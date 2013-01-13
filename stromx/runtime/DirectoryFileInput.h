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

#ifndef STROMX_RUNTIME_DIRECTORYFILEINPUT_H
#define STROMX_RUNTIME_DIRECTORYFILEINPUT_H

#include "Config.h"
#include "FileInput.h"
#include <fstream>
#include <sstream>

namespace stromx
{
    namespace runtime
    {
        /** \brief File input which reads the input files from a common directory. */
        class STROMX_RUNTIME_API DirectoryFileInput : public FileInput
        {
        public:
            /** 
             * Constructs a directory file input which reads files from a directory.
             * 
             * \param directory The path to the directory.
             */
            DirectoryFileInput(const std::string & directory) : m_initialized(false), m_directory(directory) {}
            virtual ~DirectoryFileInput();
            
            virtual void initialize(const std::string & text, const std::string & filename);
            
            virtual std::istream & text();
            virtual bool hasFile() const;
            virtual std::istream & openFile(const OpenMode mode);
            virtual std::istream & file();
            
        private:
            static const std::string PATH_SEPARATOR;
            
            bool m_initialized;
            std::string m_directory;
            std::string m_currentFilename;
            std::ifstream m_currentFile;
            std::istringstream m_currentText;
        };
    }
}

#endif // STROMX_RUNTIME_DIRECTORYFILEINPUT_H

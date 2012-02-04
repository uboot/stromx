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

#include "Config.h"
#include "FileOutput.h"
#include <fstream>
#include <sstream>

namespace stromx
{
    namespace core
    {
        /** \brief File output which stores the output data in single files in a common directory. */
        class STROMX_CORE_API DirectoryFileOutput : public FileOutput
        {
        public:
            /** Constructs a directory file output which stores all files in \c directory. */
            DirectoryFileOutput(const std::string & directory) : m_initialized(false), m_directory(directory) {}
            virtual ~DirectoryFileOutput();
            
            virtual void initialize(const std::string & filename);
            virtual const std::string & getFilename() const;
            virtual const std::string getText() const;
            
            virtual std::ostream & text();
            virtual std::ostream & openFile(const std::string & ext, const OpenMode mode);
            virtual std::ostream & file();
            
        private:
            static const std::string PATH_SEPARATOR;
            
            bool m_initialized;
            std::string m_directory;
            std::string m_currentBasename;
            std::string m_currentFilename;
            std::ofstream m_currentFile;
            std::ostringstream m_currentText;
        };
    }
}

#endif // STROMX_CORE_DIRECTORYFILEOUTPUT_H

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

#include "ZipFileInput.h"

#include <zip.h>
#include "Exception.h"

namespace stromx
{
    namespace core
    {
        ZipFileInput::ZipFileInput(const std::string& archive)
          : m_archiveHandle(0),
            m_initialized(false),
            m_archive(archive),
            m_currentFile(0)
        {
            int error = 0;
            m_archiveHandle = zip_open(m_archive.c_str(), 0, &error);
            
            if(! m_archiveHandle)
                throw FileAccessFailed(m_archive);
        }

        ZipFileInput::~ZipFileInput()
        {
            if(m_archiveHandle)
                zip_close(m_archiveHandle);
        }

        void ZipFileInput::initialize(const std::string& text, const std::string& filename)
        {
            m_currentText.clear();
            m_currentText.str(text);
            
            if(m_currentFile)
            {
                delete m_currentFile;
                m_currentFile = 0;
            }
            
            m_currentFilename = filename;
            m_initialized = true;
        }

        const bool ZipFileInput::hasFile() const
        {
            if(! m_initialized)
                throw WrongState("No current data in directory input.");
            
            return ! m_currentFilename.empty();
        }

        std::istream& ZipFileInput::text()
        {
            if(! m_initialized)
                throw WrongState("No current data in directory input.");
            
            return m_currentText;
        }

        std::istream& ZipFileInput::openFile(const stromx::core::InputProvider::OpenMode mode)
        {            
            if(! m_initialized)
                throw WrongState("No current data in directory input.");
            
            if(m_currentFile)
                throw WrongState("File has already been opened.");
            
            if(! hasFile())
                throw NoInputFile();
            
            std::ios_base::openmode iosmode;
            if(mode == BINARY)
                iosmode = std::ios_base::binary;
            
            struct zip_stat stat;
            if(zip_stat(m_archiveHandle, m_currentFilename.c_str(), 0, &stat) < 0)
                throw FileAccessFailed(m_archive);
            
            unsigned int fileSize = stat.size;
            char* content = new char[fileSize];
                
            zip_file* file = zip_fopen(m_archiveHandle, m_currentFilename.c_str(), 0);
            if(! file)
                throw FileAccessFailed(m_archive);
            
            if(zip_fread(file, content, fileSize) != fileSize)
                throw FileAccessFailed(m_archive, "Failed to read the file '" + m_currentFilename + "'.");
            
            m_currentFile = new std::istringstream(std::string(content, fileSize), std::ios_base::in & iosmode);
            
            return *m_currentFile;
        }

        std::istream& ZipFileInput::file()
        {
            if(! m_initialized)
                throw WrongState("No current data in directory input.");
            
            if(! m_currentFile)
                throw WrongState("File has not been opened.");
            
            return *m_currentFile;
        }
    }
}

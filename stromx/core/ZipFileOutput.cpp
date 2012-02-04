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

#include "ZipFileOutput.h"

#include <zip.h>
#include "Exception.h"

namespace stromx
{
    namespace core
    {
        ZipFileOutput::ZipFileOutput(const std::string& archive)
          : m_archiveHandle(0),
            m_initialized(false),
            m_archive(archive),
            m_currentFile(0)
        {
            int error = 0;
            m_archiveHandle = zip_open(m_archive.c_str(), ZIP_CREATE, &error);
            
            if(! m_archiveHandle)
                throw FileAccessFailed(m_archive);
            
            // delete all files in the archive
            int numFiles = zip_get_num_files(m_archiveHandle);
            for(int i = 0; i < numFiles; ++i)
            {
                if(zip_delete(m_archiveHandle, i) < 0)
                    throw FileAccessFailed(m_archive);
            }
        }

        ZipFileOutput::~ZipFileOutput()
        {
            try
            {
                dumpFile();
            }
            catch(FileAccessFailed &)
            {
                // ignore exceptions in destructor
            }
            
            if(m_archiveHandle)
                zip_close(m_archiveHandle);
        }

        void ZipFileOutput::initialize(const std::string& filename)
        {
            dumpFile();
            
            m_currentText.str("");
            m_currentBasename = filename;
            m_initialized = true;
            m_currentFilename = "";
        }

        const std::string& ZipFileOutput::getFilename() const
        {
            if(! m_initialized)
                throw WrongState("No current file in directory output.");
            
            return m_currentFilename;
        }

        const std::string ZipFileOutput::getText() const
        {
            return m_currentText.str();
        }

        std::ostream& ZipFileOutput::text()
        {
            if(! m_initialized)
                throw WrongState("No current file in directory output.");
            
            return m_currentText;
        }

        std::ostream& ZipFileOutput::openFile(const std::string& ext, const stromx::core::OutputProvider::OpenMode mode)
        {            
            if(! m_initialized)
                throw WrongState("No current file in directory output.");
            
            if(m_currentFile)
                throw WrongState("File has already been opened.");

            std::string filename;
            
            if(! ext.empty())
                m_currentFilename = m_currentBasename + "." + ext;
            else
                m_currentFilename = m_currentBasename;
            
            std::ios_base::openmode iosmode;
            if(mode == BINARY)
                iosmode = std::ios_base::binary;
            
            m_currentFile = new std::ostringstream(iosmode);
            
            return *m_currentFile;
        }

        std::ostream& ZipFileOutput::file()
        {
            if(! m_initialized)
                throw WrongState("No current file in directory output.");
            
            if(! m_currentFile)
                throw WrongState("File has not been opened.");
            
            return *m_currentFile;
        }
        
        void ZipFileOutput::dumpFile()
        {
            if(m_currentFile)
            {
                m_bufferedFiles.push_back(m_currentFile->str());
                std::string & fileContent = m_bufferedFiles.back();
                
                zip_source* source = zip_source_buffer(m_archiveHandle, fileContent.c_str(), fileContent.size(), 0);
                if(! source)
                    throw FileAccessFailed(m_archive);
                
                if(zip_add(m_archiveHandle, m_currentFilename.c_str(), source) < 0)
                {
                    zip_source_free(source);
                    throw FileAccessFailed(m_archive, "Failed to add file '" + m_currentFilename + "' to archive.");
                }
                
                delete m_currentFile;
                m_currentFile = 0;
            }
        }
    }
}

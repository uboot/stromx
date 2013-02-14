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

#include <boost/filesystem.hpp>
#include "stromx/runtime/DirectoryFileOutput.h"
#include "stromx/runtime/Exception.h"

namespace stromx
{
    namespace runtime
    {
        const std::string DirectoryFileOutput::PATH_SEPARATOR = boost::filesystem::path("/").string();
        
        DirectoryFileOutput::~DirectoryFileOutput()
        {
            if(m_currentFile.is_open())
                m_currentFile.close();
        }

        void DirectoryFileOutput::initialize(const std::string& filename)
        {
            m_currentText.str("");
            
            if(m_currentFile.is_open())
                m_currentFile.close();
        
            m_currentBasename = filename;
            m_initialized = true;
            m_currentFilename = "";
        }

        const std::string DirectoryFileOutput::getText() const
        {
            return m_currentText.str();
        }

        std::ostream& DirectoryFileOutput::text()
        {
            if(! m_initialized)
                throw WrongState("Directory output has not been initialized.");
            
            return m_currentText;
        }

        std::ostream& DirectoryFileOutput::openFile(const std::string& ext, const stromx::runtime::OutputProvider::OpenMode mode)
        {
            if(! m_initialized)
                throw WrongState("Directory output has not been initialized.");
            
            if(m_currentFile.is_open())
                throw WrongState("File has already been opened.");

            std::string filename;  
            
            if(! ext.empty())
                m_currentFilename = m_currentBasename + "." + ext;
            else
                m_currentFilename = m_currentBasename;
            
            filename = m_directory + PATH_SEPARATOR + m_currentFilename;
                
            std::ios_base::openmode iosmode = std::ios_base::out;
            if(mode == BINARY)
                iosmode |= std::ios_base::binary;
            
            m_currentFile.open(filename.c_str(), iosmode);
            
            if(m_currentFile.fail())
                throw FileAccessFailed(filename, m_directory);
            
            return m_currentFile;
        }
        
        const std::string& DirectoryFileOutput::getFilename() const
        {
            if(! m_initialized)
                throw WrongState("Directory output has not been initialized.");
            
            return m_currentFilename;
        }

        std::ostream& DirectoryFileOutput::file()
        {
            if(! m_initialized)
                throw WrongState("Directory output has not been initialized.");
            
            if(! m_currentFile.is_open())
                throw WrongState("File has not been opened.");
            
            return m_currentFile;
        }     
    }
}
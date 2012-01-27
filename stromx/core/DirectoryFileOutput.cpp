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
#include "DirectoryFileOutput.h"
#include "Exception.h"

namespace stromx
{
    namespace core
    {
        const std::string DirectoryFileOutput::PATH_SEPARATOR = boost::filesystem::path("/").string();
        
        DirectoryFileOutput::~DirectoryFileOutput()
        {
            if(m_currentFile.is_open())
                m_currentFile.close();
        }

        void DirectoryFileOutput::setFile(const std::string& filename)
        {
            m_currentText.str("");
            
            if(m_currentFile.is_open())
                m_currentFile.close();
        
            m_currentFilename = filename;
            m_isSet = true;
            m_activeFilename = "";
        }

        const std::string DirectoryFileOutput::getText() const
        {
            return m_currentText.str();
        }

        std::ostream& DirectoryFileOutput::text()
        {
            if(! m_isSet)
                throw WrongState("No current file in directory output.");
            
            return m_currentText;
        }

        std::ostream& DirectoryFileOutput::openFile(const std::string& ext, const stromx::core::OutputProvider::OpenMode mode)
        {
            if(! m_isSet)
                throw WrongState("No current file in directory output.");
            
            if(m_currentFile.is_open())
                throw WrongState("File has already been opened.");
            
            std::ios_base::openmode iosmode;
            
            if(mode == BINARY)
                iosmode = std::ios_base::binary;
            
            std::string filename;
            
            if(! ext.empty())
                m_activeFilename = m_currentFilename + "." + ext;
            else
                m_activeFilename = m_currentFilename;
            
            filename = m_directory + PATH_SEPARATOR + m_activeFilename;
                
            m_currentFile.open(filename.c_str(), std::ios_base::in & iosmode);
            
            if(m_currentFile.fail())
                throw FileAccessFailed(m_currentFilename);
            
            return m_currentFile;
        }
        
        const std::string& DirectoryFileOutput::getFilename() const
        {
            if(! m_isSet)
                throw WrongState("No current file in directory output.");
            
            return m_activeFilename;
        }

        std::ostream& DirectoryFileOutput::file()
        {
            if(! m_isSet)
                throw WrongState("No current file in directory output.");
            
            if(! m_currentFile.is_open())
                throw WrongState("File has not been opened.");
            
            return m_currentFile;
        }     
    }
}
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
#include "DirectoryFileInput.h"
#include "Exception.h"

namespace stromx
{
    namespace core
    {
        const std::string DirectoryFileInput::PATH_SEPARATOR = boost::filesystem::path("/").string();
        
        DirectoryFileInput::~DirectoryFileInput()
        {
            if(m_currentFile.is_open())
                m_currentFile.close();
        }

        void DirectoryFileInput::initialize(const std::string& text, const std::string& filename)
        {
            m_currentText.clear();
            m_currentText.str(text);
            
            if(m_currentFile.is_open())
                m_currentFile.close();
            
            m_currentFilename = filename;
            m_initialized = true;
        }

        std::istream& DirectoryFileInput::text()
        {
            if(! m_initialized)
                throw WrongState("Directory file has not been initialized.");
            
            return m_currentText;
        }
        
        const bool DirectoryFileInput::hasFile() const 
        {
            if(! m_initialized)
                throw WrongState("Directory file has not been initialized.");
            
            return ! m_currentFilename.empty();
        }
            
        std::istream& DirectoryFileInput::openFile(const OpenMode mode)
        {
            if(! m_initialized)
                throw WrongState("Directory file has not been initialized.");
            
            if(m_currentFile.is_open())
                throw WrongState("File has already been opened.");
            
            if(! hasFile())
                throw NoInputFile();
            
            std::ios_base::openmode iosmode = std::ios_base::in;
            if(mode == BINARY)
                iosmode |= std::ios_base::binary;
            
            std::string filename = m_directory + PATH_SEPARATOR + m_currentFilename;
            m_currentFile.open(filename.c_str(), iosmode);
            
            if(m_currentFile.fail())
                throw FileAccessFailed(filename, m_directory, "");
            
            return m_currentFile;
        }

        std::istream& DirectoryFileInput::file()
        {
            if(! m_initialized)
                throw WrongState("Directory file has not been initialized.");
            
            if(! m_currentFile.is_open())
                throw WrongState("File has not been opened.");
            
            return m_currentFile;
        }
    }
}
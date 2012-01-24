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

#include "DirectoryFileInput.h"
#include "Exception.h"

namespace stromx
{
    namespace core
    {
        void DirectoryFileInput::selectFile(const std::string& filename)
        {
            if(m_currentFile.is_open())
                m_currentFile.close();
            
            m_currentFilename = filename;
        }
        
        DirectoryFileInput::~DirectoryFileInput()
        {
            if(m_currentFile.is_open())
                m_currentFile.close();
        }

        void DirectoryFileInput::setText(const std::string& text)
        {
            m_currentText.str(text);
        }

        std::istream& DirectoryFileInput::text()
        {
            return m_currentText;
        }

        std::istream& DirectoryFileInput::openFile(std::ios_base::openmode mode)
        {
            if(m_currentFile.is_open())
                m_currentFile.close();
            
            m_currentFile.open(m_currentFilename.c_str(), mode);
            
            if(m_currentFile.fail())
                throw FileAccessFailed(m_currentFilename);
            
            return m_currentFile;
        }

        std::istream& DirectoryFileInput::file()
        {
            if(! m_currentFile.is_open())
                throw WrongState("File has not been opened.");
            
            return m_currentFile;
        }
    }
}
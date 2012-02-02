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
          : m_archive(0)
        {
            int error = 0;
            m_archive = zip_open(archive.c_str(), ZIP_CREATE, &error);
            
            if(!m_archive)
                throw FileAccessFailed(archive);
        }

        ZipFileOutput::~ZipFileOutput()
        {
            if(m_archive)
                zip_close(m_archive);
        }

        void ZipFileOutput::initialize(const std::string& filename)
        {
            m_fileOutput.str("");
            m_fileOutput.clear();
        }

        const std::string& ZipFileOutput::getFilename() const
        {

        }

        const std::string ZipFileOutput::getText() const
        {

        }

        std::ostream& ZipFileOutput::text()
        {

        }

        std::ostream& ZipFileOutput::openFile(const std::string& ext, const stromx::core::OutputProvider::OpenMode mode)
        {

        }

        std::ostream& ZipFileOutput::file()
        {

        }
    }
}

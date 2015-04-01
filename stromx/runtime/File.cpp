/* 
 *  Copyright 2015 Matthias Fuchs
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

#include "stromx/runtime/Config.h"
#include "stromx/runtime/File.h"
#include "stromx/runtime/InputProvider.h"
#include "stromx/runtime/OutputProvider.h"
#include "stromx/runtime/Version.h"

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <fstream>

namespace stromx
{
    namespace runtime
    {
        const std::string File::TYPE = "File";
        const std::string File::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        const Version File::VERSION = Version(0, 1, 0);
        
        File::File(const std::string & path, const OpenMode mode)
          : m_mode(mode),
            m_path(path)
        {
            boost::filesystem::path boostPath(path);
            if (! boost::filesystem::exists(boostPath))
                throw FileAccessFailed(path, "File does not exist.");
                
            if (boost::filesystem::is_directory(boostPath))
                throw FileAccessFailed(path, "Path points to a directory.");
            
            m_extension = boostPath.extension().string();
            if (! m_extension.empty())
                m_extension.erase(0, 1);
        }
        
        File::File(const File & file)
          : m_mode(file.m_mode),
            m_path(file.m_path),
            m_extension(file.m_extension)
        {
        }
        
        void File::serialize(OutputProvider & output) const
        {
            OutputProvider::OpenMode mode = OutputProvider::TEXT;
            std::string modeString = "text";
            if (m_mode == BINARY)
            {
                mode = OutputProvider::BINARY;
                modeString = "binary";
            }
            
            std::ifstream file(m_path.c_str());
            if (! file.is_open())
                throw Exception((boost::format("Failed to open '%1'") % m_path).str());
                
            std::string content((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());
            
            output.text() << modeString << " " << m_extension;
            output.openFile(m_extension, mode);
            output.file() << content;
        }
        
        void File::deserialize(InputProvider & input, const Version & /*version*/)
        {
            std::string modeString;
            input.text() >> modeString;
            if (modeString == "text")
                m_mode = TEXT;
            else if (modeString == "binary")
                m_mode = BINARY;
            else
                throw Exception("Unknown file mode.");
            
            input.text() >> m_extension;
            std::string content((std::istreambuf_iterator<char>(input.file())),
                                 std::istreambuf_iterator<char>());
            
            boost::filesystem::path tempPath = boost::filesystem::unique_path();
            std::ofstream file(tempPath.native().c_str(),
                m_mode == TEXT ? std::ofstream::out : 
                                 std::ofstream::out | std::ofstream::binary);
            if (! file.is_open())
                throw Exception((boost::format("Failed to open file.") % tempPath.native()).str());
            file << content;
            m_path = tempPath.native();
        }
        
        const VariantHandle & File::variant() const
        {
            switch (m_mode)
            {
            case TEXT:
                return Variant::TEXT_FILE;
            case BINARY:
                return Variant::BINARY_FILE;
            default:
                BOOST_ASSERT(false);
            }
        }
    }
}
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
#include "stromx/runtime/Variant.h"

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <fstream>

namespace stromx
{
    namespace runtime
    {
        const std::string File::TYPE = "File";
        const std::string File::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        const Version File::VERSION = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
        
        std::string File::staticTempDir = boost::filesystem::temp_directory_path().native();
        
        const std::string& File::tempDir()
        {
            return staticTempDir;
        }

        void File::setTempDir(const std::string dir)
        {
            if (dir.empty())
                staticTempDir = boost::filesystem::temp_directory_path().native();
            else
                staticTempDir = dir;
        }
        
        const std::string File::tempPath(const std::string & extension)
        {
            boost::filesystem::path model = staticTempDir;
            model /= "%%%%-%%%%-%%%%-%%%%";
            model += extension;
            boost::filesystem::path tempPath = boost::filesystem::unique_path(model);
            return tempPath.native();
        }
        
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
            
            if (m_path.empty())
                return;
            
            std::string modeString = "text";
            if (m_mode == BINARY)
            {
                mode = OutputProvider::BINARY;
                modeString = "binary";
            }
            
            std::ifstream file(m_path.c_str());
            if (! file.is_open())
                throw Exception((boost::format("Failed to open '%1%'") % m_path).str());
                
            std::string content((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());
            
            output.text() << modeString << " " << m_extension;
            output.openFile(m_extension, mode);
            output.file() << content;
        }
        
        void File::deserialize(InputProvider & input, const Version & /*version*/)
        {            
            std::string modeString;
            InputProvider::OpenMode providerMode = InputProvider::TEXT;
            
            input.text() >> modeString;
            if (modeString == "text")
            {
                m_mode = TEXT;
            }
            else if (modeString == "binary")
            {
                m_mode = BINARY;
                providerMode = InputProvider::BINARY;
            }
            else if (modeString.empty())
            {
                return;
            }
            else
            {
                throw Exception("Unknown file mode.");
            }
            
            input.text() >> m_extension;
            input.openFile(providerMode);
            std::string content((std::istreambuf_iterator<char>(input.file())),
                                 std::istreambuf_iterator<char>());
            
            std::string path = tempPath(m_extension);
            std::ofstream file(path.c_str(),
                m_mode == TEXT ? std::ofstream::out : 
                                 std::ofstream::out | std::ofstream::binary);
            if (! file.is_open())
                throw Exception((boost::format("Failed to open file '%1%'.") % path).str());
            file << content;
            m_path = path;
        }
        
        void File::setExtension(const std::string & extension)
        {
            if (! m_path.empty())
                throw WrongState("The extension of files with non-empty path can not be set");
            m_extension = extension;
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
                throw InternalError();
            }
        }
        
        const VariantHandle & data_traits<File>::variant()
        { 
            return Variant::FILE;
        }
    }
}
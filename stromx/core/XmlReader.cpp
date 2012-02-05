/* 
 *  Copyright 2011 Matthias Fuchs
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

#include "XmlReader.h"

#include "DirectoryFileInput.h"
#include "ZipFileInput.h"
#include "impl/XmlReaderImpl.h"
#include "impl/XmlUtilities.h"

namespace stromx
{
    namespace core
    {
        Stream*const XmlReader::readStream(const std::string& filepath, const Factory& factory) const
        {
            std::string directory = impl::XmlUtilities::computePath(filepath);
            std::string filename = impl::XmlUtilities::computeName(filepath);
            std::string filebase = impl::XmlUtilities::stripExtension(filename);
            std::string extension = impl::XmlUtilities::computeExtension(filepath);
            
            Stream* stream = 0;
            if(extension == "xml")
            {
                DirectoryFileInput input(directory);
                stream = readStream(input, filename, factory);
            }
            else if(extension == "zip")
            {
                ZipFileInput input(filepath);
                stream = readStream(input, filebase + ".xml", factory);
            }
            else
                throw FileAccessFailed(filepath, "Filename has invalid extension '" + extension +"'.");
            
            return stream;
        }
        
        Stream*const XmlReader::readStream(FileInput& input, const std::string filename, const stromx::core::Factory& factory) const
        {
            impl::XmlReaderImpl impl(factory);
            return impl.readStream(input, filename);
        }
        
        void XmlReader::readParameters(const std::string& filepath, const stromx::core::Factory& factory,
                                       const std::vector< Operator* > & operators) const
        {
            std::string directory = impl::XmlUtilities::computePath(filepath);
            std::string filename = impl::XmlUtilities::computeName(filepath);
            std::string filebase = impl::XmlUtilities::stripExtension(filename);
            std::string extension = impl::XmlUtilities::computeExtension(filepath);
            
            if(extension == "xml")
            {
                DirectoryFileInput input(directory);
                readParameters(input, filename, factory, operators);
            }
            else if(extension == "zip")
            {
                ZipFileInput input(filepath);
                readParameters(input, filebase + ".xml", factory, operators);
            }
            else
                throw FileAccessFailed(filepath, "Filename has invalid extension '" + extension +"'.");
        }

        void XmlReader::readParameters(FileInput& input, const std::string filename, const stromx::core::Factory& factory,
                                       const std::vector< Operator* > & operators) const
        {
            impl::XmlReaderImpl impl(factory);
            impl.readParameters(input, filename, operators);
        }
    }
}

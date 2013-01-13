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
    namespace runtime
    {
        Stream* XmlReader::readStream(const std::string& filepath, const Factory& factory) const
        {
            std::string directory = impl::XmlUtilities::computePath(filepath);
            std::string filename = impl::XmlUtilities::computeName(filepath);
            std::string extension = impl::XmlUtilities::computeExtension(filepath);
            
            Stream* stream = 0;
            if(extension == "xml")
            {
                DirectoryFileInput input(directory);
                
                try
                {
                    stream = readStream(input, filename, factory);
                }
                catch(FileException& e)
                {
                    e.setContainer(directory);
                    throw;
                }
            }
            else if(extension == "zip" || extension == "stromx")
            {
                ZipFileInput input(filepath);
                
                try
                {
                    stream = readStream(input, "stream.xml", factory);
                }
                catch(FileException& e)
                {
                    e.setContainer(filepath);
                    throw;
                }
            }
            else
            {
                throw FileAccessFailed(filepath, "", "Filename has unsupported extension '" + extension +"'.");
            }
            
            return stream;
        }
        
        Stream* XmlReader::readStream(FileInput& input, const std::string filename, const stromx::runtime::Factory& factory) const
        {
            impl::XmlReaderImpl impl(factory);
            return impl.readStream(input, filename);
        }
        
        void XmlReader::readParameters(const std::string& filepath, const stromx::runtime::Factory& factory,
                                       const std::vector< Operator* > & operators) const
        {
            std::string directory = impl::XmlUtilities::computePath(filepath);
            std::string filename = impl::XmlUtilities::computeName(filepath);
            std::string extension = impl::XmlUtilities::computeExtension(filepath);
            
            if(extension == "xml")
            {
                DirectoryFileInput input(directory);
                
                try
                {
                    readParameters(input, filename, factory, operators);
                }
                catch(FileException& e)
                {
                    e.setContainer(directory);
                    throw;
                }
            }
            else if(extension == "zip" || extension == "stromx")
            {
                ZipFileInput input(filepath);
                
                try
                {
                    readParameters(input, "parameters.xml", factory, operators);
                }
                catch(FileException& e)
                {
                    e.setContainer(filepath);
                    throw;
                }
            }
            else
            {
                throw FileAccessFailed(filepath, "", "Filename has unsupported extension '" + extension +"'.");
            }
        }

        void XmlReader::readParameters(FileInput& input, const std::string filename, const stromx::runtime::Factory& factory,
                                       const std::vector< Operator* > & operators) const
        {
            impl::XmlReaderImpl impl(factory);
            impl.readParameters(input, filename, operators);
        }
    }
}

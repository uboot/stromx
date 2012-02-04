/* 
*  Copyright 2011 Thomas Fidler
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

#include "DirectoryFileOutput.h"
#include "ZipFileOutput.h"
#include "XmlWriter.h"
#include "impl/XmlUtilities.h"
#include "impl/XmlWriterImpl.h"

namespace stromx
{
    namespace core
    {
        void XmlWriter::writeStream(const std::string& filepath, const Stream& stream) const
        {
            std::string directory = impl::XmlUtilities::computePath(filepath);
            std::string filename = impl::XmlUtilities::stripExtension(impl::XmlUtilities::computeName(filepath));
            std::string extension = impl::XmlUtilities::computeExtension(filepath);
            
            if(extension == "xml")
            {
                DirectoryFileOutput output(directory);
                writeStream(output, filename, stream);
            }
            else if(extension == "zip")
            {
                ZipFileOutput output(filepath);
                writeStream(output, filename, stream);
            }
            else
                throw FileAccessFailed(filepath, "Filename has invalid extension '" + extension +"'.");
        }
        
        void XmlWriter::writeStream(FileOutput& output, const std::string filename, const stromx::core::Stream& stream) const
        {
            using namespace impl;
            
            XmlWriterImpl impl;
            impl.writeStream(output, XmlUtilities::stripExtension(filename), stream);
        }
        
        void XmlWriter::writeParameters(const std::string& filepath, 
                                        const std::vector<const stromx::core::Operator* >& operators) const
        {
            std::string directory = impl::XmlUtilities::computePath(filepath);
            std::string filename = impl::XmlUtilities::stripExtension(impl::XmlUtilities::computeName(filepath));
            std::string extension = impl::XmlUtilities::computeExtension(filepath);
            
            if(extension == "xml")
            {
                DirectoryFileOutput output(directory);
                writeParameters(output, filename, operators);
            }
            else if(extension == "zip")
            {
                ZipFileOutput output(filepath);
                writeParameters(output, filename, operators);
            }
            else
                throw FileAccessFailed(filepath, "Filename has invalid extension '" + extension +"'.");
        }
        
        void XmlWriter::writeParameters(FileOutput& output, const std::string filename,
                                        const std::vector< const stromx::core::Operator* >& operators) const
        {
            using namespace impl;
            
            XmlWriterImpl impl;
            impl.writeParameters(output, XmlUtilities::stripExtension(filename), operators);
        }
    }
}

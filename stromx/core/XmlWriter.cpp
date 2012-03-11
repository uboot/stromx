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
            std::string filebase = impl::XmlUtilities::stripExtension(impl::XmlUtilities::computeName(filepath));
            std::string extension = impl::XmlUtilities::computeExtension(filepath);
            
            if(extension == "xml")
            {
                DirectoryFileOutput output(directory);
                
                try
                {
                    writeStream(output, filebase, stream);
                }
                catch(FileException& e)
                {
                    e.setContainer(directory);
                    throw;
                }
            }
            else if(extension == "zip")
            {
                ZipFileOutput output(filepath);
                
                try
                {
                    writeStream(output, "stream", stream);
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
        
        void XmlWriter::writeStream(FileOutput& output, const std::string basename, const stromx::core::Stream& stream) const
        {
            using namespace impl;
            
            XmlWriterImpl impl;
            impl.writeStream(output, basename, stream);
        }
        
        void XmlWriter::writeParameters(const std::string& filepath, const std::vector< Operator* >& operators) const
        {
            std::vector<const Operator*> constOperators(operators.begin(), operators.end());
            writeParameters(filepath, constOperators);
        }
        
        void XmlWriter::writeParameters(const std::string& filepath, 
                                        const std::vector<const stromx::core::Operator* >& operators) const
        {
            std::string directory = impl::XmlUtilities::computePath(filepath);
            std::string filebase = impl::XmlUtilities::stripExtension(impl::XmlUtilities::computeName(filepath));
            std::string extension = impl::XmlUtilities::computeExtension(filepath);
            
            if(extension == "xml")
            {
                DirectoryFileOutput output(directory);
                
                try
                {
                    writeParameters(output, filebase, operators);
                }
                catch(FileException& e)
                {
                    e.setContainer(directory);
                    throw;
                }
            }
            else if(extension == "zip")
            {
                ZipFileOutput output(filepath);
                
                try
                {
                    writeParameters(output, "parameters", operators);
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
        
        void XmlWriter::writeParameters(FileOutput& output, const std::string basename, const std::vector< Operator* >& operators) const
        {
            std::vector<const Operator*> constOperators(operators.begin(), operators.end());
            writeParameters(output, basename, constOperators);
        }
        
        void XmlWriter::writeParameters(FileOutput& output, const std::string basename,
                                        const std::vector< const stromx::core::Operator* >& operators) const
        {
            using namespace impl;
            
            XmlWriterImpl impl;
            impl.writeParameters(output, basename, operators);
        }
    }
}

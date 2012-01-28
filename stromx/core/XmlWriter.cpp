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

#include "XmlWriter.h"

#include "impl/XmlUtilities.h"
#include "impl/XmlWriterImpl.h"
#include "DirectoryFileOutput.h"

namespace stromx
{
    namespace core
    {
        void XmlWriter::writeStream(const std::string& filepath, const Stream& stream) const
        {
            using namespace impl;
            
            XmlWriterImpl impl;
            
            std::string directory = XmlUtilities::computePath(filepath);
            std::string filename = XmlUtilities::stripExtension(XmlUtilities::computeName(filepath));
            DirectoryFileOutput output(directory);
            
            impl.writeStream(output, filename, stream);
        }
        
        void XmlWriter::writeStream(FileOutput& output, const std::string filename, const stromx::core::Stream& stream) const
        {
            using namespace impl;
            
            XmlWriterImpl impl;
            impl.writeStream(output, XmlUtilities::stripExtension(filename), stream);
        }
    }
}

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
#include "impl/XmlReaderImpl.h"
#include "impl/XmlUtilities.h"

namespace stromx
{
    namespace core
    {
        Stream*const XmlReader::readStream(const std::string& filepath, const Factory& factory) const
        {
            using namespace impl;
            
            XmlReaderImpl impl(factory);
            
            std::string directory = XmlUtilities::computePath(filepath);
            std::string filename = XmlUtilities::computeName(filepath);
            DirectoryFileInput input(directory);
            
            return impl.readStream(input, filename);
        }
        
        Stream*const XmlReader::readStream(FileInput& input, const std::string filename, const stromx::core::Factory& factory) const
        {
            impl::XmlReaderImpl impl(factory);
            return impl.readStream(input, filename);
        }
    }
}

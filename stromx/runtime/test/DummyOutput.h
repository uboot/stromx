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

#ifndef STROMX_RUNTIME_DUMMYOUTPUT_H
#define STROMX_RUNTIME_DUMMYOUTPUT_H

#include "stromx/runtime/OutputProvider.h"

namespace stromx
{
    namespace runtime
    {
        class DummyOutput : public OutputProvider
        {
        public:
            std::ostream & text() { return m_text; }
            std::ostream & openFile(const std::string &, const OpenMode) { return m_file; }
            std::ostream & file() { return m_file; }
            
            const std::string value() const { return m_text.str(); }
            
        private:
            std::ostringstream m_text;
            std::ostringstream m_file;
        };
    }
}

#endif // STROMX_RUNTIME_DUMMYOUTPUT_H

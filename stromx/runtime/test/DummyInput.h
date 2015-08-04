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

#ifndef STROMX_RUNTIME_DUMMYINPUT_H
#define STROMX_RUNTIME_DUMMYINPUT_H

#include "stromx/runtime/InputProvider.h"

namespace stromx
{
    namespace runtime
    {
        class DummyInput : public InputProvider
        {
        public:
            explicit DummyInput(const std::string & text)
              : m_text(text),
                m_file("")
            {}
            
            std::istream & text() { return m_text; }
            std::istream & openFile(const OpenMode) { return m_file; }
            bool hasFile() const { return true; }
            std::istream & file() { return m_file; }
            
        private:
            std::istringstream m_text;
            std::istringstream m_file;
        };
    }
}

#endif // STROMX_RUNTIME_DUMMYINPUT_H

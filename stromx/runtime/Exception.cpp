/* 
 *  Copyright 2012 Matthias Fuchs
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

#include "Exception.h"

#ifdef __GNUG__
    #include <execinfo.h>
    #include <cxxabi.h>
    #include <sstream>
#endif // __GNUG__

namespace stromx
{
    namespace runtime
    {
        Exception::Exception(const std::string & message)
          : m_message(message)
        {
#ifdef DEBUG
    #ifdef __GNUG__
            std::ostringstream out;
            
            const size_t MAX_DEPTH = 100;
            size_t depth = 0;
            void *addresses[MAX_DEPTH];
            char **lines;
            depth = backtrace(addresses, MAX_DEPTH);
            lines = backtrace_symbols(addresses, depth);
            out << "Call stack:" << std::endl;
            
            for (size_t i = 1; i < depth; i++)
            {
                std::string line(lines[i]);
                size_t start = line.find_first_of("(");
                size_t end = line.find_first_of("+");
                
                if(start != std::string::npos && end != std::string::npos)
                {
                    std::string function = line.substr(start + 1, end - start - 1);
                    const size_t LENGTH = 300;
                    char* demangled = new char[LENGTH];
                    size_t newLength = LENGTH;
                    int status;
                    
                    char* result = abi::__cxa_demangle(function.c_str(), demangled, &newLength, &status);
                    
                    if(result)
                    {
                        demangled = result;
                        out << demangled << std::endl;
                    }
                    else
                    {
                        out << line << std::endl;
                    }
                    
                    delete [] demangled;
                }
                else
                {
                    out << line << std::endl;
                }
            }
            
            delete [] lines;
            m_callStack = out.str();
    #endif // __GNUG__
#endif // DEBUG
        }
    }
}

/* 
 *  Copyright 2013 Matthias Fuchs
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

#include "stromx/runtime/Color.h"

#include <iomanip>
#include <sstream>

#include "stromx/runtime/Exception.h"

namespace
{
    uint8_t hexToInt(const std::string & hex)
    {
        std::istringstream in(hex);
        unsigned int value;
        in >> std::hex >> value;
        
        return value;
    }
}

namespace stromx
{
    namespace runtime
    {
        std::ostream& operator<<(std::ostream& out, const Color& color)
        {
            out << "#";
            out << std::setw(2) << std::setfill('0') << std::hex << (unsigned int)(color.r());
            out << std::setw(2) << std::setfill('0') << std::hex << (unsigned int)(color.g());
            out << std::setw(2) << std::setfill('0') << std::hex << (unsigned int)(color.b());
            return out;
        }

        std::istream& operator>>(std::istream& in, Color& color)
        {
            std::string text;
            in >> std::setw(1) >> text;
            
            if(text.length() != 1 || text[0] != '#')
                throw WrongArgument("Input must be of the type '#rrggbb'.");
            
            std::string r, g, b;
            in >> std::setw(2) >> r;
            in >> std::setw(2) >> g;
            in >> std::setw(2) >> b;
            
            color = Color(hexToInt(r), hexToInt(g), hexToInt(b));
            
            return in;
        }
    }
}

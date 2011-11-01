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

#ifndef STROM_ENUM_H
#define STROM_ENUM_H

#include "Primitive.h"

namespace strom
{
    /** \brief Value of an enumeration */
    class Enum : public UInt32
    {
    public:
        Enum() : UInt32() {}
        Enum(const unsigned int value) : UInt32(value) {}
        
        virtual const DataVariant & variant() const { return DataVariant::ENUM; }
        virtual const std::string & type() const { return TYPE; }
        
    private:
        static const std::string TYPE;
    };
}

#endif // STROM_ENUM_H

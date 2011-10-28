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


#include <float.h>

#include "Config.h"
#include "Primitive.h"

namespace stromx
{
    namespace core
    {
        template<>
        const DataVariant & Bool::variant() const { return DataVariant::BOOL; }
        template<>
        const std::string Bool::TYPE = "Bool";
        template<>
        const std::string Bool::PACKAGE = PACKAGE_NAME;
        template<>
        const Version Bool::VERSION = Version(STROM_VERSION_MAJOR, STROM_VERSION_MINOR);
        
        template<>
        const DataVariant & Int8::variant() const { return DataVariant::INT_8; }
        template<>
        const std::string Int8::TYPE = "Int8";
        template<>
        const Int8 Int8::MIN = Int8(INT8_MIN);
        template<>
        const Int8 Int8::MAX = Int8(INT8_MAX);
        template<>
        const std::string Int8::PACKAGE = PACKAGE_NAME;
        template<>
        const Version Int8::VERSION = Version(STROM_VERSION_MAJOR, STROM_VERSION_MINOR);
        
        template <>
        const std::string Int8::serialize(const std::string & name, const std::string & path) const
        {
            return boost::lexical_cast<std::string>(static_cast<int>(*this));
        }
        
        template <>
        void Int8::deserialize(const std::string & data, const std::string & path)
        {
            *this = Int8(boost::lexical_cast<int>(data));
        }
        
        template<>
        const DataVariant & UInt8::variant() const { return DataVariant::UINT_8; }
        template<>
        const std::string UInt8::TYPE = "UInt8";
        template<>
        const UInt8 UInt8::MIN = UInt8(0);
        template<>
        const UInt8 UInt8::MAX = UInt8(UINT8_MAX);
        template<>
        const std::string UInt8::PACKAGE = PACKAGE_NAME;
        template<>
        const Version UInt8::VERSION = Version(STROM_VERSION_MAJOR, STROM_VERSION_MINOR);
        
        template <>
        const std::string UInt8::serialize(const std::string & name, const std::string & path) const
        {
            return boost::lexical_cast<std::string>(static_cast<int>(*this));
        }
        
        template <>
        void UInt8::deserialize(const std::string & data, const std::string & path)
        {
            *this = UInt8(boost::lexical_cast<int>(data));
        }
        
        template<>
        const DataVariant & Int16::variant() const { return DataVariant::INT_16; }
        template<>
        const std::string Int16::TYPE = "Int16";
        template<>
        const std::string Int16::PACKAGE = PACKAGE_NAME;
        template<>
        const Version Int16::VERSION = Version(STROM_VERSION_MAJOR, STROM_VERSION_MINOR);
        
        template<>
        const DataVariant & UInt16::variant() const { return DataVariant::UINT_16; }
        template<>
        const std::string UInt16::TYPE = "UInt16";
        template<>
        const std::string UInt16::PACKAGE = PACKAGE_NAME;
        template<>
        const Version UInt16::VERSION = Version(STROM_VERSION_MAJOR, STROM_VERSION_MINOR);
        
        template<>
        const DataVariant & Int32::variant() const { return DataVariant::INT_32; }
        template<>
        const std::string Int32::TYPE = "Int32";
        template<>
        const Int32 Int32::MIN = Int32(INT32_MIN);
        template<>
        const Int32 Int32::MAX = Int32(INT32_MAX);
        template<>
        const std::string Int32::PACKAGE = PACKAGE_NAME;
        template<>
        const Version Int32::VERSION = Version(STROM_VERSION_MAJOR, STROM_VERSION_MINOR);
        
        template<>
        const DataVariant & UInt32::variant() const { return DataVariant::UINT_32; }
        template<>
        const std::string UInt32::TYPE = "UInt32";
        template<>
        const UInt32 UInt32::MIN = UInt32(0);
        template<>
        const UInt32 UInt32::MAX = UInt32(UINT32_MAX);
        template<>
        const std::string UInt32::PACKAGE = PACKAGE_NAME;
        template<>
        const Version UInt32::VERSION = Version(STROM_VERSION_MAJOR, STROM_VERSION_MINOR);
        
        template<>
        const DataVariant & Float::variant() const { return DataVariant::FLOAT; }
        template<>
        const std::string Float::TYPE = "Float";
        template<>
        const Float Float::MIN = Float(- FLT_MAX);
        template<>
        const Float Float::MAX = Float(FLT_MAX);
        template<>
        const std::string Float::PACKAGE = PACKAGE_NAME;
        template<>
        const Version Float::VERSION = Version(STROM_VERSION_MAJOR, STROM_VERSION_MINOR);
        
        template<>
        const DataVariant & Double::variant() const { return DataVariant::DOUBLE; }
        template<>
        const std::string Double::TYPE = "Double";
        template<>
        const Double Double::MIN = Double(- DBL_MAX);
        template<>
        const Double Double::MAX = Double(DBL_MAX);
        template<>
        const std::string Double::PACKAGE = PACKAGE_NAME;
        template<>
        const Version Double::VERSION = Version(STROM_VERSION_MAJOR, STROM_VERSION_MINOR);
    }
}


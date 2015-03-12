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

#include "stromx/runtime/Primitive.h"
#include <boost/lexical_cast.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <cfloat>
#include "stromx/runtime/Config.h"
#include "stromx/runtime/InputProvider.h"
#include "stromx/runtime/OutputProvider.h"

namespace stromx
{
    namespace runtime
    {
        namespace 
        {
            const unsigned int VERSION_MAJOR = 0;
            const unsigned int VERSION_MINOR = 1;
            const unsigned int VERSION_DESCRIPTION = 0;
        }
        
        template<>
        const VariantHandle & Bool::variant() const { return Variant::BOOL; }
        template<>
        const std::string Bool::TYPE = "Bool";
        template<>
        const std::string Bool::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version Bool::VERSION = Version(VERSION_MAJOR, VERSION_MINOR, VERSION_DESCRIPTION);
        
        template <>
        void Bool::serialize(OutputProvider & out) const
        {
            out.text() << m_value;
        }
        
        template <>
        void Bool::deserialize(InputProvider & in, const Version &)
        {
            in.text() >> m_value;
        }
        
        template<>
        const VariantHandle & Int8::variant() const { return Variant::INT_8; }
        template<>
        const std::string Int8::TYPE = "Int8";
        template<>
        const Int8 Int8::MIN = Int8(INT8_MIN);
        template<>
        const Int8 Int8::MAX = Int8(INT8_MAX);
        template<>
        const std::string Int8::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version Int8::VERSION = Version(VERSION_MAJOR, VERSION_MINOR, VERSION_DESCRIPTION);

        template <>
        void Int8::serialize(OutputProvider & out) const
        {
            out.text() << static_cast<int>(m_value);
        }
        
        template <>
        void Int8::deserialize(InputProvider & in, const Version &)
        {
            int temp;
            in.text() >> temp;
            m_value = static_cast<int8_t>(temp);
        }
        
        template<>
        const VariantHandle & UInt8::variant() const { return Variant::UINT_8; }
        template<>
        const std::string UInt8::TYPE = "UInt8";
        template<>
        const UInt8 UInt8::MIN = UInt8(0);
        template<>
        const UInt8 UInt8::MAX = UInt8(UINT8_MAX);
        template<>
        const std::string UInt8::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version UInt8::VERSION = Version(VERSION_MAJOR, VERSION_MINOR, VERSION_DESCRIPTION);
        
        template <>
        void UInt8::serialize(OutputProvider & out) const
        {
            out.text() << static_cast<int>(m_value);
        }
        
        template <>
        void UInt8::deserialize(InputProvider & in, const Version &)
        {
            unsigned int temp;
            in.text() >> temp;
            m_value = static_cast<uint8_t>(temp);
        }
        
        template<>
        const VariantHandle & Int16::variant() const { return Variant::INT_16; }
        template<>
        const std::string Int16::TYPE = "Int16";
        template<>
        const Int16 Int16::MIN = Int16(INT16_MIN);
        template<>
        const Int16 Int16::MAX = Int16(INT16_MAX);
        template<>
        const std::string Int16::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version Int16::VERSION = Version(VERSION_MAJOR, VERSION_MINOR, VERSION_DESCRIPTION);
        
        template <>
        void Int16::serialize(OutputProvider & out) const
        {
            out.text() << m_value;
        }
        
        template <>
        void Int16::deserialize(InputProvider & in, const Version &)
        {
            in.text() >> m_value;
        }
        
        template<>
        const VariantHandle & UInt16::variant() const { return Variant::UINT_16; }
        template<>
        const std::string UInt16::TYPE = "UInt16";
        template<>
        const UInt16 UInt16::MIN = UInt16(0);
        template<>
        const UInt16 UInt16::MAX = UInt16(UINT16_MAX);
        template<>
        const std::string UInt16::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version UInt16::VERSION = Version(VERSION_MAJOR, VERSION_MINOR, VERSION_DESCRIPTION);
        
        template <>
        void UInt16::serialize(OutputProvider & out) const
        {
            out.text() << m_value;
        }
        
        template <>
        void UInt16::deserialize(InputProvider & in, const Version &)
        {
            in.text() >> m_value;
        }
        
        template<>
        const VariantHandle & Int32::variant() const { return Variant::INT_32; }
        template<>
        const std::string Int32::TYPE = "Int32";
        template<>
        const Int32 Int32::MIN = Int32(INT32_MIN);
        template<>
        const Int32 Int32::MAX = Int32(INT32_MAX);
        template<>
        const std::string Int32::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version Int32::VERSION = Version(VERSION_MAJOR, VERSION_MINOR, VERSION_DESCRIPTION);
        
        template <>
        void Int32::serialize(OutputProvider & out) const
        {
            out.text() << m_value;
        }
        
        template <>
        void Int32::deserialize(InputProvider & in, const Version &)
        {
            in.text() >> m_value;
        }
        
        template<>
        const VariantHandle & UInt32::variant() const { return Variant::UINT_32; }
        template<>
        const std::string UInt32::TYPE = "UInt32";
        template<>
        const UInt32 UInt32::MIN = UInt32(0);
        template<>
        const UInt32 UInt32::MAX = UInt32(UINT32_MAX);
        template<>
        const std::string UInt32::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version UInt32::VERSION = Version(VERSION_MAJOR, VERSION_MINOR, VERSION_DESCRIPTION);
        
        template <>
        void UInt32::serialize(OutputProvider & out) const
        {
            out.text() << m_value;
        }
        
        template <>
        void UInt32::deserialize(InputProvider & in, const Version &)
        {
            in.text() >> m_value;
        }
        
        template<>
        const VariantHandle & Int64::variant() const { return Variant::INT_64; }
        template<>
        const std::string Int64::TYPE = "Int64";
        template<>
        const Int64 Int64::MIN = Int64(INT64_MIN);
        template<>
        const Int64 Int64::MAX = Int64(INT64_MAX);
        template<>
        const std::string Int64::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version Int64::VERSION = Version(VERSION_MAJOR, VERSION_MINOR, VERSION_DESCRIPTION);
        
        template <>
        void Int64::serialize(OutputProvider & out) const
        {
            out.text() << m_value;
        }
        
        template <>
        void Int64::deserialize(InputProvider & in, const Version &)
        {
            in.text() >> m_value;
        }
        
        template<>
        const VariantHandle & UInt64::variant() const { return Variant::UINT_64; }
        template<>
        const std::string UInt64::TYPE = "UInt64";
        template<>
        const UInt64 UInt64::MIN = UInt64(0);
        template<>
        const UInt64 UInt64::MAX = UInt64(UINT64_MAX);
        template<>
        const std::string UInt64::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version UInt64::VERSION = Version(VERSION_MAJOR, VERSION_MINOR, VERSION_DESCRIPTION);
        
        template <>
        void UInt64::serialize(OutputProvider & out) const
        {
            out.text() << m_value;
        }
        
        template <>
        void UInt64::deserialize(InputProvider & in, const Version &)
        {
            in.text() >> m_value;
        }
        
        template<>
        const VariantHandle & Float32::variant() const { return Variant::FLOAT_32; }
        template<>
        const std::string Float32::TYPE = "Float32";
        template<>
        const Float32 Float32::MIN = Float32(- FLT_MAX);
        template<>
        const Float32 Float32::MAX = Float32(FLT_MAX);
        template<>
        const std::string Float32::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version Float32::VERSION = Version(VERSION_MAJOR, VERSION_MINOR, VERSION_DESCRIPTION);
        
        template <>
        void Float32::serialize(OutputProvider & out) const
        {
            out.text() << m_value;
        }
        
        template <>
        void Float32::deserialize(InputProvider & in, const Version &)
        {
            in.text() >> m_value;
        }
        
        template<>
        const VariantHandle & Float64::variant() const { return Variant::FLOAT_64; }
        template<>
        const std::string Float64::TYPE = "Float64";
        template<>
        const Float64 Float64::MIN = Float64(- DBL_MAX);
        template<>
        const Float64 Float64::MAX = Float64(DBL_MAX);
        template<>
        const std::string Float64::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version Float64::VERSION = Version(VERSION_MAJOR, VERSION_MINOR, VERSION_DESCRIPTION);
    
        template <>
        void Float64::serialize(OutputProvider & out) const
        {
            out.text() << m_value;
        }
        
        template <>
        void Float64::deserialize(InputProvider & in, const Version &)
        {
            in.text() >> m_value;
        }
    }
}


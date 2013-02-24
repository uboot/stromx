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
        template<>
        const DataVariant & Bool::variant() const { return DataVariant::BOOL; }
        template<>
        const std::string Bool::TYPE = "Bool";
        template<>
        const std::string Bool::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version Bool::VERSION = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
        
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
        const DataVariant & Int8::variant() const { return DataVariant::INT_8; }
        template<>
        const std::string Int8::TYPE = "Int8";
        template<>
        const Int8 Int8::MIN = Int8(INT8_MIN);
        template<>
        const Int8 Int8::MAX = Int8(INT8_MAX);
        template<>
        const std::string Int8::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version Int8::VERSION = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);

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
        const DataVariant & UInt8::variant() const { return DataVariant::UINT_8; }
        template<>
        const std::string UInt8::TYPE = "UInt8";
        template<>
        const UInt8 UInt8::MIN = UInt8(0);
        template<>
        const UInt8 UInt8::MAX = UInt8(UINT8_MAX);
        template<>
        const std::string UInt8::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version UInt8::VERSION = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
        
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
        const DataVariant & Int16::variant() const { return DataVariant::INT_16; }
        template<>
        const std::string Int16::TYPE = "Int16";
        template<>
        const Int16 Int16::MIN = Int16(INT16_MIN);
        template<>
        const Int16 Int16::MAX = Int16(INT16_MAX);
        template<>
        const std::string Int16::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version Int16::VERSION = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
        
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
        const DataVariant & UInt16::variant() const { return DataVariant::UINT_16; }
        template<>
        const std::string UInt16::TYPE = "UInt16";
        template<>
        const UInt16 UInt16::MIN = UInt16(0);
        template<>
        const UInt16 UInt16::MAX = UInt16(UINT16_MAX);
        template<>
        const std::string UInt16::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version UInt16::VERSION = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
        
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
        const DataVariant & Int32::variant() const { return DataVariant::INT_32; }
        template<>
        const std::string Int32::TYPE = "Int32";
        template<>
        const Int32 Int32::MIN = Int32(INT32_MIN);
        template<>
        const Int32 Int32::MAX = Int32(INT32_MAX);
        template<>
        const std::string Int32::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version Int32::VERSION = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
        
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
        const DataVariant & UInt32::variant() const { return DataVariant::UINT_32; }
        template<>
        const std::string UInt32::TYPE = "UInt32";
        template<>
        const UInt32 UInt32::MIN = UInt32(0);
        template<>
        const UInt32 UInt32::MAX = UInt32(UINT32_MAX);
        template<>
        const std::string UInt32::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version UInt32::VERSION = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
        
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
        const DataVariant & Float::variant() const { return DataVariant::FLOAT; }
        template<>
        const std::string Float::TYPE = "Float";
        template<>
        const Float Float::MIN = Float(- FLT_MAX);
        template<>
        const Float Float::MAX = Float(FLT_MAX);
        template<>
        const std::string Float::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version Float::VERSION = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
        
        template <>
        void Float::serialize(OutputProvider & out) const
        {
            out.text() << m_value;
        }
        
        template <>
        void Float::deserialize(InputProvider & in, const Version &)
        {
            in.text() >> m_value;
        }
        
        template<>
        const DataVariant & Double::variant() const { return DataVariant::DOUBLE; }
        template<>
        const std::string Double::TYPE = "Double";
        template<>
        const Double Double::MIN = Double(- DBL_MAX);
        template<>
        const Double Double::MAX = Double(DBL_MAX);
        template<>
        const std::string Double::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        template<>
        const Version Double::VERSION = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
    
        template <>
        void Double::serialize(OutputProvider & out) const
        {
            out.text() << m_value;
        }
        
        template <>
        void Double::deserialize(InputProvider & in, const Version &)
        {
            in.text() >> m_value;
        }
    }
}


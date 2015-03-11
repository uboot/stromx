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

#ifndef STROMX_RUNTIME_TESTDATA_H
#define STROMX_RUNTIME_TESTDATA_H

#include "stromx/runtime/Data.h"

namespace stromx
{
    namespace runtime
    {
        class TestData : public Data
        {
        public:
            static const VariantHandle TEST_DATA;
            
            TestData() : m_value(0) { wasDestructed = false; }
            explicit TestData(const int value) : m_value(value) { wasDestructed = false; }
            ~TestData() { wasDestructed = true; }
            
            virtual const Version & version() const { return VERSION; }
            virtual const std::string & type() const { return TYPE; }
            virtual const std::string & package() const { return PACKAGE; }
            
            virtual const VariantHandle & variant() const { return TEST_DATA; }
            
            virtual Data* clone() const { return new TestData; }
            
            virtual void serialize(OutputProvider & output) const;
            virtual void deserialize(InputProvider & input, const Version & version);
            
            int value() const { return m_value; }
            void setValue(int value) { m_value = value; }
            
            static bool wasDestructed;
            
        private:
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const Version VERSION;
            static const unsigned int TEST_DATA_ID = 0;
            
            int m_value;
        };
        
        template <>
        class data_traits<TestData>
        {
        public:
            static const VariantHandle & variant() { return TestData::TEST_DATA; }
        };
    }
}

#endif // STROMX_RUNTIME_TESTDATA_H

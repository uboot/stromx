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

#ifndef STROMX_CORE_TESTDATA_H
#define STROMX_CORE_TESTDATA_H

#include "../Data.h"

namespace stromx
{
    namespace core
    {
        class TestData : public Data
        {
        public:
            TestData() : m_value(0) { wasDestructed = false; }
            TestData(const int value) : m_value(value) { wasDestructed = false; }
            ~TestData() { wasDestructed = true; }
            
            virtual const Version & version() const { return VERSION; }
            virtual const std::string & type() const { return TYPE; }
            virtual const std::string & package() const { return PACKAGE; }
            
            virtual const DataVariant & variant() const { return DataVariant::NONE; }
            
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
            
            int m_value;
        };
    }
}

#endif // STROMX_CORE_TESTDATA_H

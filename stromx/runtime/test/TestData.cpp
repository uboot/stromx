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

#include <fstream>
#include "stromx/runtime/InputProvider.h"
#include "stromx/runtime/OutputProvider.h"
#include "stromx/runtime/Version.h"
#include "stromx/runtime/test/TestData.h"

namespace stromx
{
    namespace runtime
    {
        bool TestData::wasDestructed = false;
        
        class TestVariant : public VariantInterface
        {            
        public:
            TestVariant()
              : m_id(0), m_package("TestPackage")
            {}
            
            unsigned int id() const { return m_id; }
            const std::string & package() const { return m_package; }
            virtual const std::string title() const { return ""; }
            virtual bool isVariant(const VariantInterface& variant) const
            {   
                return variant.package() == m_package || variant == Variant::DATA;
            }
            
        private:
            unsigned int m_id;
            std::string m_package;
        };
        
        const std::string TestData::TYPE = "TestData";
        const std::string TestData::PACKAGE = "TestPackage";
        const Version TestData::VERSION = Version(0, 1, 0);
        
        const VariantHandle TestData::TEST_DATA = VariantHandle(new TestVariant());
        
        void TestData::serialize(OutputProvider & output) const
        {
            output.openFile("txt", OutputProvider::TEXT) << m_value;
        }

        void TestData::deserialize(InputProvider & input, const Version &)
        {
            input.openFile(InputProvider::TEXT) >> m_value;
        }
    }
}
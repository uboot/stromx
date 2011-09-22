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

#ifndef STROM_TESTDATA_H
#define STROM_TESTDATA_H

#include <strom/Data.h>

namespace strom
{
    class TestData : public Data
    {
    public:
        TestData() { wasDestructed = false; }
        ~TestData() { wasDestructed = true; }
        
        virtual const Version & version() const { return VERSION; }
        virtual const std::string & type() const { return TYPE; }
        virtual const std::string & package() const { return PACKAGE; }
        
        virtual const DataVariant & variant() const { return DataVariant::NONE; }
        
        virtual Data* const clone() const { return new TestData; }
        
        static bool wasDestructed;
        
    private:
        static const std::string TYPE;
        static const std::string PACKAGE;
        static const Version VERSION;
    };
}

#endif // STROM_TESTDATA_H

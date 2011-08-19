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

#ifndef STREAM_TRIGGER_H
#define STREAM_TRIGGER_H

#include "Data.h"

namespace stream
{
    class Trigger : public Data
    {
    public:
        Trigger() {}
        
        virtual const Version & version() const { return VERSION; }
        virtual const std::string & name() const { return NAME; }
        virtual const std::string & package() const { return PACKAGE; }
        
        virtual const DataType & type() const { return DataType::TRIGGER; }
        
        virtual Data* const clone() const { return new Trigger; }
        
    private:
        static const std::string NAME;
        static const std::string PACKAGE;
        static const Version VERSION;
    };
}

#endif // STREAM_ENUM_H

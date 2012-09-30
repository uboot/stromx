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

#include "Data.h"

#include "DataRef.h"
#include "Exception.h"

namespace stromx
{
    namespace core
    {
        const stromx::core::DataVariant& Data::classVariant()
        {
            return DataVariant::DATA;
        }
        
        Data::operator DataRef() const
        {
            return DataRef(this->clone());
        }
        
        void Data::serialize(OutputProvider & out) const
        {
            throw Exception("This data type does not support serialization.");
        }

        void Data::deserialize(InputProvider & in, const Version & version)
        {
            throw Exception("This data type does not support deserialization.");
        }
    }
}

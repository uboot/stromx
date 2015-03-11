/* 
 *  Copyright 2012 Matthias Fuchs
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

#include "stromx/runtime/Data.h"
#include "stromx/runtime/DataRef.h"

namespace stromx
{
    namespace runtime
    {
        DataRef::DataRef(Data* data)
          : m_data(data) 
        { 
        }
        
        DataRef::DataRef(const stromx::runtime::Data& data)
          : m_data(data.clone())
        {
        }

        const Version & DataRef::version() const
        { 
            return m_data->version();
        }
        
        const std::string & DataRef::type() const
        {
            return m_data->type();
        }
        
        const std::string & DataRef::package() const
        { 
            return m_data->package();  
        }
        
        const VariantHandle & DataRef::variant() const 
        { 
            return m_data->variant();
        }
        
        bool DataRef::isVariant(const VariantInterface & v) const
        {
            return m_data->isVariant(v);
        }
        
        Data* DataRef::clone() const 
        {
            return m_data->clone();
        }
        
        void DataRef::serialize(OutputProvider & out) const
        { 
            return m_data->serialize(out);
        }       
        
        void DataRef::deserialize(InputProvider & in, const Version & version)
        { 
            m_data->deserialize(in, version);
        } 
    }
}

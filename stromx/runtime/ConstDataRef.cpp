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

#include "ConstDataRef.h"

#include "stromx/runtime/Data.h"
#include "stromx/runtime/DataRef.h"

namespace stromx
{
    namespace runtime
    {
        ConstDataRef::ConstDataRef(const Data* data)
          : m_data(data)
        {
        }
        
        ConstDataRef::ConstDataRef(const DataRef & dataRef) 
          : m_data(dataRef.m_data)
        {
        }
        
        const Version & ConstDataRef::version() const
        { 
            return m_data->version();
        }
        
        const std::string & ConstDataRef::type() const
        {
            return m_data->type();
        }
        
        const std::string & ConstDataRef::package() const
        { 
            return m_data->package();  
        }
        
        const VariantHandle & ConstDataRef::variant() const 
        { 
            return m_data->variant();
        }
        
        bool ConstDataRef::isVariant(const VariantInterface & v) const
        {
            return m_data->isVariant(v);
        }
        
        Data* ConstDataRef::clone() const 
        {
            return m_data->clone();
        }
        
        void ConstDataRef::serialize(OutputProvider & out) const
        { 
            return m_data->serialize(out);
        }        
        
        void ConstDataRef::deserialize(InputProvider &, const Version &)
        { 
            throw NotImplemented("Constant data references can not be deserialized.");
        }
    }
}

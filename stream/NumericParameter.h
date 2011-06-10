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

#ifndef STREAM_NUMERICPARAMETER
#define STREAM_NUMERICPARAMETER

#include "AbstractNumericParameter.h"

namespace stream
{
    template<typename data_t>
    class NumericParameter : public AbstractNumericParameter
    {
    public:
        NumericParameter(const std::string& name, const unsigned int id,
                         const stream::DataType& type,
                         const stream::Parameter::AccessMode inactiveAccess,
                         const stream::Parameter::AccessMode activeAccess,
                         const data_t& min, const data_t& max)
          : AbstractNumericParameter(name, id, type, inactiveAccess, activeAccess),
            m_min(min),
            m_max(max)
        {}
        
        NumericParameter(const std::string& name, const unsigned int id,
                         const stream::DataType& type,
                         const stream::Parameter::AccessMode inactiveAccess,
                         const stream::Parameter::AccessMode activeAccess)
          : AbstractNumericParameter(name, id, type, inactiveAccess, activeAccess),
            m_min(data_t::MIN),
            m_max(data_t::MAX)
        {}
        
        
        virtual const Data& max() const { return m_max; }
        
        virtual const Data& min() const { return m_min; }
        virtual void setMax(const data_t& value)
        {
            m_max = value;
        }
        
        virtual void setMin(const data_t& value)
        {
            m_min = value;
        }
        
    private:
        data_t m_min;
        data_t m_max;
    };
}


#endif // STREAM_NUMERICPARAMETER

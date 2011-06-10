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

#ifndef STREAM_ABSTRACTNUMERICPARAMETER
#define STREAM_ABSTRACTNUMERICPARAMETER

#include "Parameter.h"

namespace stream
{
    class Data;
    
    class AbstractNumericParameter : public Parameter
    {
    public:
        AbstractNumericParameter(const std::string& name, const unsigned int id,
                                 const stream::DataType& type,
                                 const stream::Parameter::AccessMode inactiveAccess,
                                 const stream::Parameter::AccessMode activeAccess)
          : Parameter(name, id, type, inactiveAccess, activeAccess)
        {}
        
        virtual const Data& max() const = 0;
        virtual const Data& min() const = 0;
    };
}


#endif // STREAM_ABSTRACTNUMERICPARAMETER

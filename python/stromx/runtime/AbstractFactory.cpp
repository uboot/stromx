/* 
*  Copyright 2013 Matthias Fuchs
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

#include <stromx/runtime/AbstractFactory.h>
#include <stromx/runtime/Data.h>
#include <stromx/runtime/Operator.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{
    struct AbstractFactoryWrap : AbstractFactory, wrapper<AbstractFactory>
    {
        Operator* newOperator(const std::string & package, const std::string & type) const
        {
            return this->get_override("newOperator")(package,type);
        }
        
        Data* newData(const std::string & package, const std::string & type) const
        {
            return this->get_override("newData")(package, type);
        }
        
        const std::vector<const OperatorKernel*> & availableOperators() const
        {
            return this->get_override("availableOperators")();
        }
    };
}

void exportAbstractFactory()
{          
    class_<AbstractFactoryWrap, boost::noncopyable>("AbstractFactory", no_init)
    ;
}
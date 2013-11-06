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

#ifndef STROMX_RUNTIME_ABSTRACTFACTORY_H
#define STROMX_RUNTIME_ABSTRACTFACTORY_H

#include <string>
#include <vector>

namespace stromx
{
    namespace runtime
    {
        class Data;
        class Operator;
        class OperatorKernel;
        
        /** \brief %Factory of operator and data objects. */
        class AbstractFactory
        {
        public:
            /** 
             * Allocates and returns a new operator. The factory of the operator
             * will be set to this factory.
             */
            virtual Operator* newOperator(const std::string & package, const std::string & type) const = 0;      
            
            /** Allocates and returns a new data object. */
            virtual Data* newData(const std::string & package, const std::string & type) const = 0;
            
            /** Returns a list of the operators registered with the factory. */
            virtual const std::vector<const OperatorKernel*> & availableOperators() const = 0;
        };
    }
}

#endif // STROMX_RUNTIME_ABSTRACTFACTORY_H

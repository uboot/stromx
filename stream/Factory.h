/* 
*  Copyright 2011 Thomas Fidler
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

#ifndef STREAM_FACTORY_H
#define STREAM_FACTORY_H

#include "Registry.h"

#include <string>
#include <vector>

namespace stream
{
    class Operator;
    
    class Factory : public Registry
    {
    public:
        virtual ~Factory();
        
        virtual void registerOperator(const OperatorKernel* const op);
        
        virtual OperatorKernel* const newOperator(const std::string & package, const std::string & name) const;
        
        virtual const std::vector<const OperatorKernel*> & availableOperators() const;
        
    private:
        std::vector<const OperatorKernel*> m_operators;
    };
}

#endif // STREAM_FACTORY_H

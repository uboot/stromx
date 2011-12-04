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


#ifndef STROMX_CORE_IMPL_NETWORK_H
#define STROMX_CORE_IMPL_NETWORK_H

#include <string>
#include <vector>
#include "../Output.h"
#include "../Input.h"

namespace stromx
{
    namespace core
    {
        class Operator;
        
        namespace impl
        {
            class InputNode;
            class OutputNode;
        
            class Network
            {
            public:
                enum Status
                {
                    INACTIVE,
                    ACTIVE
                };
                
                Network();
                ~Network();
                
                const std::vector<Operator*>& operators() const { return m_operators; }
                    
                void connect(const Output & source, const Input & target);
                void disconnect(const Input & target);
                const Output connectionSource(const Input & target) const;
                
                void addOperator(Operator* const op);
                void removeOperator(Operator* const op);

                void activate();
                void deactivate();
                
                InputNode* getInputNode(const Input & input) const;
                OutputNode* getOutputNode(const Output & output) const;
                    
            private:
                std::vector<Operator*> m_operators;
            };
        }
    }
}

#endif // STROMX_CORE_IMPL_NETWORK_H

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


#ifndef STROMX_RUNTIME_IMPL_NETWORK_H
#define STROMX_RUNTIME_IMPL_NETWORK_H

#include <string>
#include <vector>
#include "stromx/runtime/ExceptionObserver.h"
#include "stromx/runtime/OutputConnector.h"

namespace stromx
{
    namespace runtime
    {
        class Operator;
        class OperatorError;
        
        namespace impl
        {
            class InputNode;
            class OutputNode;
            
            class NetworkObserver
            {    
        public:
                virtual void observe(const OperatorError & ex, const ExceptionObserver::Phase phase) const = 0;
                
                virtual ~NetworkObserver() {}
            };
        
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
                    
                void connect(Operator* const sourceOp, const unsigned int outputId, 
                             Operator* const targetOp, const unsigned int inputId) ;
                void disconnect(Operator* const targetOp, const unsigned int inputId);
                const OutputConnector connectionSource(const Operator* const targetOp, const unsigned int inputId) const;
                
                void addOperator(Operator* const op);
                void removeOperator(Operator* const op);
                void removeInput(Operator* const op, const unsigned int inputId);
                void removeOutput(Operator* const op, const unsigned int outputId);

                void activate();
                void deactivate();
                void interrupt();
                
                InputNode* getInputNode(Operator* const op, const unsigned int inputId) const;
                OutputNode* getOutputNode(Operator* const op, const unsigned int outputId) const;
                
                void setObserver(const NetworkObserver* const observer);
                    
            private:
                std::vector<Operator*> m_operators;
                const NetworkObserver* m_observer;
            };
        }
    }
}

#endif // STROMX_RUNTIME_IMPL_NETWORK_H

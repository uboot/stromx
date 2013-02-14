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

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/Operator.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/impl/OutputNode.h"

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            OutputNode::OutputNode(Operator*const op, const unsigned int outputId)
            : m_operator(op),
                m_outputId(outputId),
                m_servedInputs(0)
            {}
            
            void OutputNode::addConnectedInput(InputNode*const input)
            {
                if(! input)
                    throw WrongArgument("Passed null as input.");
                
                if(m_connectedInputs.count(input))
                    throw WrongArgument("Input node has already been connected to this output node.");
                
                m_connectedInputs.insert(input);
            }

            void OutputNode::removeConnectedInput(InputNode*const input)
            {
                if(m_connectedInputs.count(input))
                    m_connectedInputs.erase(input);
            }

            DataContainer OutputNode::getOutputData()
            {
                DataContainer value;
                try
                {
                    value = m_operator->getOutputData(m_outputId);
                }
                catch(OperatorError & ex)
                {
                    ex.setName(m_operator->name());
                    throw;
                }
                    
                // the data has been obtained
                // now make sure the connection counter is adapted in an atomic operation
                {
                    lock_t lock(m_mutex);
                    
                    m_servedInputs++; 
                    
                    if(m_servedInputs >= m_connectedInputs.size())
                    {
                        m_operator->clearOutputData(m_outputId);
                        m_servedInputs = 0;
                    }
                    
                }
                
                return value;
            }
        
            void OutputNode::reset()
            {
                m_servedInputs = 0;
            }
        }

    }
}

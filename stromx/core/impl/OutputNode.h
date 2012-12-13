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

#ifndef STROMX_CORE_IMPL_OUTPUTNODE_H
#define STROMX_CORE_IMPL_OUTPUTNODE_H

#include <boost/thread/mutex.hpp>
#include <set>

namespace stromx
{
    namespace core
    {
        class Operator;
        class DataContainer;
        
        namespace impl
        {
            class InputNode;
        
            class OutputNode
            {
            public:
                OutputNode(Operator* const op, const unsigned int outputId);
                
                unsigned int outputId() const { return m_outputId; }
                Operator* const op() const { return m_operator; }
                DataContainer getOutputData();
                void addConnectedInput(InputNode* const input);
                void removeConnectedInput(InputNode* const input);
                const std::set<InputNode*> & connectedInputs() const { return m_connectedInputs; }
                
                /** 
                 * Resets the counter which tracks how many inputs have received the data 
                 * of the output node.
                 */
                void reset();
                
            private:
                typedef boost::lock_guard<boost::mutex> lock_t;
                
                Operator* m_operator;
                unsigned int m_outputId;
                std::set<InputNode*> m_connectedInputs;
                unsigned int m_servedInputs;
                
                boost::mutex m_mutex;  
            };
        }
    }
}

#endif // STROMX_CORE_IMPL_OUTPUTNODE_H
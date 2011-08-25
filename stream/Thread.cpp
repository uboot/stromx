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

#include "Thread.h"

#include "Exception.h"
#include "impl/Network.h"
#include "Operator.h"

#include "impl/InputNode.h"

namespace stream
{
    Thread::Thread(const stream::Network*const network)
      : m_network(network)
    {
        if(! network)
            throw WrongArgument("Passed null pointer as network.");
    }
        
    void Thread::addNode(Operator* const op, const unsigned int inputId)
    {
        InputNode* inputNode = m_network->getInputNode(op, inputId);
        m_thread.addNode(inputNode);
        m_nodeSequence.push_back(Node(op, inputId));
    }

    void Thread::insertNode(const unsigned int position, Operator* const op, const unsigned int inputId)
    {
        InputNode* inputNode = m_network->getInputNode(op, inputId);
        m_thread.insertNode(position, inputNode);
        m_nodeSequence.insert(m_nodeSequence.begin() + position, Node(op, inputId));
    }

    void Thread::removeNode(const unsigned int position)
    {
        m_thread.removeNode(position);
        m_nodeSequence.erase(m_nodeSequence.begin() + position);
    }
} 

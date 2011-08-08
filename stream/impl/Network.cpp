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


#include "Network.h"
#include "../Operator.h"
#include "../Exception.h"
#include "../Node.h"
#include "InputNode.h"
#include "OutputNode.h"

namespace stream
{
    Network::Network()
        :m_operators(0)
    {
    }
    
    Network::~Network()
    {
        for(std::vector<Operator*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            delete *iter;
        }

        m_operators.clear();
    }
    
    void Network::connect(Operator* const targetOp, const unsigned int inputId, 
                         Operator* const sourceOp, const unsigned int outputId)
    { 
        getInputNode(targetOp, inputId)->connect(getOutputNode(sourceOp, outputId));
    }

    void Network::disconnect(Operator* const targetOp, const unsigned int inputId)
    {
        getInputNode(targetOp, inputId)->disconnect();
    }
    
    void Network::activate()
    {
        for(std::vector<Operator*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            (*iter)->activate();
        }
    }

    void Network::deactivate()
    {
        for(std::vector<Operator*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            (*iter)->deactivate();
        }

    }
    
    void Network::addOperator(Operator* const op)
    {
        if (op == 0)
        {
            throw WrongArgument("Invalid argument: Null pointer");
        }
        
        if(op->status() != Operator::INITIALIZED)
            throw WrongArgument("Operator must be initialized.");
        
        for(std::vector<Operator*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            if ((*iter)->info() == op->info())
            {
                throw WrongArgument("Operator already exists");
            }
        }

        m_operators.push_back(op);
    }
    
    void Network::removeOperator(Operator*const op)
    {
        if (op == 0)
        {
            throw WrongArgument("Invalid argument: Null pointer");
        }

        for(std::vector<Operator*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            if ((*iter) == op)
            {
                m_operators.erase(iter);
                return;
            }
        }
        
        throw WrongArgument("Operator does not exist");
    }
    
    const Node Network::source(Operator* const targetOp, const unsigned int inputId) const
    {
        for(std::vector<Operator*>::const_iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            if ((*iter) == targetOp)
            {
                const OutputNode& node = targetOp->getInputNode(inputId)->source();
                return Node(node.op(), node.outputId());
            }
        }
        
        throw WrongArgument("Unknown operator.");
    }
    
    InputNode* Network::getInputNode(Operator* const op, const unsigned int inputId) const
    {
        std::vector<Operator*>::const_iterator iter = 
            std::find(m_operators.begin(), m_operators.end(), op);
        
        if(iter == m_operators.end())
            throw WrongArgument("Operator is not part of the stream.");
        
        return (*iter)->getInputNode(inputId);
    }
    
    OutputNode* Network::getOutputNode(Operator* const op, const unsigned int outputId) const
    {
        std::vector<Operator*>::const_iterator iter = 
            std::find(m_operators.begin(), m_operators.end(), op);
        
        if(iter == m_operators.end())
            throw WrongArgument("Operator is not part of the stream.");
        
        return (*iter)->getOutputNode(outputId);
    }
}
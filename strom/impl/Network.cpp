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

#include "InputNode.h"
#include "Network.h"
#include "OutputNode.h"
#include "../Exception.h"
#include "../Node.h"
#include "../Operator.h"

namespace strom
{
    class Description;
    
    namespace impl
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
        
        void Network::connect(Operator* const sourceOp, const unsigned int outputId, 
                              Operator* const targetOp, const unsigned int inputId) const
        { 
            getInputNode(targetOp, inputId)->connect(getOutputNode(sourceOp, outputId));
        }

        void Network::disconnect(Operator* const targetOp, const unsigned int inputId) const
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
                    // disconnect from all sources
                    for(std::vector<const Description*>::const_iterator desc = op->info()->inputs().begin();
                        desc != op->info()->inputs().end();
                        ++desc)
                    {
                        disconnect(op, (*desc)->id());
                    }
                    
                    
                    // disconnect all targets
                    for(std::vector<const Description*>::const_iterator desc = op->info()->outputs().begin();
                        desc != op->info()->outputs().end();
                        ++desc)
                    {
                        const OutputNode* output = op->getOutputNode((*desc)->id());
                        
                        // for each output disconnect each connected input
                        for(std::set<InputNode*>::iterator input = output->connectedInputs().begin();
                            input != output->connectedInputs().end();
                            ++input)
                        {
                            (*input)->disconnect();
                        } 
                    }
                        
                    m_operators.erase(iter);
                    return;
                }
            }
            
            throw WrongArgument("Operator does not exist");
        }
        
        const Node Network::source(const Operator* const targetOp, const unsigned int inputId) const
        {
            for(std::vector<Operator*>::const_iterator iter = m_operators.begin();
                iter != m_operators.end();
                ++iter)
            {
                if ((*iter) == targetOp)
                {
                    const InputNode* input = (*iter)->getInputNode(inputId);
                    
                    if(input->isConnected())
                        return Node(input->source().op(), input->source().outputId());
                    else
                        return Node();
                }
            }
            
            throw WrongArgument("Unknown operator.");
        }
        
        InputNode* Network::getInputNode(Operator* const op, const unsigned int inputId) const
        {
            std::vector<Operator*>::const_iterator iter = 
                std::find(m_operators.begin(), m_operators.end(), op);
            
            if(iter == m_operators.end())
                throw WrongArgument("Operator is not part of the strom.");
            
            return (*iter)->getInputNode(inputId);
        }
        
        OutputNode* Network::getOutputNode(Operator* const op, const unsigned int outputId) const
        {
            std::vector<Operator*>::const_iterator iter = 
                std::find(m_operators.begin(), m_operators.end(), op);
            
            if(iter == m_operators.end())
                throw WrongArgument("Operator is not part of the strom.");
            
            return (*iter)->getOutputNode(outputId);
        }
    }
}
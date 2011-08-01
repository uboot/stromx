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
#include "Operator.h"
#include "SynchronizedOperatorKernel.h"
#include "Exception.h"

namespace stream
{
    Network::Network()
        : m_status(INACTIVE)
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
    
    void Network::activate()
    {
        if (m_status == ACTIVE)
        {
            throw InvalidStateException("Network already active");
        }
        
        for(std::vector<Operator*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            (*iter)->kernel()->activate();
        }
        
        m_status = ACTIVE;
    }

    void Network::deactivate()
    {
        for(std::vector<Operator*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            (*iter)->kernel()->deactivate();
        }
        
        m_status = INACTIVE;
    }
    
    Operator*const Network::addOperator(OperatorKernel*const op)
    {
        for(std::vector<Operator*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            if ((*iter)->kernel()->info() == static_cast<const OperatorInfo*>(op))
            {
                throw ArgumentException("Operator already exists");
            }
        }

        Operator* node = new Operator(op);
        m_operators.push_back(node);
        return node;
    }
    
    void Network::removeOperator(Operator*const op)
    {
        if (op == 0)
        {
            throw ArgumentException("Invalid argument: Null pointer");
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
        
        throw ArgumentException("Operator does not exist");
    }
    
    Operator* const Network::getOperator(const std::string & name)
    {
        for(std::vector<Operator*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            if ((*iter)->name() == name)
            {
                return *iter;
            }
        }  
        
        throw InvalidStateException("Operator does not exist");
    }
}
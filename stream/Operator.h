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

#ifndef STREAM_OPERATOR_H
#define STREAM_OPERATOR_H

#include "DataContainer.h"

#include "impl/SynchronizedOperatorKernel.h"

namespace stream
{
    class OperatorInfo;
    class OperatorKernel;
    class Id2DataMapper;
    class Data;
    class InputNode;
    class OutputNode;
    
    class Operator
    {
        friend class Network;
        friend class Thread;
        friend class ThreadImplTest;
        friend class InputNodeTest;
        friend class OutputNodeTest;
        friend class NetworkTest;
        
    public:
        enum Status
        {
            NONE,
            INITIALIZED,
            ACTIVE,
            EXECUTING
        };
        
        Operator(OperatorKernel* const kernel);
        
        const std::string & name() const { return m_name; }
        void setName(const std::string & name) { m_name = name; }
        
        const OperatorInfo* const info() const { return m_kernel->info(); }
        const Status status() { return Status(m_kernel->status()); }
        void setParameter(unsigned int id, const Data& value) { m_kernel->setParameter(id, value); }
        const Data& getParameter(unsigned int id) { return m_kernel->getParameter(id); }
        DataContainer getOutputData(const unsigned int id) { return m_kernel->getOutputData(id); }
        void setInputData(const unsigned int id, DataContainer data) { m_kernel->setInputData(id, data); }
        void clearOutputData(unsigned int id) { m_kernel->clearOutputData(id); }
        void initialize();
        
    private:
        ~Operator();
        
        InputNode* const getInputNode(const unsigned int id);
        OutputNode* const getOutputNode(const unsigned int id);
        void activate(){ m_kernel->activate(); }
        void deactivate(){ m_kernel->deactivate(); }
        
        std::string m_name;
        SynchronizedOperatorKernel* m_kernel;
        std::map<unsigned int, OutputNode*> m_outputs;
        std::map<unsigned int, InputNode*> m_inputs;
    };
}

#endif // STREAM_OPERATOR_H

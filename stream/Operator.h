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
#include "OperatorInfo.h"

#include <map>
#include <string>

namespace stream
{
    class OperatorInfo;
    class OperatorKernel;
    class Id2DataMapper;
    class Data;
    
    namespace impl
    {
        class Network;
        class InputNode;
        class OutputNode;
        class SynchronizedOperatorKernel;
    }
    
    class Operator
    {
        friend class impl::Network;
        friend class Thread;
        friend class OperatorTester;
        friend class ThreadImplTest;
        friend class InputNodeTest;
        friend class OutputNodeTest;
        friend class NetworkTest;
        friend class FactoryTest;
        
    public:
        enum Status
        {
            NONE,
            INITIALIZED,
            ACTIVE,
            EXECUTING
        };
        
        Operator(OperatorKernel* const kernel);
        virtual ~Operator();
        
        const std::string & name() const { return m_name; }
        void setName(const std::string & name) { m_name = name; }
        
        const OperatorInfo* const info() const;
        const Status status();
        void setParameter(unsigned int id, const Data& value);
        const Data& getParameter(unsigned int id) const;
        DataContainer getOutputData(const unsigned int id);
        void setInputData(const unsigned int id, DataContainer data);
        void clearOutputData(unsigned int id);
        void initialize();
        
    private:
        impl::InputNode* const getInputNode(const unsigned int id);
        impl::OutputNode* const getOutputNode(const unsigned int id);
        void activate();
        void deactivate();
        
        std::string m_name;
        impl::SynchronizedOperatorKernel* m_kernel;
        std::map<unsigned int, impl::OutputNode*> m_outputs;
        std::map<unsigned int, impl::InputNode*> m_inputs;
    };
}

#endif // STREAM_OPERATOR_H

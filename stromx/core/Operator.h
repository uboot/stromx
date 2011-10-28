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

#ifndef STROM_OPERATOR_H
#define STROM_OPERATOR_H

#include <map>
#include <string>
#include "DataContainer.h"
#include "Exception.h"
#include "OperatorInfo.h"

namespace stromx
{
    namespace core
    {
        class Data;
        class Id2DataMapper;
        class OperatorInfo;
        class OperatorKernel;
        
        namespace impl
        {
            class InputNode;
            class Network;
            class OutputNode;
            class SynchronizedOperatorKernel;
        }
        
        /** \brief Abstract operator */
        class Operator
        {
            friend class FactoryTest;
            friend class impl::Network;
            friend class InputNodeTest;
            friend class NetworkTest;
            friend class OperatorTester;
            friend class OutputNodeTest;
            friend class ThreadImplTest;
            friend class Thread;
            
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
            const Status status() const;
            void setParameter(const unsigned int id, const Data& value);
            const Data& getParameter(const unsigned int id) const;
            
            template<typename data_t>
            const data_t& getParameter(unsigned int id) const
            {
                try
                {
                    return dynamic_cast<const data_t &>(getParameter(id));
                }
                catch(std::bad_cast &)
                {
                    throw BadCast();
                }
            }
            
            const DataContainer getOutputData(const unsigned int id) const;
            void setInputData(const unsigned int id, const DataContainer data);
            void clearOutputData(const unsigned int id);
            void initialize();
            
        private:
            impl::InputNode* const getInputNode(const unsigned int id) const;
            impl::OutputNode* const getOutputNode(const unsigned int id) const;
            void activate();
            void deactivate();
            
            std::string m_name;
            impl::SynchronizedOperatorKernel* m_kernel;
            std::map<unsigned int, impl::OutputNode*> m_outputs;
            std::map<unsigned int, impl::InputNode*> m_inputs;
        };
    }
}

#endif // STROM_OPERATOR_H

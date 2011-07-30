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


#ifndef STREAM_NETWORK_H
#define STREAM_NETWORK_H

#include <string>
#include <vector>

namespace stream
{
    class OperatorKernel;
    class SynchronizedOperatorKernel;
    class OperatorNode;
    
    class Network
    {
    public:
        enum Status
        {
            INACTIVE,
            ACTIVE
        };
        
        Network();
        virtual ~Network();
        
        const std::string & name() { return m_name; }
        void setName(const std::string name) { m_name = name; }
        const Status status() { return m_status; }
        const std::vector<OperatorNode*> operators() { return m_operators; }
        
        OperatorNode* const addOperator(SynchronizedOperatorKernel* const op);
        OperatorNode* const addOperator(OperatorKernel* const op);
        void removeOperator(OperatorNode* const op);
        OperatorNode* const getOperator(const std::string & name);
        void activate();
        void deactivate();
               
    private:
        std::string m_name; 
        Status m_status;
        std::vector<OperatorNode*> m_operators;
    };
}

#endif // STREAM_NETWORK_H

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

#ifndef STROMX_RUNTIME_IMPL_INPUTNODE_H
#define STROMX_RUNTIME_IMPL_INPUTNODE_H

namespace stromx
{
    namespace runtime
    {
        class DataContainer;
        class Operator;
        
        namespace impl
        {
            class OutputNode;
        
            class InputNode
            {
        public:
                InputNode(Operator* const op, const unsigned int inputId);
                
                unsigned int inputId() const { return m_inputId; }
                Operator* op() const { return m_operator; }
                bool isConnected() const { return m_source != 0; }
                const OutputNode& source() const;
                void connect(OutputNode* const output);
                void disconnect();
                void setInputData();
                
            private:
                OutputNode *m_source;
                unsigned int m_inputId;
                Operator* m_operator;
            };
        }
    }
}

#endif // STROMX_RUNTIME_IMPL_INPUTNODE_H

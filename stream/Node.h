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

#ifndef STREAM_NODE_H
#define STREAM_NODE_H

namespace stream
{
    class Operator;
    
    class Node
    {
    public:
        Node(Operator* const op, const unsigned int id) : m_op(op), m_id(id)
        {}
        
        Operator* const op() const { return m_op; }
        const unsigned int id() const { return m_id; }
        
    private:
        Operator* m_op;
        unsigned int m_id;
    };
}

#endif // STREAM_NODE_H
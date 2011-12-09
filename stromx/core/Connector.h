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

#ifndef STROMX_CORE_CONNECTOR_H
#define STROMX_CORE_CONNECTOR_H

namespace stromx
{
    namespace core
    {
        class Operator;
        
        /** \brief Identifier of a connector of an operator. */
        class Connector
        {
        public:
            enum Type
            {
                UNDEFINED,
                INPUT,
                OUTPUT
            };
            
            Connector()
              : m_type(UNDEFINED),
                m_op(0),
                m_id(0)
            {}
            
            const Operator* const op() const { return m_op; }
            const unsigned int id() const { return m_id; }
            const bool empty() const { return m_op == 0; }
            const Type type() const { return m_type; }
            
        protected:
            Connector(const Type type, const Operator* const op, const unsigned int id)
              : m_type(type),
                m_op(op),
                m_id(id)
            {}
            
        private:
            Type m_type;
            const Operator* m_op;
            unsigned int m_id;
        };
    }
}

#endif // STROMX_CORE_CONNECTOR_H

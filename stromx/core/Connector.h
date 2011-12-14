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
            /** The possible types of a connector. */
            enum Type
            {
                /** An invalid connector has undefined type. */
                UNDEFINED,
                /** An input connector. */
                INPUT,
                /** An output connector. */
                OUTPUT
            };
            
            /** 
             * Constructs an invalid connector. Valid connectors can only be constructed
             * using the derived classes Input and Output.
             */
            Connector()
              : m_type(UNDEFINED),
                m_op(0),
                m_id(0)
            {}
            
            /** Returns a pointer to the operator the connector belongs to. */
            const Operator* const op() const { return m_op; }
            
            /** Returns the ID of the connector. */
            const unsigned int id() const { return m_id; }
            
            /** 
             * Returns \c true if this connector is valid, i.e. it does
             * point to a valid connector. If the connector is invalid its
             * operator pointer is 0 and its type UNDEFINED.
             */
            const bool valid() const { return m_op != 0; }
            
            /**
             * Returns the type of the connector, i.e. whether this connector
             * is an input or output connector.
             */
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

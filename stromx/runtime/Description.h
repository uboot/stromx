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

#ifndef STROMX_RUNTIME_DESCRIPTION_H
#define STROMX_RUNTIME_DESCRIPTION_H

#include "stromx/runtime/DescriptionBase.h"

namespace stromx
{
    namespace runtime
    {
        
        /** \brief %Description of a connector.
         *
         * In addition to the information stored in the parent class
         * descriptions store threading properties of connectors.
         */
        class STROMX_RUNTIME_API Description : public DescriptionBase
        {
        public:
            /** Constructs a description. */
            Description(const unsigned int id, const VariantHandle& variant)
              : DescriptionBase(id, variant),
                m_operatorThread(0),
                m_defaultType(CONNECTOR)
            {}
            
            /** Returns the operator thread of this connector. 
             * 
             * \sa setOperatorThread()
             */
            unsigned int operatorThread() const
            {
                return m_operatorThread;
            }
            
            /** 
             * Sets the operator thread of this connector.
             * 
             * The operator thread of a connector specifies which thread can
             * be assigned to the connector in the context of the operator the
             * connector belongs to. If e.g. an operator has one input and one 
             * output and produces one data object at the output if and only if
             * one data object is given to the input than it is safe to assign
             * the same operator thread to the input and the output. I.e. the 
             * the operator threads of both the input and the output can have
             * their default values 0.
             * 
             * If the operator produces one data object at the output only after
             * it has received a couple of input objects at the input then
             * a thread handling the input and the output would dead-lock
             * waiting for the output after the input has been set.
             * Setting the operator thread values of the input and the output
             * tells an algorithm which automatically assigns threads to the
             * stream that this situation must be avoided.
             */
            void setOperatorThread(const unsigned int operatorThread)
            { 
                m_operatorThread = operatorThread;
            }
            
            /**
             * Input and output connectors can be configured to act as operator
             * parameters. Such a connector is listed as an operator parameter.
             * If the default type of a connectors is DescriptionBase::PARAMETER
             * it will be a parameter upon the first initialization of the operator.
             * It can however be converted back to a connector by the client.
             */
            Type defaultType() const { return m_defaultType; }
            
            /**
             * Sets the default type.
             * 
             * \sa defaultType
             */
             void setDefaultType(const Type type) { m_defaultType = type; }
            
        private:
            unsigned int m_operatorThread;
            Type m_defaultType;
        };
    }
}

#endif // STROMX_RUNTIME_DESCRIPTION_H
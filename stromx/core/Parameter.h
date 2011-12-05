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

#ifndef STROMX_CORE_PARAMETER_H
#define STROMX_CORE_PARAMETER_H

#include "Description.h"

namespace stromx
{
    namespace core
    {
        /** \brief %Description of a parameter.
         * 
         * In addition to the information stored in a Description object
         * parameter descriptions store information about the access mode
         * of a given parameter.
         */
        class Parameter : public Description
        {
        public:
            enum AccessMode
            {
                /** The parameter can never be accessed. */
                NO_ACCESS,
                /** The parameter can be read during each status of the operator. */
                NONE_READ,
                /** 
                 * The parameter can be read during each status of the operator.
                 * It can be written during the status Operator::NONE.
                 */
                NONE_WRITE,
                /** 
                 * The parameter can be read during the status Operator::INITIALIZED,
                 * Operator::ACTIVE and Operator::EXECUTING.
                 */
                INITIALIZED_READ,
                /** 
                 * This parameter can be read during the status Operator::INITIALIZED,
                 * Operator::ACTIVE and Operator::EXECUTING.
                 * It can be written during the status Operator::INITIALIZED.
                 */
                INITIALIZED_WRITE,
                /** 
                 * This parameter can be read during the status Operator::INITIALIZED,
                 * Operator::ACTIVE and Operator::EXECUTING.
                 * It can be written during the status Operator::INITIALIZED and 
                 * Operator::ACTIVE.
                 */
                ACTIVATED_WRITE
            };
            
            /** Constructs a parameter description. */
            Parameter(const unsigned int id, const DataVariant& variant)
            : Description(id, variant),
                m_access(NO_ACCESS)
            {}
            
            /** Returns the access mode. */
            const AccessMode accessMode() const { return m_access; }
            
            /** Sets the access mode. */
            void setAccessMode(const AccessMode mode) { m_access = mode; }
            
        private:
            AccessMode m_access;
        };
    }
}

#endif // STROMX_CORE_PARAMETER_H
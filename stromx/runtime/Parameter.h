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

#ifndef STROMX_RUNTIME_PARAMETER_H
#define STROMX_RUNTIME_PARAMETER_H

#include "stromx/runtime/Description.h"

namespace stromx
{
    namespace runtime
    {
        class ParameterGroup;
        
        /** \brief %Description of a parameter.
         * 
         * In addition to the information stored in a Description object
         * parameter descriptions store information about the access mode
         * of a given parameter.
         */
        class STROMX_RUNTIME_API Parameter : public Description
        {
        public:
            /**
             * The possible access modes of a parameter.
             * 
             * <table>
             *     <tr>
             *       <td>Status</td>
             *       <td>%None</td>
             *       <td>Intialized</td>
             *       <td>Active</td>
             *       <td>Executing</td>
             *     </tr>
             *     <tr>
             *       <td>NO_ACCESS</td>
             *       <td>-</td>
             *       <td>-</td>
             *       <td>-</td>
             *       <td>-</td>
             *     </tr>
             *     <tr>
             *       <td>NONE_READ</td>
             *       <td>R</td>
             *       <td>R</td>
             *       <td>R</td>
             *       <td>R</td>
             *     </tr>
             *     <tr>
             *       <td>NONE_WRITE</td>
             *       <td>R/W</td>
             *       <td>R</td>
             *       <td>R</td>
             *       <td>R</td>
             *     </tr>
             *     <tr>
             *       <td>INITIALIZED_READ</td>
             *       <td>-</td>
             *       <td>R</td>
             *       <td>R</td>
             *       <td>R</td>
             *     </tr>
             *     <tr>
             *       <td>INITIALIZED_WRITE</td>
             *       <td>-</td>
             *       <td>R/W</td>
             *       <td>R</td>
             *       <td>R</td>
             *     </tr>
             *     <tr>
             *       <td>ACTIVATED_WRITE</td>
             *       <td>-</td>
             *       <td>R/W</td>
             *       <td>R/W</td>
             *       <td>R/W</td>
             *     </tr>
             * </table>
             * 
             */
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
                 * It can be written during the status Operator::INITIALIZED,  
                 * Operator::ACTIVE and Operator::EXECUTING.
                 */
                ACTIVATED_WRITE
            };
            
            /** Constructs a parameter description. */
            Parameter(const unsigned int id, const VariantHandle& variant, ParameterGroup* const group = 0);
            
            /** Returns the access mode. */
            AccessMode accessMode() const { return m_access; }
            
            /** Sets the access mode. */
            void setAccessMode(const AccessMode mode) { m_access = mode; }
            
            /** 
             * Returns the group the parameter belongs to. Returns 0 if the parameter
             * does not belong to any group.
             */
            virtual const Parameter* group() const { return m_group; }
            
            /**
             * Returns the members of this parameter group. If the parameter is not 
             * a parameter group or it has no members the returned list is empty.
             */
            virtual const std::vector<const Parameter*> & members() const { return NO_MEMBERS; }
            
        private:
            static const std::vector<const Parameter*> NO_MEMBERS;
            
            AccessMode m_access;
            const Parameter* m_group;
        };
    }
}

#endif // STROMX_RUNTIME_PARAMETER_H
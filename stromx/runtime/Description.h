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

#ifndef STROMX_RUNTIME_DESCRIPTOR_H
#define STROMX_RUNTIME_DESCRIPTOR_H

#include <string>
#include "stromx/runtime/Config.h"
#include "stromx/runtime/DataVariant.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief %Description of an ID to data variant map.
         * 
         * Instances of this class associate an ID with a data variant.
         * Input nodes, output nodes and parameters of operators are characterized
         * by such a description. In case of input and output nodes the data variant defines
         * which data type can be passed to an input node or is to be expected from
         * an output node. In case of parameter it characterizes the data type of the 
         * respective paramter. In addition, a Description can have a name which is 
         * purely informal and does not have any influence on its behavior.
         */
        class Description
        {
        public:
            /** Constructs a description. */
            Description(const unsigned int id, const DataVariant& variant)
            : m_id(id),
              m_variant(variant)
            {}
            
            virtual ~Description() {}
            
            /** Returns the title. */
            const std::string & title() const { return m_title; }
            
            /** Returns the ID. */
            unsigned int id() const { return m_id; }
            
            /** Returns the data variant. */
            const DataVariant & variant() const { return m_variant; }
            
            /** Sets the title of the description. */
            void setTitle(const std::string & title) { m_title = title; }
            
        private:
            std::string m_title;
            unsigned int m_id;
            DataVariant m_variant;
        };
    }
}

#endif // STROMX_RUNTIME_DESCRIPTOR_H
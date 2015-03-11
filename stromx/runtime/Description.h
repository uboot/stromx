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

#include <vector>

#include <string>
#include "stromx/runtime/Config.h"
#include "stromx/runtime/VariantHandle.h"
#include "stromx/runtime/Enum.h"
#include "stromx/runtime/None.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief %Description of an enumeration value. */
        class EnumDescription
        {
        public:
            /** Constructs an enumeration description. */
            explicit EnumDescription(const Enum & value)
              : m_value(value)
            {}
            
            /** Constructs an enumeration description with a documentation title . */
            EnumDescription(const Enum & value, const std::string & title)
              : m_value(value),
                m_title(title)
            {}
            
            /** Returns the value. */
            const Enum value() const { return m_value; }
            
            /** Returns the description title. */
            const std::string & title() const { return m_title; }
            
        private:
            Enum m_value;
            std::string m_title;
        };
        
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
        class STROMX_RUNTIME_API Description
        {
        public:
            /** Constructs a description. */
            Description(const unsigned int id, const VariantHandle& variant)
            : m_id(id),
              m_variant(variant)
            {}
            
            virtual ~Description() {}
            
            /** Returns the title. */
            const std::string & title() const { return m_title; }
            
            /** Returns the ID. */
            unsigned int id() const { return m_id; }
            
            /** Returns the data variant. */
            const VariantHandle & variant() const { return m_variant; }
            
            /** Sets the title of the description. */
            void setTitle(const std::string & title) { m_title = title; }
            
                        /** Returns the maximal value of this parameter or an instance of None. */
            virtual const Data& max() const { return NONE; }
            
            /** Returns the minimal value of this parameter or an instance of None. */
            virtual const Data& min() const { return NONE; }
            
            /** 
             * Returns a useful step size to decrease or increase the value of this
             * parameter. Returns NONE if no step size is defined for this parameter.
             */
            virtual const Data& step() const { return NONE; }
            
            /** Returns the possible values of an enumeration value or an empty vector. */
            virtual const std::vector<EnumDescription> & descriptions() const { return NO_DESCRIPTIONS; }
            
            /** 
             * Returns the required number of rows of a matrix parameters. Returns
             * 0 if any number of rows is possible.
             */
            virtual unsigned int rows() const { return 0; }
            
            /** 
             * Returns the required number of columns of a matrix parameters. Returns
             * 0 if any number of rows is possible.
             */
            virtual unsigned int cols() const { return 0; }
            
        private:
            static const std::vector<EnumDescription> NO_DESCRIPTIONS;
            static const None NONE;
            
            std::string m_title;
            unsigned int m_id;
            VariantHandle m_variant;
        };
    }
}

#endif // STROMX_RUNTIME_DESCRIPTION_H
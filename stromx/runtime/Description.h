/* 
 *  Copyright 2015 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_DESCRIPTIONBASE_H
#define STROMX_RUNTIME_DESCRIPTIONBASE_H

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
        
        /** \brief Abstract description of an ID to variant map.
         * 
         * Instances of this class associate an ID with a data variant. Moreover
         * the contain meta information of the data characterized by this mapping.
         * Input connectors, output connectors and parameters of operators are characterized
         * by such a description. In case of input and output nodes the data variant defines
         * which data type can be passed to an input node or is to be expected from
         * an output node. In case of parameter it characterizes the data type of the 
         * respective paramter. In addition, a Description can have a name which is 
         * purely informal and does not have any influence on its behavior.
         */
        class STROMX_RUNTIME_API Description
        {
        public:
            /**  Different types objects which are annotated by an description. */
            enum Type 
            {
                /** Undefined type.*/
                NO_TYPE,
                /** Operator parameter. */ 
                PARAMETER,
                /** Input connector. */
                INPUT,
                /** Output connector. */
                OUTPUT
            };
            
            /** The update behavior of operator parameters. */
            enum UpdateBehavior
            {
                /** 
                 * The value of this parameter will only change when it is 
                 * explicitly set by Operator::setParameter(). In general, it 
                 * is not necessary to set a persistent parameter to the 
                 * same value more than once as it will hold the correct value
                 * after the first change. In the same spirit a read value can
                 * be considered to be "real" value until the parameter is 
                 * changed by the user.
                 * 
                 * This is the default behavior of parameters.
                 */
                PERSISTENT,
                /**
                 * The value of this parameter can be set but reading the 
                 * parameter does not necessarily return a meaningful result. 
                 * E.g. setting a trigger parameter causes an action to be
                 * performed whenever the parameter is set. 
                 * In general it makes sense set send parameters more than once.
                 */
                PUSH,
                /** 
                 * Snapshots of the value of this parameter can be read but
                 * setting it does not have a meaningful effect. E.g. the
                 * current state of a classifier operator which learns while
                 * being executed could be queried from a pull parameters.
                 * In general it makes to frequently read receive parameters.
                 */
                PULL,
                /** 
                 * This parameter is only internally used by the operator. It
                 * makes no sense to read or write its values from outside. In 
                 * a GUI an internal parameter would not be displayed.
                 */
                INTERNAL
            };
            
            /** Constructs a description. */
            Description(const unsigned int id, const VariantHandle& variant);
            
            virtual ~Description() {}
            
            /** Returns the title. */
            const std::string & title() const { return m_title; }
            
            /** Returns the ID. */
            unsigned int id() const { return m_id; }
            
            /** Returns the data variant of this value. */
            const VariantHandle & variant() const { return m_variant; }
            
            /** Sets the title of the description. */
            void setTitle(const std::string & title) { m_title = title; }
            
            /** Returns how this data value should be visualized or an instance of Variant::None. */
            const VariantHandle & visualization() const { return m_visualization; }
            
            /** Sets the visualization of this description. */
            void setVisualization(const VariantHandle & visualization);
            
            /** Returns the maximal value of this data value or an instance of None. */
            virtual const Data& max() const { return NONE; }
            
            /** Returns the minimal value of this data value or an instance of None. */
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
             * 0 if any number of columns is possible.
             */
            virtual unsigned int cols() const { return 0; }
            
            /** 
             * Input and output connectors can be configured to act as operator
             * parameters. Such a connector is listed as an operator parameter.
             * Its original type however remains that of a connector and signals
             * the client that the parameter can be configured back to a connector
             * if necessary.
             */
            virtual Type originalType() const { return NO_TYPE; }
            
            /** 
             * Input and output connectors can be configured to act as operator
             * parameters. Such a connector is listed as an operator parameter. The 
             * current type always refers to way the connector is currently configured,
             * i.e. possibly as a parameter.
             */
            virtual Type currentType() const { return NO_TYPE; }
            
        private:
            static const std::vector<EnumDescription> NO_DESCRIPTIONS;
            static const None NONE;
            
            std::string m_title;
            unsigned int m_id;
            VariantHandle m_variant;
            VariantHandle m_visualization;
        };
    }
}

#endif // STROMX_RUNTIME_DESCRIPTIONBASE_H
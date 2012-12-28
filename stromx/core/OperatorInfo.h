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

#ifndef STROMX_CORE_OPERATORINFO_H
#define STROMX_CORE_OPERATORINFO_H

#include <vector>
#include "Parameter.h"
#include "Version.h"

namespace stromx
{
    namespace core
    {
        /**
         * \brief Meta-information about an operator
         * 
         * This class contains all the information how to use an operator without 
         * looking at its API documentaion. This information reaches its final status
         * only after Operator::initialize() is called.
         */
        class OperatorInfo
        {
        public:
            /** Returns the type. In general this is the name of the class of its implementation. */
            virtual const std::string& type() const = 0;
            
            /** Returns the package the operator belongs to. */
            virtual const std::string& package() const = 0;
            
            /** Returns the version of the operator. */
            virtual const Version& version() const = 0;
            
            /**
             * Returns a list of the inputs of the operator. This list can
             * change when Operator::initialize() is called.
             */
            virtual const std::vector<const Description*>& inputs() const = 0;
            
            /**
             * Returns a list of the outputs of the operator. This list can
             * change when Operator::initialize() is called. After that it is
             * constant.
             */
            virtual const std::vector<const Description*>& outputs() const = 0;
            
            /**
             * Returns a list of the parameters of the operator. This list can only
             * change when Operator::initialize() is called. After that it is
             * constant with exception of the properties (however, not their ID and data
             * variant) of the single parameters.
             */
            virtual const std::vector<const Parameter*>& parameters() const = 0;
            
            /**
             * Returns the input description of the input \c id.
             */
            virtual const Description & output(const unsigned int id) const = 0;
            
            /**
             * Returns the output description of the output \c id.
             */
            virtual const Description & input(const unsigned int id) const = 0;
            
            /**
             * Returns the parameter description of the parameter \c id.
             */
            virtual const Parameter & parameter(const unsigned int id) const = 0;
        };
    }
}

#endif // STROMX_CORE_OPERATORINFO_H

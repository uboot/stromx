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

#ifndef STROMX_CORE_OPERATORKERNEL_H
#define STROMX_CORE_OPERATORKERNEL_H

#include <map>
#include "OperatorInfo.h"

namespace stromx
{
        namespace core
    {
        class Data;
        class DataContainer;
        class DataProvider;
        
        namespace impl
        {
            class SynchronizedOperatorKernel;
        }
        
        /**
         * \brief Abstract operator kernel
         * 
         * An operator kernel implements an operator in its raw form, i.e. without
         * the infrastructure code which is necessary to be a node in a stream network.
         * It is never directly used in a stream but serves as a base class for the 
         * implementation of custom operators. 
         */
        class OperatorKernel : public OperatorInfo
        {
            friend class impl::SynchronizedOperatorKernel;
            
        public:
            /**
             * Constructs an operator kernel
             */
            OperatorKernel(const std::string & type,
                    const std::string & package,
                    const Version & version,
                    const std::vector<const Parameter*>& parameters);
                
            /**
             * Constructs an operator kernel
             */    
            OperatorKernel(const std::string & type,
                    const std::string & package,
                    const Version & version,
                    const std::vector<const Description*>& inputs,
                    const std::vector<const Description*>& outputs,
                    const std::vector<const Parameter*>& parameters);
             
            /**
             * Constructs an operator kernel
             */       
            OperatorKernel(const std::string & type,
                    const std::string & package,
                    const Version & version);
            
            /**
             * Returns a copy of the operator, i.e. a object of the same type is
             * allocated and returned. The current status of the operator is \em not
             * reproduced.
             */
            virtual OperatorKernel* const clone() const = 0;
            virtual ~OperatorKernel();
            
            /** Returns the type. In general this is the name of the class of its implementation. */
            const std::string& type() const { return m_type; }
            
            /** Returns the package the operator belongs to */
            const std::string& package() const { return m_package; }
            
            /** Returns the version of the operator */
            const Version& version() const { return m_version; }

            /**
             * Returns a list of the inputs of the operator. This list can
             * change when OperatorKernel::initialize() is called. After that it is
             * constant.
             */
            const std::vector<const Description*>& inputs() const { return m_inputs; }
            
            /**
             * Returns a list of the outputs of the operator. This list can
             * change when Operator::initialize() is called. After that it is
             * constant.
             */
            const std::vector<const Description*>& outputs() const { return m_outputs; }
            
            /**
             * Returns a list of the outputs of the operator. This list can only
             * change when Operator::initialize() is called. After that it is
             * constant.
             */
            const std::vector<const Parameter*>& parameters() const { return m_parameters; }
            
            /**
             * Returns the parameter description of the parameter \c id
             */
            const Parameter & parameter(const unsigned int id) const;
            
            /** Sets a parameter \c id to \c value */
            virtual void setParameter(const unsigned int id, const Data& value) {};
            
            /** Gets the current value of the parameter \c id */
            virtual const Data& getParameter(const unsigned int id) const {};
            
            virtual void execute(DataProvider& provider) = 0;
            
            /** 
             * Initializes the operator. After initialization the operator kernel must
             * be prepared to accept calls to activate(). An operator kernel can only be
             * initialized once, i.e. its state can never be reverted to not-initialzed.
             */
            virtual void initialize() {}

            /** 
             * Activates the operator. After activation the operator kernel must
             * be prepared to accept calls to execute().
             */
            virtual void activate() {}

            /** 
             * Deactivates the operator, i.e. returns its state to the same as before
             * activation
             */
            virtual void deactivate() {}
            
        protected:
            virtual void initialize(const std::vector<const Description*>& inputs,
                                    const std::vector<const Description*>& outputs,
                                    const std::vector<const Parameter*>& parameters);
            
            Parameter & parameter(const unsigned int id);
            
        private:
            static void validate(const std::vector<const Description*>& descriptors);
            static void validate(const std::vector<const Parameter*>& descriptors);
            
            std::string m_type;
            std::string m_package;
            Version m_version;
            std::vector<const Description*> m_inputs;
            std::vector<const Description*> m_outputs;
            std::vector<const Parameter*> m_parameters;
            std::map<unsigned int, const Parameter*> m_parameterMap;
        };
    }
}

#endif // STROMX_CORE_OPERATORKERNEL_H

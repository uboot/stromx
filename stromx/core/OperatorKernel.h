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
#include <set>
#include "DataRef.h"
#include "Exception.h"
#include "OperatorInfo.h"

namespace stromx
{
        namespace core
    {
        class DataContainer;
        class DataProvider;
        
        namespace impl
        {
            class SynchronizedOperatorKernel;
        }
        
        /**
         * \brief Abstract operator kernel.
         * 
         * An operator kernel implements an operator in its raw form, i.e. without
         * the infrastructure code which is necessary to be a node in a stream network.
         * It is never directly used in a stream but serves as a base class for the 
         * implementation of custom operators. 
         */
        class STROMX_CORE_API OperatorKernel : public OperatorInfo
        {
            friend class impl::SynchronizedOperatorKernel;
            friend class OperatorKernelTest;
            
        public:
            virtual ~OperatorKernel();
            
            /**
             * Returns a copy of the operator, i.e. a object of the same type is
             * allocated and returned. The current status of the operator is \em not
             * reproduced.
             */
            virtual OperatorKernel* const clone() const = 0;
            
            const std::string& type() const { return m_type; }
            
            const std::string& package() const { return m_package; }
            
            const Documentation& doc() const { return m_doc; }
            
            const Version& version() const { return m_version; }

            const std::vector<const Description*>& inputs() const { return m_inputs; }
            
            const std::vector<const Description*>& outputs() const { return m_outputs; }
            
            const std::vector<const Parameter*>& parameters() const { return m_parameters; }
            
            const Parameter & parameter(const unsigned int id) const;
            
            virtual const Description & input(const unsigned int id) const;
            
            virtual const Description & output(const unsigned int id) const;
            
            /** Sets a parameter \c id to \c value. */
            virtual void setParameter(const unsigned int id, const Data& value) { throw NotImplemented(); };
            
            /** Gets the current value of the parameter \c id. */
            virtual const DataRef getParameter(const unsigned int id) const { throw NotImplemented(); };
            
            /**
             * Executes the operator.
             * \param provider Provides the input data for the execution and accepts its results.
             */
            virtual void execute(DataProvider& provider) = 0;
            
            /** 
             * Initializes the operator. After initialization the operator kernel must
             * be prepared to accept calls to activate().
             */
            virtual void initialize() {}
            
            /**
             * Deinitializes the operator. Must be called from
             * any overloads of this function in derived classes.
             */
            virtual void deinitialize();

            /** 
             * Activates the operator. After activation the operator kernel must
             * be prepared to accept calls to execute().
             */
            virtual void activate() {}

            /** 
             * Deactivates the operator, i.e. returns its state to the same as before
             * activation.
             */
            virtual void deactivate() {}
            
            
        protected:
            /**
             * Constructs an operator kernel.
             */
            OperatorKernel(const std::string & type,
                    const std::string & package,
                    const Version & version,
                    const std::vector<const Parameter*>& parameters);
                
            /**
             * Constructs an operator kernel.
             */    
            OperatorKernel(const std::string & type,
                    const std::string & package,
                    const Version & version,
                    const std::vector<const Description*>& inputs,
                    const std::vector<const Description*>& outputs,
                    const std::vector<const Parameter*>& parameters);
             
            /**
             * Constructs an operator kernel.
             */       
            OperatorKernel(const std::string & type,
                    const std::string & package,
                    const Version & version);
            /**
             * Initializes an operator kernel. Must only be called from
             * OperatorKernel::initialize() and overloads of this function
             * in derived classes.
             */
            virtual void initialize(const std::vector<const Description*>& inputs,
                                    const std::vector<const Description*>& outputs,
                                    const std::vector<const Parameter*>& parameters);
            
            /**
             * Returns the parameter \c id.
             */
            Parameter & parameter(const unsigned int id);
            
            /** Sets the documentation. */
            void setDoc(const Documentation & doc);
            
        private:
            void validateInputs(const std::vector<const Description*>& descriptors);
            void validateOutputs(const std::vector<const Description*>& descriptors);
            void validateParameters(const std::vector<const Parameter*>& descriptors);
            const Parameter & findParameter(const unsigned int id) const;
            
            std::string m_type;
            std::string m_package;
            Version m_version;
            Documentation m_doc;
            std::vector<const Description*> m_inputs;
            std::vector<const Description*> m_outputs;
            std::vector<const Parameter*> m_parameters;
            
            std::map<unsigned int, const Parameter*> m_parameterMap;
            std::map<unsigned int, const Description*> m_inputMap;
            std::map<unsigned int, const Description*> m_outputMap;
            
            // the indices of all inputs, output and parameters which are
            // have been during initialization, i.e. which must be removed
            // upon deinitialization
            std::set<unsigned int> m_activeInputs;
            std::set<unsigned int> m_activeOutputs;
            std::set<unsigned int> m_activeParameters;
        };
    }
}

#endif // STROMX_CORE_OPERATORKERNEL_H

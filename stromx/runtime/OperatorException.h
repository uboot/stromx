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

#ifndef STROMX_RUNTIME_OPERATOREXCEPTI0N_H
#define STROMX_RUNTIME_OPERATOREXCEPTI0N_H

#include "stromx/runtime/Exception.h"

#include "stromx/runtime/OperatorInfo.h"
#include "stromx/runtime/Parameter.h"

namespace stromx
{
    namespace runtime
    {
        class Parameter;
        
        /** \brief Error in connection with a specified operator. */
        class OperatorError : public Exception
        {
        public:
            /** Constructs an operator exception, i.e. an exception related to an operator. */
            OperatorError(const OperatorInfo& op, const std::string & message)
              : Exception(message),
                m_type(op.type()),
                m_package(op.package())
            {}
            
            /** Constructs an operator exception, i.e. an exception related to an operator. */
            OperatorError(const OperatorInfo& op, const std::string & message, const std::string & name)
              : Exception(message),
                m_name(name),
                m_type(op.type()),
                m_package(op.package())
            {}
            
            virtual ~OperatorError() throw() {}
            
            /** Returns the type of the operator this exception relates to. */
            const std::string & type() const { return m_type; }
            
            /** Returns the package of the operator this exception relates to. */
            const std::string & package() const { return m_package; }
            
            /** Returns the name of the operator this exception relates to. */
            const std::string & name() const { return m_name; }
            
            /** Sets the name of the operator this exception relates to. */
            void setName(const std::string & name) { m_name = name; }
            
        private:
            std::string m_name;
            const std::string m_type;
            const std::string m_package;
        };
        
        /** \brief The current state of the operator does not allow a specific operation. */
        class WrongOperatorState : public OperatorError
        {
        public:
            WrongOperatorState(const OperatorInfo& op, const std::string & message = "WrongOperatorState")
              : OperatorError(op, message)
            {}
        };
        
        /** \brief A parameter with this ID does not exist. */
        class WrongParameterId : public OperatorError
        {
        public:
            WrongParameterId(const unsigned int id, const OperatorInfo& op, const std::string & message = "WrongParameterId")
              : OperatorError(op, message),
                m_id(id)
            {}
            
        private:
            unsigned int m_id;
        };
        
        /** \brief General error in connection with a specified parameter. */
        class ParameterError : public OperatorError
        {
        public:
            ParameterError(const Parameter& param, const OperatorInfo& op, const std::string & message = "ParameterError")
              : OperatorError(op, message),
                m_id(param.id()),
                m_title(param.title()),
                m_variant(param.variant())
            {}
            
            virtual ~ParameterError() throw() {}
            
            /** Returns the ID of the parameter this exception relates to. */
            unsigned int id() const { return m_id; }
            
            /** Returns the title of the parameter this exception relates to. */
            const std::string & title() const { return m_title; }
            
            /** Returns the data variant of the parameter this exception relates to. */
            const VariantInterface & variant() const { return m_variant; }
            
        private:
            const unsigned int m_id;
            const std::string m_title;
            const VariantHandle & m_variant;
        };
        
        /** \brief Tried to set a parameter to data of the wrong type. */
        class WrongParameterType : public ParameterError
        {
        public:
            WrongParameterType(const Parameter& param, const OperatorInfo& op, const std::string & message = "WrongParameterType")
              : ParameterError(param, op, message)
            {}
        };
        
        /** \brief Tried to set a parameter to a wrong value. */
        class WrongParameterValue : public ParameterError
        {
        public:
            WrongParameterValue(const Parameter& param, const OperatorInfo& op, const std::string & message = "WrongParameterValue")
              : ParameterError(param, op, message)
            {}
        };
        
        /** \brief Tried to violate the parameter access mode. */
        class ParameterAccessViolation : public ParameterError
        {
        public:
            ParameterAccessViolation(const Parameter& param, const OperatorInfo& op, const std::string & message = "ParameterAccessViolation")
              : ParameterError(param, op, message)
            {}
        };
        
        
        /** \brief General error in connection with an operator output. */
        class OutputError : public OperatorError
        {
        public:
            OutputError(const unsigned int outputId, const OperatorInfo& op, const std::string & message = "InputError")
              : OperatorError(op, message),
                m_outputId(outputId)
            {}
            
            unsigned int outputId() const { return m_outputId; }
        private:
            const unsigned int m_outputId;
        };
        
        /** \brief General error in connection with an operator input. */
        class InputError : public OperatorError
        {
        public:
            InputError(const unsigned int inputId, const OperatorInfo& op, const std::string & message = "InputError")
              : OperatorError(op, message),
                m_inputId(inputId)
            {}
            
            unsigned int inputId() const { return m_inputId; }
        private:
            const unsigned int m_inputId;
        };
        
        /** \brief Tried to set an input to data of the wrong type. */
        class WrongInputType : public InputError
        {
        public:
            WrongInputType(const unsigned int inputId, const OperatorInfo& op, const std::string & message = "WrongInputType")
              : InputError(inputId, op, message)
            {}
        };
        
        /** \brief Tried to set an output to data of the wrong type. */
        class WrongOutputType : public OutputError
        {
        public:
            WrongOutputType(const unsigned int outputId, const OperatorInfo& op, const std::string & message = "WrongInputType")
              : OutputError(outputId, op, message)
            {}
        };
    }
}

#endif // STROMX_RUNTIME_OPERATOREXCEPTI0N_H

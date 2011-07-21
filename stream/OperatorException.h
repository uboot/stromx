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

#ifndef STREAM_OPERATOREXCEPTI0N_H
#define STREAM_OPERATOREXCEPTI0N_H

#include "Exception.h"

namespace stream
{
    class OperatorInfo;
    class Parameter;
    
    class OperatorException : public Exception
    {
    protected:
        OperatorException(const OperatorInfo& op, const std::string & message)
          : Exception(message),
            m_operator(op)
        {}
          
    private:
        const OperatorInfo& m_operator;
                       
    };
    
    class ParameterIdException : public OperatorException
    {
    public:
        ParameterIdException(const unsigned int id, const OperatorInfo& op, const std::string & message = "")
          : OperatorException(op, message),
            m_id(id)
        {}
        
    private:
        unsigned int m_id;
    };
    
    class ParameterException : public OperatorException
    {
    public:
        ParameterException(const Parameter& param, const OperatorInfo& op, const std::string & message = "")
          : OperatorException(op, message),
            m_parameter(param)
        {}
          
    private:
        const Parameter& m_parameter;
    };
    
    class ParameterTypeException : public ParameterException
    {
    public:
        ParameterTypeException(const Parameter& param, const OperatorInfo& op, const std::string & message = "")
          : ParameterException(param, op, message)
        {}
    };
    
    class ParameterValueException : public ParameterException
    {
    public:
        ParameterValueException(const Parameter& param, const OperatorInfo& op, const std::string & message = "")
          : ParameterException(param, op, message)
        {}
    };
    
    class ParameterAccessModeException : public ParameterException
    {
    public:
        ParameterAccessModeException(const Parameter& param, const OperatorInfo& op, const std::string & message = "")
          : ParameterException(param, op, message)
        {}
    };
    
    class InputException : public OperatorException
    {
    public:
        InputException(const unsigned int inputId, const OperatorInfo& op, const std::string & message = "")
          : OperatorException(op, message),
            m_inputId(inputId)
        {}
          
    private:
        const unsigned int m_inputId;;
    };
    
    class InputTypeException : public InputException
    {
    public:
        InputTypeException(const unsigned int inputId, const OperatorInfo& op, const std::string & message = "")
          : InputException(inputId, op, message)
        {}
    };
}

#endif // STREAM_OPERATOREXCEPTI0N_H

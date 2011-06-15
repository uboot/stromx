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

#ifndef STREAM_OPERATOR_H
#define STREAM_OPERATOR_H

#include "OperatorInfo.h"
#include "DataOwner.h"

namespace stream
{
    class Data;
    class DataContainer;
    class DataProvider;
    
    class Operator : public OperatorInfo, public DataOwner
    {
    public:
        Operator(const std::string & name,
                 const std::string & package,
                 const Version & version,
                 const std::vector<Description>& inputs,
                 const std::vector<Description>& outputs,
                 const std::vector<Parameter>& parameters);
        
        const std::string& name() const { return m_name; }
        const std::string& package() const { return m_package; }
        const Version& version() const { return m_version; }
        const std::vector<Description>& inputs() const { return m_inputs; }
        const std::vector<Description>& outputs() const { return m_outputs; }
        const std::vector<Parameter>& parameters() const { return m_parameters; }
        
        virtual void setParameter(unsigned int id, const Data& value) = 0;
        virtual const Data& getParameter(unsigned int id) = 0;
        
        virtual void execute(DataProvider& provider) = 0;
        virtual void activate() {}
        virtual void deactivate() {}
        
        virtual void release(DataContainer* const data) {};
        
    protected: 
        Description& getParameterDescription(unsigned int id);
        
    private:
        static void validate(const std::vector<Description>& descriptors);
        static void validate(const std::vector<Parameter>& descriptors);
        
        std::string m_name;
        std::string m_package;
        Version m_version;
        std::vector<Description> m_inputs;
        std::vector<Description> m_outputs;
        std::vector<Parameter> m_parameters;       
    };
}

#endif // STREAM_OPERATOR_H
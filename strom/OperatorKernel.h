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

#ifndef STROM_OPERATORKERNEL_H
#define STROM_OPERATORKERNEL_H

#include <map>
#include "OperatorInfo.h"

namespace strom
{
    class Data;
    class DataContainer;
    class DataProvider;
    
    namespace impl
    {
        class SynchronizedOperatorKernel;
    }
    
    /** \brief Abstract operator kernel */
    class OperatorKernel : public OperatorInfo
    {
        friend class impl::SynchronizedOperatorKernel;
        
    public:
        OperatorKernel(const std::string & type,
                 const std::string & package,
                 const Version & version,
                 const std::vector<const Parameter*>& parameters);
                 
        OperatorKernel(const std::string & type,
                 const std::string & package,
                 const Version & version,
                 const std::vector<const Description*>& inputs,
                 const std::vector<const Description*>& outputs,
                 const std::vector<const Parameter*>& parameters);
                 
        OperatorKernel(const std::string & type,
                 const std::string & package,
                 const Version & version);
                 
        virtual OperatorKernel* const clone() const = 0;
        virtual ~OperatorKernel();
        
        const std::string& type() const { return m_type; }
        const std::string& package() const { return m_package; }
        const Version& version() const { return m_version; }
        const std::vector<const Description*>& inputs() const { return m_inputs; }
        const std::vector<const Description*>& outputs() const { return m_outputs; }
        const std::vector<const Parameter*>& parameters() const { return m_parameters; }
        const Parameter & parameter(const unsigned int id) const;
        
        virtual void setParameter(const unsigned int id, const Data& value) {};
        virtual const Data& getParameter(const unsigned int id) const {};
        virtual void execute(DataProvider& provider) = 0;
        virtual void initialize() {}
        virtual void activate() {}
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

#endif // STROM_OPERATORKERNEL_H

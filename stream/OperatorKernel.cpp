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

#include "OperatorKernel.h"

#include "Exception.h"

#include <set>
#include <boost/lexical_cast.hpp>

namespace stream
{
    OperatorKernel::OperatorKernel (const std::string & name,
                        const std::string & package,
                        const Version & version,
                        const std::vector<const Parameter*>& parameters)
      : m_name(name),
        m_package(package),
        m_version(version),
        m_parameters(parameters)
    {
        validate(parameters);
    }
    
    OperatorKernel::OperatorKernel (const std::string & name,
                        const std::string & package,
                        const Version & version)
      : m_name(name),
        m_package(package),
        m_version(version)
    {
    }
    
    OperatorKernel::OperatorKernel(const std::string& name,
                       const std::string& package,
                       const stream::Version& version,
                       const std::vector<const Description* >& inputs,
                       const std::vector<const Description* >& outputs,
                       const std::vector<const Parameter* >& parameters)
      : m_name(name),
        m_package(package),
        m_version(version),
        m_inputs(inputs),
        m_outputs(outputs),
        m_parameters(parameters)
    {
        validate(inputs);
        validate(outputs);
        
        for(std::vector<const stream::Parameter*>::const_iterator iter = parameters.begin();
            iter != parameters.end();
            ++iter)
        {
            m_parameterMap[(*iter)->id()] = *iter;
        }
    }

    
    void OperatorKernel::initialize(const std::vector<const stream::Description*>& inputs,
                              const std::vector<const stream::Description*>& outputs,
                              const std::vector<const stream::Parameter*>& parameters)
    {
        for(std::vector<const stream::Description*>::const_iterator iter = inputs.begin();
            iter != inputs.end();
            ++iter)
        {
            m_inputs.push_back(*iter);
        }
        
        validate(inputs);
        
        for(std::vector<const stream::Description*>::const_iterator iter = outputs.begin();
            iter != outputs.end();
            ++iter)
        {
            m_outputs.push_back(*iter);
        }
        
        validate(outputs);
        
        for(std::vector<const stream::Parameter*>::const_iterator iter = parameters.begin();
            iter != parameters.end();
            ++iter)
        {
            m_parameters.push_back(*iter);
            m_parameterMap[(*iter)->id()] = *iter;
        }
        
        validate(m_parameters);
    }
    
    Parameter& OperatorKernel::parameter(const unsigned int id)
    {
        return const_cast<Parameter&>(parameter(id));
    }
    
    const Parameter& OperatorKernel::parameter(const unsigned int id) const
    {
        std::map<unsigned int, const Parameter*>::const_iterator iter = m_parameterMap.find(id);
        if(iter == m_parameterMap.end())
            throw WrongId("No parameter with ID " + id);
        
        return *iter->second;
    }
    
    OperatorKernel::~OperatorKernel()
    {
        deactivate();
        
        for(std::vector<const Description*>::const_iterator iter = m_inputs.begin();
            iter != m_inputs.end();
            ++iter)
        {
            delete *iter;
        }
        
        for(std::vector<const Description*>::const_iterator iter = m_outputs.begin();
            iter != m_outputs.end();
            ++iter)
        {
            delete *iter;
        }
        
        for(std::vector<const Parameter*>::const_iterator iter = m_parameters.begin();
            iter != m_parameters.end();
            ++iter)
        {
            delete *iter;
        }
    }

    void OperatorKernel::validate(const std::vector<const Description*>& descriptors)
    {
        std::set<unsigned int> ids;
        
        for(std::vector<const Description*>::const_iterator iter = descriptors.begin();
            iter != descriptors.end();
            ++iter)
        {
            if(ids.count((*iter)->id()))
                throw InvalidArgument("ID " + boost::lexical_cast<std::string>((*iter)->id()) + " appears twice.");
        }
    }
    
    void OperatorKernel::validate(const std::vector<const Parameter*>& descriptors)
    {
        std::set<unsigned int> ids;
        
        for(std::vector<const Parameter*>::const_iterator iter = descriptors.begin();
            iter != descriptors.end();
            ++iter)
        {
            if(ids.count((*iter)->id()))
                throw InvalidArgument("ID " + boost::lexical_cast<std::string>((*iter)->id()) + " appears twice.");
        }
    }
}

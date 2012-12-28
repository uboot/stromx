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

#include <boost/lexical_cast.hpp>
#include <set>
#include "Exception.h"
#include "OperatorKernel.h"

namespace stromx
{
    namespace core
    {
        OperatorKernel::OperatorKernel (const std::string & type,
                            const std::string & package,
                            const Version & version,
                            const std::vector<const Parameter*>& parameters)
          : m_type(type),
            m_package(package),
            m_version(version)
        {
            validateParameters(parameters);
            
            for(std::vector<const core::Parameter*>::const_iterator iter = parameters.begin();
                iter != parameters.end();
                ++iter)
            {
                m_parameters.push_back(*iter);
                m_parameterMap[(*iter)->id()] = *iter;
            }
        }
        
        OperatorKernel::OperatorKernel (const std::string & type,
                            const std::string & package,
                            const Version & version)
          : m_type(type),
            m_package(package),
            m_version(version)
        {
        }
        
        OperatorKernel::OperatorKernel(const std::string& type,
                        const std::string& package,
                        const core::Version& version,
                        const std::vector<const Description* >& inputs,
                        const std::vector<const Description* >& outputs,
                        const std::vector<const Parameter* >& parameters)
          : m_type(type),
            m_package(package),
            m_version(version)
        {
            validateInputs(inputs);
            validateOutputs(outputs);
            validateParameters(parameters);
            
            for(std::vector<const core::Description*>::const_iterator iter = inputs.begin();
                iter != inputs.end();
                ++iter)
            {
                m_inputs.push_back(*iter);
                m_inputMap[(*iter)->id()] = *iter;
            }
            
            
            for(std::vector<const core::Description*>::const_iterator iter = outputs.begin();
                iter != outputs.end();
                ++iter)
            {
                m_outputs.push_back(*iter);
                m_outputMap[(*iter)->id()] = *iter;
            }
            
            
            for(std::vector<const core::Parameter*>::const_iterator iter = parameters.begin();
                iter != parameters.end();
                ++iter)
            {
                m_parameters.push_back(*iter);
                m_parameterMap[(*iter)->id()] = *iter;
            }
        }

        
        void OperatorKernel::initialize(const std::vector<const core::Description*>& inputs,
                                        const std::vector<const core::Description*>& outputs,
                                        const std::vector<const core::Parameter*>& parameters)
        {
            validateInputs(inputs);
            validateOutputs(outputs);
            validateParameters(parameters);
            
            for(std::vector<const core::Description*>::const_iterator iter = inputs.begin();
                iter != inputs.end();
                ++iter)
            {
                m_inputs.push_back(*iter);
                m_inputMap[(*iter)->id()] = *iter;
                m_activeInputs.insert((*iter)->id());
            }
            
            
            for(std::vector<const core::Description*>::const_iterator iter = outputs.begin();
                iter != outputs.end();
                ++iter)
            {
                m_outputs.push_back(*iter);
                m_outputMap[(*iter)->id()] = *iter;
                m_activeOutputs.insert((*iter)->id());
            }
            
            
            for(std::vector<const core::Parameter*>::const_iterator iter = parameters.begin();
                iter != parameters.end();
                ++iter)
            {
                m_parameters.push_back(*iter);
                m_parameterMap[(*iter)->id()] = *iter;
                m_activeParameters.insert((*iter)->id());
            }
        }
        
        void OperatorKernel::deinitialize()
        {
            for(std::set<unsigned int>::iterator iter = m_activeInputs.begin();
                iter != m_activeInputs.end();
                ++iter)
            {
                m_inputs.pop_back();
                m_inputMap.erase(m_inputMap.find(*iter));
            }
            
            for(std::set<unsigned int>::iterator iter = m_activeOutputs.begin();
                iter != m_activeOutputs.end();
                ++iter)
            {
                m_outputs.pop_back();
                m_outputMap.erase(m_outputMap.find(*iter));
            }
            
            for(std::set<unsigned int>::iterator iter = m_activeParameters.begin();
                iter != m_activeParameters.end();
                ++iter)
            {
                m_parameters.pop_back();
                m_parameterMap.erase(m_parameterMap.find(*iter));
            }
            
            m_activeInputs.clear();
            m_activeOutputs.clear();
            m_activeParameters.clear();
        }
        
        const Parameter & OperatorKernel::findParameter(const unsigned int id) const
        {
            std::map<unsigned int, const Parameter*>::const_iterator iter = m_parameterMap.find(id);
            if(iter == m_parameterMap.end())
                throw WrongId("No parameter with ID " + id);
            
            return *iter->second;
        }
        
        Parameter& OperatorKernel::parameter(const unsigned int id)
        {
            return const_cast<Parameter &>(findParameter(id));
        }
        
        const Parameter& OperatorKernel::parameter(const unsigned int id) const
        {
            return findParameter(id);
        }
        
        const Description& OperatorKernel::input(const unsigned int id) const
        {
            std::map<unsigned int, const Description*>::const_iterator iter = m_inputMap.find(id);
            if(iter == m_inputMap.end())
                throw WrongId("No input with ID " + id);
            
            return *iter->second;
        }

        const Description& OperatorKernel::output(const unsigned int id) const
        {
            std::map<unsigned int, const Description*>::const_iterator iter = m_outputMap.find(id);
            if(iter == m_outputMap.end())
                throw WrongId("No output with ID " + id);
            
            return *iter->second;
        }
        
        OperatorKernel::~OperatorKernel()
        {
            deactivate();
            deinitialize();
            
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

        void OperatorKernel::validateOutputs(const std::vector<const Description*>& descriptors)
        {
            std::set<unsigned int> existingIds;
            std::set<unsigned int> newIds;
            
            // collect all existing IDs
            for(std::vector<const Description*>::const_iterator iter = m_outputs.begin();
                iter != m_outputs.end();
                ++iter)
            {
                existingIds.insert((*iter)->id());
            }
            
            // collect all new IDs
            for(std::vector<const Description*>::const_iterator iter = descriptors.begin();
                iter != descriptors.end();
                ++iter)
            {
                if(newIds.count((*iter)->id()))
                    throw WrongArgument("ID " + boost::lexical_cast<std::string>((*iter)->id()) + " appears twice.");
            
                newIds.insert((*iter)->id());
            }
            
            // check if any of the new IDs matches an existing ID
            for(std::set<unsigned int>::const_iterator iter = newIds.begin();
                iter != newIds.end();
                ++iter)
            {
                if(existingIds.count(*iter))
                    throw WrongArgument("Parameter with ID " + boost::lexical_cast<std::string>(*iter) + " has already been added.");
            }
        }

        void OperatorKernel::validateInputs(const std::vector<const Description*>& descriptors)
        {
            std::set<unsigned int> existingIds;
            std::set<unsigned int> newIds;
            
            // collect all existing IDs
            for(std::vector<const Description*>::const_iterator iter = m_inputs.begin();
                iter != m_inputs.end();
                ++iter)
            {
                existingIds.insert((*iter)->id());
            }
            
            // collect all new IDs
            for(std::vector<const Description*>::const_iterator iter = descriptors.begin();
                iter != descriptors.end();
                ++iter)
            {
                if(newIds.count((*iter)->id()))
                    throw WrongArgument("ID " + boost::lexical_cast<std::string>((*iter)->id()) + " appears twice.");
            
                newIds.insert((*iter)->id());
            }
            
            // check if any of the new IDs matches an existing ID
            for(std::set<unsigned int>::const_iterator iter = newIds.begin();
                iter != newIds.end();
                ++iter)
            {
                if(existingIds.count(*iter))
                    throw WrongArgument("Parameter with ID " + boost::lexical_cast<std::string>(*iter) + " has already been added.");
            }
        }
        
        void OperatorKernel::validateParameters(const std::vector<const Parameter*>& descriptors)
        {
            std::set<unsigned int> existingIds;
            std::set<unsigned int> newIds;
            std::set<const Parameter*> params;
            
            // collect all existing IDs
            for(std::vector<const Parameter*>::const_iterator iter = m_parameters.begin();
                iter != m_parameters.end();
                ++iter)
            {
                existingIds.insert((*iter)->id());
                params.insert(*iter);
            }
            
            // collect all new IDs
            for(std::vector<const Parameter*>::const_iterator iter = descriptors.begin();
                iter != descriptors.end();
                ++iter)
            {
                if(newIds.count((*iter)->id()))
                    throw WrongArgument("ID " + boost::lexical_cast<std::string>((*iter)->id()) + " appears twice.");
            
                newIds.insert((*iter)->id());
                params.insert(*iter);
            }
            
            // check if any of the new IDs matches an existing ID
            for(std::set<unsigned int>::const_iterator iter = newIds.begin();
                iter != newIds.end();
                ++iter)
            {
                if(existingIds.count(*iter))
                    throw WrongArgument("Parameter with ID " + boost::lexical_cast<std::string>(*iter) + " has already been added.");
            }
            
            for(std::vector<const Parameter*>::const_iterator iter = descriptors.begin();
                iter != descriptors.end();
                ++iter)
            {
                if((*iter)->group())
                {
                    if(! params.count((*iter)->group()))
                        throw WrongArgument("Parameter with ID "
                            + boost::lexical_cast<std::string>(*iter)
                            + " references a group which has not been added to the operator.");
                }
            }
        }
    }
}

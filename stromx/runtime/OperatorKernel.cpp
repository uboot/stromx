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
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/OperatorKernel.h"
#include "stromx/runtime/impl/ConnectorParameter.h"

namespace stromx
{
    namespace runtime
    {
        OperatorKernel::OperatorKernel (const std::string & type,
                            const std::string & package,
                            const Version & version,
                            const std::vector<const Parameter*>& parameters,
                            const OperatorProperties & properties)
          : m_type(type),
            m_package(package),
            m_version(version),
            m_properties(properties)
        {
            validateDescriptions(std::vector<const Input*>(), 
                                 std::vector<const Output*>(),
                                 parameters);
            
            for(std::vector<const runtime::Parameter*>::const_iterator iter = parameters.begin();
                iter != parameters.end();
                ++iter)
            {
                m_parameters.push_back(*iter);
            }
            
            updateVisibleDescriptions(false);
        }
        
        OperatorKernel::OperatorKernel (const std::string & type,
                            const std::string & package,
                            const Version & version,
                            const OperatorProperties & properties)
          : m_type(type),
            m_package(package),
            m_version(version),
            m_properties(properties)
        {
        }
        
        OperatorKernel::OperatorKernel(const std::string& type,
                        const std::string& package,
                        const runtime::Version& version,
                        const std::vector<const Input* >& inputs,
                        const std::vector<const Output* >& outputs,
                        const std::vector<const Parameter* >& parameters,
                        const OperatorProperties & properties)
          : m_type(type),
            m_package(package),
            m_version(version),
            m_properties(properties)
        {
            validateDescriptions(inputs, outputs, parameters);
            
            for(std::vector<const runtime::Input*>::const_iterator iter = inputs.begin();
                iter != inputs.end();
                ++iter)
            {
                m_inputs.push_back(*iter);
            }
            
            
            for(std::vector<const runtime::Output*>::const_iterator iter = outputs.begin();
                iter != outputs.end();
                ++iter)
            {
                m_outputs.push_back(*iter);
            }
            
            
            for(std::vector<const runtime::Parameter*>::const_iterator iter = parameters.begin();
                iter != parameters.end();
                ++iter)
            {
                m_parameters.push_back(*iter);
            }
            
            updateVisibleDescriptions(false);
        }

        OperatorKernel::OperatorKernel(const std::string& type,
                        const std::string& package,
                        const runtime::Version& version,
                        const std::vector<const Input* >& inputs,
                        const std::vector<const Output* >& outputs,
                        const OperatorProperties & properties)
          : m_type(type),
            m_package(package),
            m_version(version),
            m_properties(properties)
        {
            validateDescriptions(inputs, outputs,
                                 std::vector<const Parameter* >());
            
            for(std::vector<const runtime::Input*>::const_iterator iter = inputs.begin();
                iter != inputs.end();
                ++iter)
            {
                m_inputs.push_back(*iter);
            }
            
            
            for(std::vector<const runtime::Output*>::const_iterator iter = outputs.begin();
                iter != outputs.end();
                ++iter)
            {
                m_outputs.push_back(*iter);
            }
            
            updateVisibleDescriptions(false);
        }
        
        void OperatorKernel::initialize()
        {
            setupDefaultTypeMap();
            updateVisibleDescriptions(true);
        }
        
        void OperatorKernel::initialize(const std::vector<const runtime::Input*>& inputs,
                                        const std::vector<const runtime::Output*>& outputs,
                                        const std::vector<const runtime::Parameter*>& parameters)
        {
            validateDescriptions(inputs, outputs, parameters);
            
            for(std::vector<const runtime::Input*>::const_iterator iter = inputs.begin();
                iter != inputs.end();
                ++iter)
            {
                m_inputs.push_back(*iter);
                m_activeInputs.insert((*iter)->id());
            }
            
            
            for(std::vector<const runtime::Output*>::const_iterator iter = outputs.begin();
                iter != outputs.end();
                ++iter)
            {
                m_outputs.push_back(*iter);
                m_activeOutputs.insert((*iter)->id());
            }
            
            
            for(std::vector<const runtime::Parameter*>::const_iterator iter = parameters.begin();
                iter != parameters.end();
                ++iter)
            {
                m_parameters.push_back(*iter);
                m_activeParameters.insert((*iter)->id());
            }
            
            setupDefaultTypeMap();
            updateVisibleDescriptions(true);
        }
        
        void OperatorKernel::deinitialize()
        {
            for(std::set<unsigned int>::iterator iter = m_activeInputs.begin();
                iter != m_activeInputs.end();
                ++iter)
            {
                m_inputs.pop_back();
            }
            
            for(std::set<unsigned int>::iterator iter = m_activeOutputs.begin();
                iter != m_activeOutputs.end();
                ++iter)
            {
                m_outputs.pop_back();
            }
            
            for(std::set<unsigned int>::iterator iter = m_activeParameters.begin();
                iter != m_activeParameters.end();
                ++iter)
            {
                m_parameters.pop_back();
            }
            
            m_activeInputs.clear();
            m_activeOutputs.clear();
            m_activeParameters.clear();
            
            updateVisibleDescriptions(false);
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
        
        const Input& OperatorKernel::input(const unsigned int id) const
        {
            std::map<unsigned int, const Input*>::const_iterator iter = m_inputMap.find(id);
            if(iter == m_inputMap.end())
                throw WrongId("No input with ID " + id);
            
            return *iter->second;
        }

        const Output& OperatorKernel::output(const unsigned int id) const
        {
            std::map<unsigned int, const Output*>::const_iterator iter = m_outputMap.find(id);
            if(iter == m_outputMap.end())
                throw WrongId("No output with ID " + id);
            
            return *iter->second;
        }
        
        const OperatorProperties& OperatorKernel::properties() const
        {
            return m_properties;
        }
        
        OperatorKernel::~OperatorKernel()
        {
            deactivate();
            deinitialize();
            
            for(std::vector<const Input*>::const_iterator iter = m_inputs.begin();
                iter != m_inputs.end();
                ++iter)
            {
                delete *iter;
            }
            
            for(std::vector<const Output*>::const_iterator iter = m_outputs.begin();
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
        
        void OperatorKernel::validateDescriptions(const std::vector<const Input*>& inputs,
                                                  const std::vector<const Output*>& outputs,
                                                  const std::vector<const Parameter*>& parameters)
        {
            std::set<unsigned int> existingIds;
            std::set<unsigned int> newIds;
            std::set<const Parameter*> allParameters;
            
            // collect all existing IDs
            for(std::vector<const Input*>::const_iterator iter = m_inputs.begin();
                iter != m_inputs.end();
                ++iter)
            {
                existingIds.insert((*iter)->id());
            }
            
            for(std::vector<const Output*>::const_iterator iter = m_outputs.begin();
                iter != m_outputs.end();
                ++iter)
            {
                existingIds.insert((*iter)->id());
            }
            
            for(std::vector<const Parameter*>::const_iterator iter = m_parameters.begin();
                iter != m_parameters.end();
                ++iter)
            {
                existingIds.insert((*iter)->id());
                allParameters.insert(*iter);
            }
            
            // collect all new IDs
            for(std::vector<const Input*>::const_iterator iter = inputs.begin();
                iter != inputs.end();
                ++iter)
            {
                if(newIds.count((*iter)->id()))
                    throw WrongArgument("ID " + boost::lexical_cast<std::string>((*iter)->id()) + " appears twice.");
                    
                newIds.insert((*iter)->id());
            }
            
            for(std::vector<const Output*>::const_iterator iter = outputs.begin();
                iter != outputs.end();
                ++iter)
            {
                if(newIds.count((*iter)->id()))
                    throw WrongArgument("ID " + boost::lexical_cast<std::string>((*iter)->id()) + " appears twice.");
                    
                newIds.insert((*iter)->id());
            }
            
            for(std::vector<const Parameter*>::const_iterator iter = parameters.begin();
                iter != parameters.end();
                ++iter)
            {
                if(newIds.count((*iter)->id()))
                    throw WrongArgument("ID " + boost::lexical_cast<std::string>((*iter)->id()) + " appears twice.");
                    
                newIds.insert((*iter)->id());
                allParameters.insert(*iter);
            }
            
            // check if any of the new IDs matches an existing ID
            for(std::set<unsigned int>::const_iterator iter = newIds.begin();
                iter != newIds.end();
                ++iter)
            {
                if(existingIds.count(*iter))
                    throw WrongArgument("Descriptor with ID " + boost::lexical_cast<std::string>(*iter) + " has already been added.");
            }
            
            // check if all groups of the new parameters are part of this operator
            for(std::vector<const Parameter*>::const_iterator iter = parameters.begin();
                iter != parameters.end();
                ++iter)
            {
                if((*iter)->group())
                {
                    if(! allParameters.count((*iter)->group()))
                        throw WrongArgument("Parameter with ID "
                            + boost::lexical_cast<std::string>(*iter)
                            + " references a group which has not been added to the operator.");
                }
            }
        }
        
        void OperatorKernel::setupDefaultTypeMap()
        {           
            m_typeMap.clear();
            m_behaviorMap.clear();
            
            for(std::vector<const Input*>::const_iterator iter = m_inputs.begin();
                iter != m_inputs.end();
                ++iter)
            {
                m_typeMap[(*iter)->id()] = (*iter)->defaultType();
                m_behaviorMap[(*iter)->id()] = (*iter)->defaultBehavior();
            }
            
            for(std::vector<const Output*>::const_iterator iter = m_outputs.begin();
                iter != m_outputs.end();
                ++iter)
            {
                m_typeMap[(*iter)->id()] = (*iter)->defaultType();
                m_behaviorMap[(*iter)->id()] = (*iter)->defaultBehavior();
            }
            
            for(std::vector<const Parameter*>::const_iterator iter = m_parameters.begin();
                iter != m_parameters.end();
                ++iter)
            {
                m_typeMap[(*iter)->id()] = DescriptionBase::PARAMETER;
                m_behaviorMap[(*iter)->id()] = (*iter)->updateBehavior();
            }
        }
            
        void OperatorKernel::updateVisibleDescriptions(const bool isInitialized)
        {
            // delete all proxy parameter (i.e. of type ConnectorParameters)
            for(std::vector<const Parameter*>::const_iterator iter = m_visibleParameters.begin();
                iter != m_visibleParameters.end();
                ++iter)
            {
                if ((*iter)->originalType() != DescriptionBase::PARAMETER)
                    delete *iter;
            }
        
            m_visibleInputs.clear();
            m_visibleOutputs.clear();
            m_visibleParameters.clear();
            
            m_inputMap.clear();
            m_outputMap.clear();
            m_parameterMap.clear();
            
            for(std::vector<const Input*>::const_iterator iter = m_inputs.begin();
                iter != m_inputs.end();
                ++iter)
            {
                if (isInitialized)
                {
                    if (m_typeMap[(*iter)->id()] == DescriptionBase::PARAMETER)
                    {
                        Parameter* param = new impl::ConnectorParameter(*iter, m_behaviorMap[(*iter)->id()]);
                        m_visibleParameters.push_back(param);
                        m_parameterMap[(*iter)->id()] = param;
                    }
                    else
                    {
                        m_visibleInputs.push_back(*iter);
                        m_inputMap[(*iter)->id()] = *iter;
                    }
                }
            }
            
            for(std::vector<const Output*>::const_iterator iter = m_outputs.begin();
                iter != m_outputs.end();
                ++iter)
            {
                if (isInitialized)
                {
                    if (m_typeMap[(*iter)->id()] == DescriptionBase::PARAMETER)
                    {
                        Parameter* param = new impl::ConnectorParameter(*iter, m_behaviorMap[(*iter)->id()]);
                        m_visibleParameters.push_back(param);
                        m_parameterMap[(*iter)->id()] = param;
                    }
                    else
                    {
                        m_visibleOutputs.push_back(*iter);
                        m_outputMap[(*iter)->id()] = *iter;
                    }
                }
            }
            
            for(std::vector<const Parameter*>::const_iterator iter = m_parameters.begin();
                iter != m_parameters.end();
                ++iter)
            {
                const bool isInitParameter = (*iter)->accessMode() == Parameter::NONE_READ || (*iter)->accessMode() == Parameter::NONE_WRITE;
                if (isInitialized || isInitParameter)
                {
                    m_visibleParameters.push_back(*iter);
                    m_parameterMap[(*iter)->id()] = *iter;
                }
            }
        }
        
        const DescriptionBase* OperatorKernel::findDescription(const unsigned int id) const
        {
            std::map<unsigned int, const Input*>::const_iterator inputIter = m_inputMap.find(id);
            if (inputIter != m_inputMap.end())
                return inputIter->second;
                
            std::map<unsigned int, const Output*>::const_iterator outputIter = m_outputMap.find(id);
            if (outputIter != m_outputMap.end())
                return outputIter->second;
                
            std::map<unsigned int, const Parameter*>::const_iterator parameterIter = m_parameterMap.find(id);
            if (parameterIter != m_parameterMap.end())
                return parameterIter->second;
                
            throw WrongArgument("No description for ID exists for this operator.");
        }
        
        void OperatorKernel::setConnectorType(const unsigned int id, const DescriptionBase::Type type,
            const Parameter::UpdateBehavior updateBehavior)
        {
            const DescriptionBase* description = findDescription(id);
            
            switch (type)
            {
            case DescriptionBase::INPUT:
                if (description->originalType() != DescriptionBase::INPUT)
                    throw WrongArgument("Descriptions can only be turned into inputs if their original type is INPUT.");
                break;
            case DescriptionBase::OUTPUT:
                if (description->originalType() != DescriptionBase::OUTPUT)
                    throw WrongArgument("Descriptions can only be turned into outputs if their original type is OUTPUT.");
                break;
            case DescriptionBase::PARAMETER:
                if (description->originalType() == DescriptionBase::INPUT &&
                    updateBehavior == DescriptionBase::PULL)
                {
                    throw WrongArgument("Inputs can not be turned into pull parameters.");
                }
                if (description->originalType() == DescriptionBase::OUTPUT &&
                    updateBehavior == DescriptionBase::PUSH)
                {
                    throw WrongArgument("Outputs can not be turned into push parameters.");
                }
                break;
            default:
                break;
            }
            
            m_typeMap[id] = type;
            m_behaviorMap[id] = updateBehavior;
            
            updateVisibleDescriptions(true);
        }
    }
}

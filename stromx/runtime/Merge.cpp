/* 
 *  Copyright 2015 Matthias Fuchs
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

#include "stromx/runtime/Merge.h"

#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/List.h"
#include "stromx/runtime/Locale.h"
#include "stromx/runtime/NumericParameter.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/RecycleAccess.h"

namespace stromx
{
    namespace runtime
    {
        const std::string Merge::TYPE("Merge");
        const std::string Merge::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Merge::VERSION(0, 1, 0);
        
        Merge::Merge()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs()),
            m_numItems(0),
            m_list(0)
        {
        }
        
        void Merge::setParameter(unsigned int id, const runtime::Data& /*value*/)
        {
            throw WrongParameterId(id, *this);
        }

        const DataRef Merge::getParameter(const unsigned int id) const
        {
            throw WrongParameterId(id, *this);
        }
        
        void Merge::activate()
        {
            m_numItems = 0;
            BOOST_ASSERT(m_list == 0);
        }
        
        void Merge::deactivate()
        {
            delete m_list;
            m_list = 0;
        }
        
        void Merge::execute(DataProvider& provider)
        {
            if (m_list == 0)
            {
                Id2DataPair dataMapper(INPUT_NUM_ITEMS);
                provider.receiveInputData(dataMapper);
                
                try
                {
                    m_numItems = ReadAccess<UInt64>(dataMapper.data()).get();
                }
                catch (BadCast&)
                {
                    throw InputError(INPUT_NUM_ITEMS, *this, "Number of items must be a 'UInt64' object.");
                }
                
                m_list = new List();
            }
            
            if (m_list->content().size() < m_numItems)
            {
                RecycleAccess recycler;
                {
                    Id2DataPair dataMapper(INPUT_DATA);
                    provider.receiveInputData(dataMapper);
                    recycler.add(dataMapper.data());
                }
                
                Data* data = recycler.get();
                m_list->content().push_back(data);
            }
            
            if (m_list->content().size() == m_numItems)
            {
                DataContainer out(m_list);
                m_list = 0;
                m_numItems = 0;
                Id2DataPair dataMapper(OUTPUT, out);
                
                provider.sendOutputData(dataMapper);
            }
        }
        
        const std::vector<const Description*> Merge::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* data = new Description(INPUT_DATA, Variant::DATA);
            data->setTitle(L_("List items"));
            inputs.push_back(data);
            
            Description* input = new Description(INPUT_NUM_ITEMS, Variant::UINT_64);
            input->setTitle(L_("Number of list items"));
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Description*> Merge::setupOutputs() const
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, Variant::LIST);
            output->setTitle(L_("List"));
            outputs.push_back(output);
            
            return outputs;
        }
    }
}

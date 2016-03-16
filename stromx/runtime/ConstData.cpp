/* 
*  Copyright 2014 Matthias Fuchs
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

#include "stromx/runtime/ConstData.h"

#include <boost/assert.hpp>

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/EnumParameter.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/String.h"
#include "stromx/runtime/TriggerData.h"
#include "stromx/runtime/Variant.h"


namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        namespace
        {
            Data* typeToData(const unsigned int type)
            {
                if (type == Variant::BOOL.id())
                    return new Bool;
                
                if (type == Variant::TRIGGER.id())
                    return new TriggerData;
                
                if (type == Variant::INT_32.id())
                    return new Int32;
                
                if (type == Variant::UINT_32.id())
                    return new UInt32;
                
                if (type == Variant::STRING.id())
                    return new String;
                
                if (type == Variant::FLOAT_32.id())
                    return new Float32;
                    
                throw InternalError("Unhandled data type");
            }
        }
        
        const std::string ConstData::TYPE("ConstData");
        const std::string ConstData::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version ConstData::VERSION(0, 1, 0);
        
        ConstData::ConstData()
          : DataOperatorBase(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_allocateData(true)
        {
        }
        
        ConstData::~ConstData()
        {
            BOOST_ASSERT(m_recycleAccess.empty());
        }
        
        void ConstData::deactivate()
        {
            Data* dataPtr = 0;
            
            // delete all remaining buffers in the recycling access
            while((dataPtr = m_recycleAccess()))
                delete dataPtr;
        }

        void ConstData::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case ALLOCATE_DATA:
                    m_allocateData = data_cast<Bool>(value);
                    break;
                case DATA_TYPE:
                    DataOperatorBase::setParameter(id, value);
                    
                    // make sure some value is set 
                    if (! valuePtr())
                        resetValuePtr(typeToData(data_cast<Enum>(value)));
                    break;
                default:
                    DataOperatorBase::setParameter(id, value);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const DataRef ConstData::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case ALLOCATE_DATA:
                return m_allocateData;
            default:
                return DataOperatorBase::getParameter(id);
            }
        }  
        
        void ConstData::execute(DataProvider& provider)
        {
            if (! valuePtr())
                throw InternalError("Value has not been set");
            
            DataContainer data;
            if (m_allocateData)
            {
                data = DataContainer(valuePtr()->clone());
            }
            else
            {
                Data* dataPtr = 0;
                
                if (m_recycleAccess.empty())
                {
                    // if this is the first time the operator executes the value must
                    // be cloned
                    dataPtr = valuePtr()->clone();
                }
                else 
                {         
                    // otherwise the value in the recycler can be reused              
                    dataPtr = m_recycleAccess.get();
                }
                    
                if (dataPtr != valuePtr())
                {
                    // if the value was changed by the user since the last execution
                    // update it
                    delete dataPtr;
                    dataPtr = valuePtr()->clone();
                }
                
                // send and remember for the next execution
                data = DataContainer(dataPtr);
                m_recycleAccess.add(data);
            }
            
            Id2DataPair outputDataMapper(OUTPUT, data);
            provider.sendOutputData( outputDataMapper);
        }
        
        const std::vector<const Parameter*> ConstData::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            parameters = DataOperatorBase::setupInitParameters();
            
            Parameter* allocate = new Parameter(ALLOCATE_DATA, Variant::BOOL);
            allocate->setTitle("Allocate data");
            allocate->setAccessMode(runtime::Parameter::NONE_WRITE);
            parameters.push_back(allocate);
                                        
            return parameters;
        }      
    } 
}

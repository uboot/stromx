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

#include "stromx/runtime/Push.h"

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/EnumParameter.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Locale.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/TriggerData.h"

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        typedef boost::lock_guard<boost::mutex> lock_t;
        typedef boost::unique_lock<boost::mutex> unique_lock_t;
        
        /** \cond */
        namespace impl
        {
            struct BoostConditionVariable
            {
                boost::condition_variable_any m_cond;
                boost::mutex m_mutex;
            };
        }
        /** \endcond */
        
        const std::string Push::TYPE("Push");
        
        const std::string Push::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Push::VERSION(0, 1, 0);
        
        Push::Push()
          : DataOperatorBase(TYPE, PACKAGE, VERSION, DataOperatorBase::setupInitParameters()),
            m_cond(new impl::BoostConditionVariable)
        {
            delete resetValuePtr();
        }
        
        Push::~Push()
        {
            delete m_cond;
        }

        void Push::setParameter(unsigned int id, const Data& value)
        {            
            if (id == DataOperatorBase::VALUE)
            {
                lock_t l(m_cond->m_mutex);
                DataOperatorBase::setParameter(id, value);
                m_cond->m_cond.notify_all();
            }
            else
            {
                DataOperatorBase::setParameter(id, value);
            }
        }
        
        void Push::activate()
        {
            lock_t l(m_cond->m_mutex);
        }
        
        void Push::execute(DataProvider& provider)
        {
            DataContainer output;
            
            {
                unique_lock_t lock(m_cond->m_mutex);
                
                // allow to set parameters while waiting
                provider.unlockParameters();
                
                try 
                {
                    // wait for parameter value
                    while (! valuePtr())
                        m_cond->m_cond.wait(lock);
                }
                catch(boost::thread_interrupted&)
                {
                    throw Interrupt();
                }
                
                // get the value
                output = DataContainer(resetValuePtr());
                
                // lock the parameters again
                provider.lockParameters();
            }
            
            Id2DataPair outputDataMapper(OUTPUT, output);
            provider.sendOutputData(outputDataMapper);
        }       
        
        void Push::initialize()
        {
            DataOperatorBase::initialize();
            parameter(VALUE).setUpdateBehavior(Parameter::PUSH);
        }
    } 
}

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

#include "stromx/runtime/Counter.h"
#include "stromx/runtime/Dump.h"
#include "stromx/runtime/Enum.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/Fork.h"
#include "stromx/runtime/Join.h"
#include "stromx/runtime/None.h"
#include "stromx/runtime/PeriodicDelay.h"
#include "stromx/runtime/Primitive.h"
#include "stromx/runtime/Queue.h"
#include "stromx/runtime/Runtime.h"
#include "stromx/runtime/Registry.h"
#include "stromx/runtime/Trigger.h"
#include "stromx/runtime/TriggerData.h"

void stromxRuntimeRegister(stromx::runtime::Registry& registry)
{
    using namespace stromx::runtime;
    
    registry.registerOperator(new Counter);
    registry.registerOperator(new Dump);
    registry.registerOperator(new Fork);
    registry.registerOperator(new Join);
    registry.registerOperator(new PeriodicDelay);
    registry.registerOperator(new Queue);
    registry.registerOperator(new Trigger);
    
    registry.registerData(new Bool);
    registry.registerData(new Int8);
    registry.registerData(new UInt8);
    registry.registerData(new Int16);
    registry.registerData(new UInt16);
    registry.registerData(new Int32);
    registry.registerData(new UInt32);
    registry.registerData(new Float64);
    registry.registerData(new Enum);
    registry.registerData(new None);
    registry.registerData(new TriggerData);
}

namespace stromx
{
    namespace runtime
    {
        Version version()
        {
            return Version(STROMX_RUNTIME_VERSION_MAJOR,
                           STROMX_RUNTIME_VERSION_MINOR,
                           STROMX_RUNTIME_VERSION_PATCH);
        }
    }
}
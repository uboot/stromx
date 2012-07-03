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

#include "Core.h"
#include "Dump.h"
#include "Enum.h"
#include "Exception.h"
#include "Fork.h"
#include "None.h"
#include "Primitive.h"
#include "Queue.h"
#include "Registry.h"
#include "TriggerData.h"

void stromxRegisterCore(stromx::core::Registry& registry)
{
    using namespace stromx::core;
    
    registry.registerOperator(new Dump);
    registry.registerOperator(new Fork);
    registry.registerOperator(new Queue);
    
    registry.registerData(new Bool);
    registry.registerData(new Int8);
    registry.registerData(new UInt8);
    registry.registerData(new Int16);
    registry.registerData(new UInt16);
    registry.registerData(new Int32);
    registry.registerData(new UInt32);
    registry.registerData(new Double);
    registry.registerData(new Enum);
    registry.registerData(new None);
    registry.registerData(new TriggerData);
}

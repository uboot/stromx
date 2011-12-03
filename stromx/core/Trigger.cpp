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

#include "Config.h"
#include "Trigger.h"

namespace stromx
{
    namespace core
    {
        const std::string Trigger::TYPE = "Trigger";
        const std::string Trigger::PACKAGE = STROMX_CORE_PACKAGE_NAME;
        const Version Trigger::VERSION = Version(STROMX_VERSION_MAJOR, STROMX_VERSION_MINOR, STROMX_VERSION_PATCH);
    }
}
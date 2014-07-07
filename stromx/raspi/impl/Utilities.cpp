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

#include "stromx/raspi/impl/Utilities.h"

#include <stromx/runtime/EnumParameter.h>

namespace stromx
{
    namespace raspi
    {
        namespace impl
        {
            runtime::EnumParameter* createGpioParameter(const int id)
            {
                using namespace stromx::runtime;
                
                EnumParameter* gpio = new EnumParameter(id);
                gpio->setTitle("GPIO");
                gpio->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
                gpio->add(EnumDescription(Enum(0), "0"));
                gpio->add(EnumDescription(Enum(1), "1"));
                gpio->add(EnumDescription(Enum(2), "2"));
                gpio->add(EnumDescription(Enum(3), "3"));
                gpio->add(EnumDescription(Enum(4), "4"));
                gpio->add(EnumDescription(Enum(7), "7"));
                gpio->add(EnumDescription(Enum(8), "8"));
                gpio->add(EnumDescription(Enum(9), "9"));
                gpio->add(EnumDescription(Enum(10), "10"));
                gpio->add(EnumDescription(Enum(14), "14"));
                gpio->add(EnumDescription(Enum(15), "15"));
                gpio->add(EnumDescription(Enum(17), "17"));
                gpio->add(EnumDescription(Enum(18), "18"));
                gpio->add(EnumDescription(Enum(21), "21"));
                gpio->add(EnumDescription(Enum(22), "22"));
                gpio->add(EnumDescription(Enum(23), "23"));
                gpio->add(EnumDescription(Enum(24), "24"));
                gpio->add(EnumDescription(Enum(25), "25"));
                gpio->add(EnumDescription(Enum(27), "27"));
                
                return gpio;
            }
        }
    }
}
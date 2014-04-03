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

#ifndef STROMX_RASPI_IMPL_GPIO_H
#define STROMX_RASPI_IMPL_GPIO_H

namespace stromx
{
    namespace raspi
    {
        namespace impl
        {
            /*
            * TODO
            * 
            * This code is taken from http://elinux.org/RPi_Low-level_peripherals#C_.2B_sysfs
            */
            
            const static int IN = 0;
            const static int OUT = 1;
            
            int GPIOExport(int pin);
            int GPIOUnexport(int pin);
            int GPIODirection(int pin, int dir);
            int GPIORead(int pin);
            int GPIOWrite(int pin, int value);
        }
    }
}

#endif // STROMX_RASPI_IMPL_GPIO_H
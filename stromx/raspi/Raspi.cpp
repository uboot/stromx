/* 
*  Copyright 2014 Thomas Fidler
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

#include "stromx/raspi/GpioTrigger.h"
#include "stromx/raspi/Raspi.h"
#include "stromx/raspi/RaspiStillCam.h"
#include "stromx/raspi/ReadGpio.h"
#include "stromx/raspi/WriteGpio.h"
#include <stromx/runtime/Registry.h>
#include <stromx/runtime/Locale.h>

namespace stromx
{
    namespace raspi
    {
        std::locale locale;
    }
}

int stromxRegisterRaspi(stromx::runtime::Registry* registry)
{
    using namespace stromx::raspi;
    
    locale = stromx::runtime::Locale::generate(STROMX_RASPI_LOCALE_DIR,
                                               STROMX_RASPI_LOCALE_DOMAIN);
    
    try
    {
        registry->registerOperator(new GpioTrigger);
        registry->registerOperator(new RaspiStillCam);
        registry->registerOperator(new ReadGpio);
        registry->registerOperator(new WriteGpio);
    }
    catch(stromx::runtime::Exception & e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

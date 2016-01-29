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

#include "stromx/cvml/Cvml.h"

#include <stromx/runtime/Locale.h>
#include <stromx/runtime/Registry.h>
#include "stromx/cvml/Svm.h"

namespace stromx
{
    namespace cvml
    {
        std::locale locale;
    }
}

int stromxRegisterCvml(stromx::runtime::Registry* registry)
{
    using namespace stromx::cvml;
    
    locale = stromx::runtime::Locale::generate(STROMX_CVML_LOCALE_DIR,
                                               STROMX_CVML_LOCALE_DOMAIN);
    
    try
    {
        registry->registerOperator(new Svm);
    }
    catch(stromx::runtime::Exception & e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
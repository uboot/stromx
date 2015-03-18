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

#include "stromx/cvsupport/AdjustRgbChannels.h"
#include "stromx/cvsupport/Buffer.h"
#include "stromx/cvsupport/DummyCamera.h"
#include "stromx/cvsupport/Clip.h"
#include "stromx/cvsupport/ConstImage.h"
#include "stromx/cvsupport/ConvertPixelType.h"
#include "stromx/cvsupport/Cvsupport.h"
#include "stromx/cvsupport/Flicker.h"
#include "stromx/cvsupport/Image.h"
#include "stromx/cvsupport/Matrix.h"
#include "stromx/cvsupport/ReadDirectory.h"
#include <stromx/runtime/Exception.h>
#include <stromx/runtime/Locale.h>
#include <stromx/runtime/Registry.h>

namespace stromx
{
    namespace cvsupport
    {
        std::locale locale;
    }
}

void stromxCvsupportRegister(stromx::runtime::Registry& registry)
{
    using namespace stromx::cvsupport;
    
    locale = stromx::runtime::Locale::generate(STROMX_CVSUPPORT_LOCALE_DIR,
                                               STROMX_CVSUPPORT_LOCALE_DOMAIN);
    
    registry.registerData(new Image);
    registry.registerData(new Matrix);
    
    registry.registerOperator(new AdjustRgbChannels);
    registry.registerOperator(new Buffer);
    registry.registerOperator(new DummyCamera);
    registry.registerOperator(new Clip);
    registry.registerOperator(new ConstImage);
    registry.registerOperator(new ConvertPixelType);
    registry.registerOperator(new Flicker);
    registry.registerOperator(new ReadDirectory);
}
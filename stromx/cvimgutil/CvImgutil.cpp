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

#include "stromx/cvimgutil/AdjustRgbChannels.h"
#include "stromx/cvimgutil/Buffer.h"
#include "stromx/cvimgutil/Camera.h"
#include "stromx/cvimgutil/Clip.h"
#include "stromx/cvimgutil/ConstImage.h"
#include "stromx/cvimgutil/ConvertPixelType.h"
#include "stromx/cvimgutil/Image.h"
#include "stromx/cvimgutil/CvImgutil.h"
#include "stromx/cvimgutil/WebCamera.h"
#include <stromx/runtime/Exception.h>
#include <stromx/runtime/Registry.h>

void stromxRegisterImgutil(stromx::runtime::Registry& registry)
{
    using namespace stromx::cvimgutil;
    
    registry.registerData(new Image);
    
    registry.registerOperator(new AdjustRgbChannels);
    registry.registerOperator(new Buffer);
    registry.registerOperator(new Camera);
    registry.registerOperator(new Clip);
    registry.registerOperator(new ConstImage);
    registry.registerOperator(new ConvertPixelType);
    registry.registerOperator(new WebCamera);
}

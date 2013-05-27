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

#include "stromx/imgutil/AdjustRgbChannels.h"
#include "stromx/imgutil/Buffer.h"
#include "stromx/imgutil/Camera.h"
#include "stromx/imgutil/Clip.h"
#include "stromx/imgutil/ConstImage.h"
#include "stromx/imgutil/ConvertPixelType.h"
#include "stromx/imgutil/Image.h"
#include "stromx/imgutil/Imgutil.h"
#include "stromx/imgutil/WebCamera.h"
#include <stromx/runtime/Exception.h>
#include <stromx/runtime/Registry.h>

void stromxRegisterImgutil(stromx::runtime::Registry& registry)
{
    using namespace stromx::imgutil;
    
    registry.registerData(new Image);
    
    registry.registerOperator(new AdjustRgbChannels);
    registry.registerOperator(new Buffer);
    registry.registerOperator(new Camera);
    registry.registerOperator(new Clip);
    registry.registerOperator(new ConstImage);
    registry.registerOperator(new ConvertPixelType);
    registry.registerOperator(new WebCamera);
}

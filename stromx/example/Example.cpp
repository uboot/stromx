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

#include "AdjustRgbChannels.h"
#include "Example.h"
#include "Blur.h"
#include "Buffer.h"
#include "Camera.h"
#include "Canny.h"
#include "Clip.h"
#include "Close.h"
#include "ConstImage.h"
#include "ConvertPixelType.h"
#include "Dilate.h"
#include "Erode.h"
#include "Image.h"
#include "HoughLinesP.h"
#include "Open.h"
#include "WebCamera.h"
#include <stromx/runtime/Exception.h>
#include <stromx/runtime/Registry.h>

void stromxRegisterExample(stromx::runtime::Registry& registry)
{
    using namespace stromx::example;
    
    registry.registerData(new Image);
    
    registry.registerOperator(new AdjustRgbChannels);
    registry.registerOperator(new Blur);
    registry.registerOperator(new Buffer);
    registry.registerOperator(new Camera);
    registry.registerOperator(new Canny);
    registry.registerOperator(new Clip);
    registry.registerOperator(new Close);
    registry.registerOperator(new ConstImage);
    registry.registerOperator(new ConvertPixelType);
    registry.registerOperator(new Dilate);
    registry.registerOperator(new Erode);
    registry.registerOperator(new HoughLinesP);
    registry.registerOperator(new Open);
    registry.registerOperator(new WebCamera);
}

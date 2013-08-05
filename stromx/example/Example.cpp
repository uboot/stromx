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

#include "stromx/example/AdjustRgbChannels.h"
#include "stromx/example/Buffer.h"
#include "stromx/example/Blur.h"
#include "stromx/example/Camera.h"
#include "stromx/example/Canny.h"
#include "stromx/example/Clip.h"
#include "stromx/example/Close.h"
#include "stromx/example/ConstImage.h"
#include "stromx/example/ConvertPixelType.h"
#include "stromx/example/Dilate.h"
#include "stromx/example/Erode.h"
#include "stromx/example/Example.h"
#include "stromx/example/Image.h"
#include "stromx/example/HoughLinesP.h"
#include "stromx/example/Open.h"
#include "stromx/example/WebCamera.h"
#include <stromx/runtime/Exception.h>
#include <stromx/runtime/Registry.h>

void stromxExampleRegister(stromx::runtime::Registry& registry)
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

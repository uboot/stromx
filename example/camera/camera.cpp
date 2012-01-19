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

#include <stromx/core/Factory.h>
#include <stromx/core/XmlReader.h>
#include <stromx/core/Stream.h>
#include <stromx/core/Core.h>
#include <stromx/core/Operator.h>
#include <stromx/core/ReadAccess.h>
#include <stromx/core/Image.h>

#include <stromx/base/Base.h>
#include <stromx/base/Camera.h>

#include <iostream>

using namespace stromx;

int main (int argc, char* argv[])
{
    core::Factory factory;
    
    stromxRegisterCore(factory);
    stromxRegisterBase(factory);
    
    core::Stream* stream = core::XmlReader().readStream("camera.xml", factory);
    
    stream->start();
    
    core::Operator* camera = stream->operators()[0];
    core::Operator* canny = stream->operators()[2];
    
    for(unsigned int i = 0; i < 5; ++i)
    {
        core::DataContainer data = canny->getOutputData(0);
        core::ReadAccess<core::Image> image(data);
        std::cout << "Received image " <<  image().height() << "x" << image().width() << std::endl;
        
        canny->clearOutputData(0);
        camera->clearOutputData(1);
    }
    
    stream->stop();
    stream->join();
    
    delete stream;
}

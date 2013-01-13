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

#include "../runtime/ExportOperatorKernel.h"

#include <boost/python.hpp>

#include <stromx/runtime/Registry.h>
#include <stromx/example/Example.h>
#include <stromx/example/Blur.h>
#include <stromx/example/Buffer.h>
#include <stromx/example/ConstImage.h>
#include <stromx/example/AdjustRgbChannels.h>
#include <stromx/example/Camera.h>
#include <stromx/example/Canny.h>
#include <stromx/example/Close.h>
#include <stromx/example/ConvertPixelType.h>
#include <stromx/example/Clip.h>
#include <stromx/example/Dilate.h>
#include <stromx/example/Erode.h>
#include <stromx/example/HoughLinesP.h>
#include <stromx/example/Open.h>

void exportImage();

using namespace boost::python;
using namespace stromx::example;
    
BOOST_PYTHON_MODULE(libbase)
{
    def("registerExample", stromxRegisterExample);
    
    exportImage();
    
    stromx::python::exportOperatorKernel<AdjustRgbChannels>("AdjustRgbChannels");
    stromx::python::exportOperatorKernel<Blur>("Blur");
    stromx::python::exportOperatorKernel<Buffer>("Buffer");
    stromx::python::exportOperatorKernel<Camera>("Camera");
    stromx::python::exportOperatorKernel<Canny>("Canny");
    stromx::python::exportOperatorKernel<Clip>("Clip");
    stromx::python::exportOperatorKernel<Close>("Close");
    stromx::python::exportOperatorKernel<ConstImage>("ConstImage");
    stromx::python::exportOperatorKernel<ConvertPixelType>("ConvertPixelType");
    stromx::python::exportOperatorKernel<Dilate>("Dilate");
    stromx::python::exportOperatorKernel<Erode>("Erode");
    stromx::python::exportOperatorKernel<HoughLinesP>("HoughLinesP");
    stromx::python::exportOperatorKernel<Open>("Open");
}
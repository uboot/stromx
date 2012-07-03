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

#include "../core/ExportOperatorKernel.h"

#include <boost/python.hpp>

#include <stromx/core/Registry.h>
#include <stromx/base/Base.h>
#include <stromx/base/Blur.h>
#include <stromx/base/Buffer.h>
#include <stromx/base/ConstImage.h>
#include <stromx/base/AdjustRgbChannels.h>
#include <stromx/base/Camera.h>
#include <stromx/base/Canny.h>
#include <stromx/base/ConvertPixelType.h>
#include <stromx/base/Clip.h>

void exportImage();

using namespace boost::python;
using namespace stromx::base;
    
BOOST_PYTHON_MODULE(libbase)
{
    def("registerBase", stromxRegisterBase);
    
    exportImage();
    
    stromx::python::exportOperatorKernel<AdjustRgbChannels>("AdjustRgbChannels");
    stromx::python::exportOperatorKernel<Blur>("Blur");
    stromx::python::exportOperatorKernel<Buffer>("Buffer");
    stromx::python::exportOperatorKernel<Camera>("Camera");
    stromx::python::exportOperatorKernel<Canny>("Canny");
    stromx::python::exportOperatorKernel<Clip>("Clip");
    stromx::python::exportOperatorKernel<ConstImage>("ConstImage");
    stromx::python::exportOperatorKernel<ConvertPixelType>("ConvertPixelType");
}
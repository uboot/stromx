/* 
*  Copyright 2013 Matthias Fuchs
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

#include <python/stromx/runtime/ExportOperatorKernel.h>

#include <boost/python.hpp>

#include <stromx/cvhighgui/Cvhighgui.h>
#include <stromx/cvhighgui/VideoCapture.h>
#include <stromx/runtime/Registry.h>

using namespace boost::python;
using namespace stromx::cvhighgui;
    
BOOST_PYTHON_MODULE(libcvhighgui)
{
    def("register", stromxCvhighguiRegister);
    
    stromx::python::exportOperatorKernel<VideoCapture>("VideoCapture");
}
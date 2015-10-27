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

#include <python/stromx/runtime/ExportOperatorKernel.h>

#include <boost/python.hpp>

#include <stromx/runtime/Registry.h>
#include <stromx/cvml/Svm.h>

void exportImage();
void exportMatrix();

using namespace boost::python;
using namespace stromx::cvml;
    
BOOST_PYTHON_MODULE(libcvml)
{
    def("register", stromxCvmlRegister);
    
    stromx::python::exportOperatorKernel<Svm>("Svm");
}
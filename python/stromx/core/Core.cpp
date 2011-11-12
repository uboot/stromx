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

#include <boost/python.hpp>

#include <stromx/core/Core.h>
#include <stromx/core/Registry.h>

void exportFactory();
void exportData();
void exportDataContainer();
void exportDataVariant();
void exportDescription();
void exportException();
void exportImage();
void exportOperator();
void exportPrimitive();
void exportReadAccess();
void exportRegistry();
void exportStream();
void exportVersion();
void exportWriteAccess();

BOOST_PYTHON_MODULE(libcore)
{
    using namespace boost::python;
    
    def("registerCore", registerCore);
    
    exportData();
    exportDataContainer();
    exportDataVariant();
    exportDescription();
    exportException();
    exportImage();
    exportOperator();
    exportPrimitive();
    exportReadAccess();
    exportRegistry();
    exportFactory();
    exportStream();
    exportVersion();    
    exportWriteAccess();
}
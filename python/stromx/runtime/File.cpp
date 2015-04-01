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

#include "ExportVector.h"

#include <stromx/runtime/File.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;


namespace
{   
    std::auto_ptr<File> allocate(const std::string & path, const File::OpenMode mode)
    {
        return std::auto_ptr<File>(new File(path, mode));
    }
}

void exportFile()
{
    scope in_File =
    class_<File, bases<Data>, std::auto_ptr<File> >("File", no_init)
        .def("__init__", make_constructor(&allocate))
        .def("path", &File::path, return_internal_reference<>())
        .def("mode", &File::mode)
        .def("extension", &File::extension, return_internal_reference<>())
    ;
        
    enum_<File::OpenMode>("OpenMode")
        .value("TEXT", File::TEXT)
        .value("BINARY", File::BINARY)
        ;
            
    implicitly_convertible< std::auto_ptr<File>, std::auto_ptr<Data> >();
}
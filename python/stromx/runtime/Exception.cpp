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

#include <stromx/runtime/Exception.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

void exportException()
{                 
    class_<Exception>("Exception", no_init)
        .def("what", &Exception::what)
    ;    
    
    class_<AccessEmpty, bases<Exception> >("AccessEmpty", no_init);
    
    class_<BadCast, bases<Exception> >("BadCast", no_init);
    
    class_<DataException, bases<Exception> >("DataException", no_init)
        .def("package", &DataException::package, return_internal_reference<>())
        .def("type", &DataException::type, return_internal_reference<>())
    ;
    
    class_<DeserializationError, bases<DataException> >("DeserializationError", no_init);
    
    class_<SerializationError, bases<DataException> >("SerializationError", no_init);
    
    class_<FactoryException, bases<Exception> >("FactoryException", no_init)
        .def("package", &FactoryException::package, return_internal_reference<>())
        .def("type", &FactoryException::type, return_internal_reference<>())
    ;
    
    class_<DataAllocationFailed, bases<FactoryException> >("DataAllocationFailed", no_init);
    
    class_<OperatorAllocationFailed, bases<FactoryException> >("OperatorAllocationFailed", no_init);
    
    class_<FileException, bases<Exception> >("FileException", no_init)
        .def("filename", &FileException::filename, return_internal_reference<>())
        .def("container", &FileException::container, return_internal_reference<>())
    ;
    
    class_<FileAccessFailed, bases<FileException> >("FileAccessFailed", no_init);
    
    class_<InconsistentFileContent, bases<FileException> >("InconsistentFileContent", no_init);
    
    class_<InvalidFileFormat, bases<FileException> >("InvalidFileFormat", no_init);
    
    class_<InternalError, bases<Exception> >("InternalError", no_init);
    
    class_<Interrupt, bases<Exception> >("Interrupt", no_init);
    
    class_<NoInputFile, bases<Exception> >("NoInputFile", no_init);
    
    class_<NotImplemented, bases<Exception> >("NotImplemented", no_init);
    
    class_<OutOfMemory, bases<Exception> >("OutOfMemory", no_init);
    
    class_<Timeout, bases<Exception> >("Timeout", no_init);
    
    class_<WrongArgument, bases<Exception> >("WrongArgument", no_init);
    
    class_<WrongState, bases<Exception> >("WrongState", no_init);
    
    class_<WrongId, bases<Exception> >("WrongId", no_init);
}

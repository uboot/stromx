 
/* 
*  Copyright 2014 Matthias Fuchs
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

#include <stromx/runtime/DirectoryFileInput.h>
#include <stromx/runtime/ZipFileInput.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{
    struct InputProviderWrap : InputProvider, wrapper<InputProvider>
    {         
        std::istream & text()
        {
            return this->get_override("text")();
        }
        
        std::istream & openFile(const std::string & ext = "", const OpenMode mode = BINARY)
        {
            return this->get_override("openFile")(ext, mode);
        }

        std::istream & file()
        {
            return this->get_override("file")();
        }
        
    };
    
    struct FileInputWrap : FileInput, wrapper<FileInput>
    {
        void initialize(const std::string & filename)
        {
            this->get_override("initialize")(filename);
        }
        
        const std::string & getFilename() const
        {
            return this->get_override("getFilename")();
        }
        
        const std::string getText() const
        {
            return this->get_override("getText")();
        }
        
        void close()
        {
            this->get_override("close")();
        }
    };
    
    std::string readFromStream(std::istream & in)
    {
        // TODO: attribute to http://stackoverflow.com/a/3203502
        std::istreambuf_iterator<char> eos;
        return std::string(std::istreambuf_iterator<char>(in), eos);
    }
}

void exportInputProvider()
{    
    {
        scope in_InputProvider =
        class_<InputProviderWrap, boost::noncopyable>("InputProvider", no_init)
            .def("text", pure_virtual(&InputProvider::text), return_internal_reference<>())
            .def("openFile", pure_virtual(&InputProvider::openFile), return_internal_reference<>())
            .def("hasFile", pure_virtual(&InputProvider::hasFile))
            .def("file", pure_virtual(&InputProvider::file), return_internal_reference<>())
        ;
        
        enum_<InputProvider::OpenMode>("OpenMode")
            .value("TEXT", InputProvider::TEXT)
            .value("BINARY", InputProvider::BINARY)
            ;
    }
    
    class_<FileInputWrap, bases<InputProvider>, boost::noncopyable>("FileInput", no_init)
        .def("initialize", pure_virtual(&FileInput::initialize))
        .def("close", pure_virtual(&FileInput::close))
    ;
    
    class_<DirectoryFileInput, bases<FileInput>, boost::noncopyable>("DirectoryFileInput", init<std::string>())
    ;
    
    class_<ZipFileInput, bases<FileInput>, boost::noncopyable>("ZipFileInput", init<std::string>())
    ;
    
    class_<std::istream, boost::noncopyable>("ostream", no_init)
        .def("read", &readFromStream)
    ;
}


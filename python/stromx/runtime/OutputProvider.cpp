 
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

#include <stromx/runtime/DirectoryFileOutput.h>
#include <stromx/runtime/ZipFileOutput.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{
    struct OutputProviderWrap : OutputProvider, wrapper<OutputProvider>
    {         
        std::ostream & text()
        {
            return this->get_override("text")();
        }
        
        std::ostream & openFile(const std::string & ext = "", const OpenMode mode = BINARY)
        {
            return this->get_override("openFile")(ext, mode);
        }

        std::ostream & file()
        {
            return this->get_override("file")();
        }
        
    };
    
    struct FileOutputWrap : FileOutput, wrapper<FileOutput>
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
    
    void writeToStream(std::ostream & out, const std::string & data)
    {
        out << data;
    }
}

void exportOutputProvider()
{    
    {
        scope in_OutputProvider =
        class_<OutputProviderWrap, boost::noncopyable>("OutputProvider", no_init)
            .def("text", pure_virtual(&OutputProvider::text), return_internal_reference<>())
            .def("openFile", pure_virtual(&OutputProvider::openFile), return_internal_reference<>())
            .def("file", pure_virtual(&OutputProvider::file), return_internal_reference<>())
        ;
        
        enum_<OutputProvider::OpenMode>("OpenMode")
            .value("TEXT", OutputProvider::TEXT)
            .value("BINARY", OutputProvider::BINARY)
            ;
    }
    
    class_<FileOutputWrap, bases<OutputProvider>, boost::noncopyable>("FileOutput", no_init)
        .def("initialize", pure_virtual(&FileOutput::initialize))
        .def("getFilename", pure_virtual(&FileOutput::getFilename), return_internal_reference<>())
        .def("getText", pure_virtual(&FileOutput::getText))
        .def("close", pure_virtual(&FileOutput::close))
    ;
    
    class_<DirectoryFileOutput, bases<FileOutput>, boost::noncopyable>("DirectoryFileOutput", init<std::string>())
    ;
    
    class_<ZipFileOutput, bases<FileOutput>, boost::noncopyable>("ZipFileOutput", init<std::string>())
    ;
    
    class_<std::ostream, boost::noncopyable>("ostream", no_init)
        .def("write", &writeToStream)
    ;
}


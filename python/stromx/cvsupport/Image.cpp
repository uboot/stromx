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

#include <stromx/runtime/Data.h>
#include <stromx/runtime/Image.h>

#include <stromx/cvsupport/Image.h>

#include <memory>
#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{  
    std::auto_ptr<stromx::cvsupport::Image> allocateFromDimension(const unsigned int width, const unsigned int height, const Image::PixelType pixelType)
    {
        return std::auto_ptr<stromx::cvsupport::Image>(new stromx::cvsupport::Image(width, height, pixelType));
    }
    
    std::auto_ptr<stromx::cvsupport::Image> allocateFromFile(const std::string & filename)
    {
        return std::auto_ptr<stromx::cvsupport::Image>(new stromx::cvsupport::Image(filename));
    } 
    
    std::auto_ptr<stromx::cvsupport::Image> allocateFromFileWithAccess(const std::string & filename, const stromx::cvsupport::Image::Conversion access)
    {
        return std::auto_ptr<stromx::cvsupport::Image>(new stromx::cvsupport::Image(filename, access));
    } 
}

void exportImage()
{
    scope in_Operator =
    class_<stromx::cvsupport::Image, bases<stromx::runtime::Image>, std::auto_ptr<stromx::cvsupport::Image> >("Image", no_init)
        .def("__init__", make_constructor(&allocateFromFile))
        .def("__init__", make_constructor(&allocateFromDimension))
        .def("__init__", make_constructor(&allocateFromFileWithAccess))
        .def<void (stromx::cvsupport::Image::*)(const std::string &) const>("save", &stromx::cvsupport::Image::save)
        .def<void (stromx::cvsupport::Image::*)(const std::string &)>("open", &stromx::cvsupport::Image::open)
        .def<void (stromx::cvsupport::Image::*)(const std::string &, const stromx::cvsupport::Image::Conversion)>("open", &stromx::cvsupport::Image::open)
        .def<void (stromx::cvsupport::Image::*)(const unsigned int, const unsigned int, const Image::PixelType)>("resize", &stromx::cvsupport::Image::resize)
    ;
     
    enum_<stromx::cvsupport::Image::Conversion>("Conversion")
        .value("UNCHANGED", stromx::cvsupport::Image::UNCHANGED)
        .value("GRAYSCALE", stromx::cvsupport::Image::GRAYSCALE)
        .value("COLOR", stromx::cvsupport::Image::COLOR)
    ;
    
    implicitly_convertible< std::auto_ptr<stromx::cvsupport::Image>, std::auto_ptr<Data> >();
}

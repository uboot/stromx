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

#include <stromx/core/DataContainer.h>
#include <stromx/core/DataVariant.h>
#include <stromx/core/Data.h>
#include <stromx/core/Image.h>

#include <stromx/base/Image.h>

#include <auto_ptr.h>
#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

namespace
{  
    std::auto_ptr<stromx::base::Image> allocateFromDimension(const unsigned int width, const unsigned int height, const Image::PixelType pixelType)
    {
        return std::auto_ptr<stromx::base::Image>(new stromx::base::Image(width, height, pixelType));
    }
    
    std::auto_ptr<stromx::base::Image> allocateFromFile(const std::string & filename)
    {
        return std::auto_ptr<stromx::base::Image>(new stromx::base::Image(filename));
    } 
}

void exportImage()
{
    class_<stromx::base::Image, bases<Data, Image>, std::auto_ptr<stromx::base::Image> >("Image", no_init)
        .def("__init__", make_constructor(&allocateFromFile))
        .def("__init__", make_constructor(&allocateFromDimension))
     ;
     
    implicitly_convertible< std::auto_ptr<stromx::base::Image>, std::auto_ptr<Data> >();
}

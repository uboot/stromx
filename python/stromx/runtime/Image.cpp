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

#include <stromx/runtime/Image.h>
#include <stromx/runtime/Version.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{
    struct ImageWrap : Image, wrapper<Image>
    {        
        const Version & version() const
        {
            return this->get_override("version")();
        }
        
        const std::string & type() const
        {
            return this->get_override("type")();
        }
        
        const std::string & package() const
        {
            return this->get_override("package")();
        }
        
        const VariantHandle & variant() const
        {
            return this->get_override("variant")();
        }
        
        Data* clone() const
        {
            return this->get_override("clone")();
        }
        
        uint8_t* buffer()
        {
            return this->get_override("buffer")();
        }
        
        unsigned int bufferSize() const
        {
            return this->get_override("bufferSize")();
        }
        
        unsigned int width() const
        {
            return this->get_override("width")();
        }
        
        unsigned int height() const
        {
            return this->get_override("height")();
        }
        
        unsigned int stride() const
        {
            return this->get_override("stride")();
        }
        
        PixelType pixelType() const
        {
            return this->get_override("pixelType")();
        }
        
        uint8_t* data()
        {
            return this->get_override("data")();
        }
        
        const uint8_t* data() const
        {
            return this->get_override("data")();
        }
        
        unsigned int rows() const
        {
            return this->get_override("rows")();
        }
        
        unsigned int cols() const
        {
            return this->get_override("cols")();
        }
        
        ValueType valueType() const
        {
            return this->get_override("valueType")();
        }
        
        unsigned int valueSize() const
        {
            return this->get_override("valueSize")();
        }
    };
}

void exportImage()
{   
       
    scope in_Image = 
    class_<ImageWrap, bases<Matrix>, boost::noncopyable>("Image", no_init)
        .def("width", pure_virtual(&Image::width))
        .def("height", pure_virtual(&Image::height))
        .def("pixelType", pure_virtual(&Image::pixelType))
        .def<unsigned int (stromx::runtime::Image::*)() const>("pixelSize", &Image::pixelSize)
        .def<unsigned int (stromx::runtime::Image::*)() const>("depth", &Image::depth)
        .def<unsigned int (stromx::runtime::Image::*)() const>("numChannels", &Image::numChannels)
    ;
    
    enum_<Image::PixelType>("PixelType")
        .value("NONE", Image::NONE)
        .value("MONO_8", Image::MONO_8)
        .value("MONO_16", Image::MONO_16)
        .value("RGB_24", Image::RGB_24)
        .value("RGB_48", Image::RGB_48)
        .value("BGR_24", Image::BGR_24)
        .value("BGR_48", Image::BGR_48)
        .value("BAYERBG_8", Image::BAYERBG_8)
        .value("BAYERBG_16", Image::BAYERBG_16)
        .value("BAYERGB_8", Image::BAYERGB_8)
        .value("BAYERGB_16", Image::BAYERGB_16)
    ;
}
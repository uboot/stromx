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

#include <stromx/core/Image.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

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
        
        const DataVariant & variant() const
        {
            return this->get_override("variant")();
        }
        
        Data* const clone() const
        {
            return this->get_override("clone")();
        }
        
        uint8_t* const buffer()
        {
            return this->get_override("buffer")();
        }
        
        const unsigned int bufferSize() const
        {
            return this->get_override("bufferSize")();
        }
        
        const unsigned int width() const
        {
            return this->get_override("width")();
        }
        
        const unsigned int height() const
        {
            return this->get_override("height")();
        }
        
        const unsigned int stride() const
        {
            return this->get_override("stride")();
        }
        
        const PixelType pixelType() const
        {
            return this->get_override("pixelType")();
        }
        
        const unsigned int pixelSize() const
        {
            return this->get_override("pixelSize")();
        }
        
        uint8_t* const data()
        {
            return this->get_override("data")();
        }
        
        const uint8_t* const data() const
        {
            return this->get_override("data")();
        }
        
        void setVariant(const DataVariant& variant)
        {
            this->get_override("setVariant")();
        }
        
        void initialize(const unsigned int width, 
                        const unsigned int height, 
                        const unsigned int stride, 
                        uint8_t* const data, 
                        const PixelType pixelType)
        {
            this->get_override("initialize")();
        }
    };
}

void exportImage()
{     
    enum_<Image::PixelType>("ImagePixelType")
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
        
    class_<ImageWrap, bases<Data>, boost::noncopyable>("Image")
        .def("bufferSize", pure_virtual(&Image::bufferSize))
        .def("width", pure_virtual(&Image::width))
        .def("height", pure_virtual(&Image::height))
        .def("stride", pure_virtual(&Image::stride))
        .def("pixelSize", pure_virtual(&Image::pixelSize))
        .def("_data", pure_virtual(reinterpret_cast<unsigned int (Image::*)()>(static_cast<uint8_t* const(Image::*)()>(&Image::data))))
    ;
}
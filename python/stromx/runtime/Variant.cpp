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

#include <stromx/runtime/Variant.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

void exportVariant()
{         
    scope in_Variant =
    class_<Variant>("Variant", no_init);
    
    in_Variant.attr("NONE") = Variant::NONE;
    in_Variant.attr("DATA") = Variant::DATA; 
    in_Variant.attr("BOOL") = Variant::BOOL; 
    in_Variant.attr("TRIGGER") = Variant::TRIGGER; 
    in_Variant.attr("ENUM") = Variant::ENUM; 
    in_Variant.attr("INT") = Variant::INT; 
    in_Variant.attr("INT_8") = Variant::INT_8;
    in_Variant.attr("INT_16") = Variant::INT_16; 
    in_Variant.attr("INT_32") = Variant::INT_32;  
    in_Variant.attr("INT_64") = Variant::INT_64;
    in_Variant.attr("UINT") = Variant::UINT;
    in_Variant.attr("UINT_8") = Variant::UINT_8;
    in_Variant.attr("UINT_16") = Variant::UINT_16;
    in_Variant.attr("UINT_32") = Variant::UINT_32;
    in_Variant.attr("UINT_64") = Variant::UINT_64;
    in_Variant.attr("FLOAT") = Variant::FLOAT; 
    in_Variant.attr("FLOAT_32") = Variant::FLOAT_32; 
    in_Variant.attr("FLOAT_64") = Variant::FLOAT_64; 
    in_Variant.attr("STRING") = Variant::STRING;
    in_Variant.attr("LIST") = Variant::LIST;
    in_Variant.attr("IMAGE") = Variant::IMAGE;
    in_Variant.attr("MONO_IMAGE") = Variant::MONO_IMAGE;
    in_Variant.attr("BGR_24_IMAGE") = Variant::BGR_24_IMAGE;
    in_Variant.attr("RGB_IMAGE") = Variant::RGB_IMAGE;
    in_Variant.attr("BGR_24_IMAGE") = Variant::BGR_24_IMAGE;
    in_Variant.attr("RGB_48_IMAGE") = Variant::RGB_48_IMAGE;
    in_Variant.attr("BGR_48_IMAGE") = Variant::BGR_48_IMAGE;
    in_Variant.attr("MONO_8_IMAGE") = Variant::MONO_8_IMAGE;
    in_Variant.attr("MONO_16_IMAGE") = Variant::MONO_16_IMAGE;
    in_Variant.attr("BAYERBG_8_IMAGE") = Variant::BAYERBG_8_IMAGE;
    in_Variant.attr("BAYERGB_8_IMAGE") = Variant::BAYERGB_8_IMAGE;
    in_Variant.attr("BAYERBG_16_IMAGE") = Variant::BAYERBG_16_IMAGE;
    in_Variant.attr("BAYERGB_16_IMAGE") = Variant::BAYERGB_16_IMAGE;
    in_Variant.attr("MATRIX") = Variant::MATRIX;
    in_Variant.attr("INT_MATRIX") = Variant::INT_MATRIX;
    in_Variant.attr("UINT_MATRIX") = Variant::UINT_MATRIX;
    in_Variant.attr("INT_8_MATRIX") = Variant::INT_8_MATRIX;
    in_Variant.attr("UINT_8_MATRIX") = Variant::UINT_8_MATRIX;
    in_Variant.attr("INT_16_MATRIX") = Variant::INT_16_MATRIX;
    in_Variant.attr("UINT_16_MATRIX") = Variant::UINT_16_MATRIX;
    in_Variant.attr("INT_32_MATRIX") = Variant::INT_32_MATRIX;
    in_Variant.attr("UINT_32_MATRIX") = Variant::UINT_32_MATRIX;
    in_Variant.attr("FLOAT_MATRIX") = Variant::FLOAT_MATRIX;
    in_Variant.attr("FLOAT_32") = Variant::FLOAT_32;
    in_Variant.attr("FLOAT_64") = Variant::FLOAT_64;
    in_Variant.attr("FILE") = Variant::FILE;
    in_Variant.attr("TEXT_FILE") = Variant::TEXT_FILE;
    in_Variant.attr("BINARY_FILE") = Variant::BINARY_FILE;
}

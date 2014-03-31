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

#include <stromx/runtime/DataVariant.h>
#include <stromx/runtime/Data.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

void exportDataVariant()
{         
    scope in_DataVariant =
    class_<DataVariant>("DataVariant", no_init)
        .def("title", &DataVariant::title, return_value_policy<copy_const_reference>())
        .def("isVariant", &DataVariant::isVariant)
        .def("id", &DataVariant::id)
        .def("package", &DataVariant::package, return_internal_reference<>())
    ;
    
    in_DataVariant.attr("NONE") = DataVariant::NONE; 
    in_DataVariant.attr("DATA") = DataVariant::DATA; 
    in_DataVariant.attr("BOOL") = DataVariant::BOOL; 
    in_DataVariant.attr("ENUM") = DataVariant::ENUM; 
    in_DataVariant.attr("INT") = DataVariant::INT; 
    in_DataVariant.attr("INT_8") = DataVariant::INT_8;
    in_DataVariant.attr("INT_16") = DataVariant::INT_16; 
    in_DataVariant.attr("INT_32") = DataVariant::INT_32; 
    in_DataVariant.attr("UINT") = DataVariant::UINT;
    in_DataVariant.attr("UINT_8") = DataVariant::UINT_8;
    in_DataVariant.attr("UINT_16") = DataVariant::UINT_16;
    in_DataVariant.attr("UINT_32") = DataVariant::UINT_32;
    in_DataVariant.attr("FLOAT") = DataVariant::FLOAT; 
    in_DataVariant.attr("FLOAT_32") = DataVariant::FLOAT_32; 
    in_DataVariant.attr("FLOAT_64") = DataVariant::FLOAT_64; 
    in_DataVariant.attr("STRING") = DataVariant::STRING;
    in_DataVariant.attr("IMAGE") = DataVariant::IMAGE;
    in_DataVariant.attr("RGB_24_IMAGE") = DataVariant::RGB_24_IMAGE;
    in_DataVariant.attr("BGR_24_IMAGE") = DataVariant::BGR_24_IMAGE;
    in_DataVariant.attr("RGB_48_IMAGE") = DataVariant::RGB_48_IMAGE;
    in_DataVariant.attr("BGR_48_IMAGE") = DataVariant::BGR_48_IMAGE;
    in_DataVariant.attr("MONO_8_IMAGE") = DataVariant::MONO_8_IMAGE;
    in_DataVariant.attr("MONO_16_IMAGE") = DataVariant::MONO_16_IMAGE;
    in_DataVariant.attr("BAYERBG_8_IMAGE") = DataVariant::BAYERBG_8_IMAGE;
    in_DataVariant.attr("BAYERGB_8_IMAGE") = DataVariant::BAYERGB_8_IMAGE;
    in_DataVariant.attr("BAYERBG_16_IMAGE") = DataVariant::BAYERBG_16_IMAGE;
    in_DataVariant.attr("BAYERGB_16_IMAGE") = DataVariant::BAYERGB_16_IMAGE;
    in_DataVariant.attr("MATRIX") = DataVariant::MATRIX;
    in_DataVariant.attr("INT_MATRIX") = DataVariant::INT_MATRIX;
    in_DataVariant.attr("UINT_MATRIX") = DataVariant::UINT_MATRIX;
    in_DataVariant.attr("INT_8_MATRIX") = DataVariant::INT_8_MATRIX;
    in_DataVariant.attr("UINT_8_MATRIX") = DataVariant::UINT_8_MATRIX;
    in_DataVariant.attr("INT_16_MATRIX") = DataVariant::INT_16_MATRIX;
    in_DataVariant.attr("UINT_16_MATRIX") = DataVariant::UINT_16_MATRIX;
    in_DataVariant.attr("INT_32_MATRIX") = DataVariant::INT_32_MATRIX;
    in_DataVariant.attr("UINT_32_MATRIX") = DataVariant::UINT_32_MATRIX;
    in_DataVariant.attr("FLOAT_MATRIX") = DataVariant::FLOAT_MATRIX;
    in_DataVariant.attr("FLOAT_32") = DataVariant::FLOAT_32;
    in_DataVariant.attr("FLOAT_64") = DataVariant::FLOAT_64;
}

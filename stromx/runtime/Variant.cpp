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
 *  See the License for the specific language governing permisVariantsions and
 *  limitations under the License.
 */

#include "stromx/runtime/Locale.h"
#include "stromx/runtime/Variant.h"

namespace stromx
{
    namespace runtime
    {
        const VariantHandle Variant::NONE = VariantHandle(new Variant(NONE_ID));
        const VariantHandle Variant::DATA = VariantHandle(new Variant(DATA_ID));
        const VariantHandle Variant::TRIGGER = VariantHandle(new Variant(TRIGGER_ID));
        const VariantHandle Variant::PRIMITIVE = VariantHandle(new Variant(PRIMITIVE_ID));
        const VariantHandle Variant::BOOL = VariantHandle(new Variant(BOOL_ID));
        const VariantHandle Variant::ENUM = VariantHandle(new Variant(ENUM_ID));
        const VariantHandle Variant::INT = VariantHandle(new Variant(INT_ID));
        const VariantHandle Variant::UINT = VariantHandle(new Variant(UINT_ID));
        const VariantHandle Variant::INT_8 = VariantHandle(new Variant(INT_8_ID));
        const VariantHandle Variant::UINT_8 = VariantHandle(new Variant(UINT_8_ID));
        const VariantHandle Variant::INT_16 = VariantHandle(new Variant(INT_16_ID));
        const VariantHandle Variant::UINT_16 = VariantHandle(new Variant(UINT_16_ID));
        const VariantHandle Variant::INT_32 = VariantHandle(new Variant(INT_32_ID));
        const VariantHandle Variant::UINT_32 = VariantHandle(new Variant(UINT_32_ID));
        const VariantHandle Variant::INT_64 = VariantHandle(new Variant(INT_64_ID));
        const VariantHandle Variant::UINT_64 = VariantHandle(new Variant(UINT_64_ID));
        const VariantHandle Variant::FLOAT = VariantHandle(new Variant(FLOAT_ID));
        const VariantHandle Variant::FLOAT_32 = VariantHandle(new Variant(FLOAT_32_ID));
        const VariantHandle Variant::FLOAT_64 = VariantHandle(new Variant(FLOAT_64_ID));
        const VariantHandle Variant::LIST = VariantHandle(new Variant(LIST_ID));
        const VariantHandle Variant::MATRIX = VariantHandle(new Variant(MATRIX_ID));
        const VariantHandle Variant::INT_MATRIX = VariantHandle(new Variant(INT_MATRIX_ID));
        const VariantHandle Variant::UINT_MATRIX = VariantHandle(new Variant(UINT_MATRIX_ID));
        const VariantHandle Variant::INT_8_MATRIX = VariantHandle(new Variant(INT_8_MATRIX_ID));
        const VariantHandle Variant::UINT_8_MATRIX = VariantHandle(new Variant(UINT_8_MATRIX_ID));
        const VariantHandle Variant::INT_16_MATRIX = VariantHandle(new Variant(INT_16_MATRIX_ID));
        const VariantHandle Variant::UINT_16_MATRIX = VariantHandle(new Variant(UINT_16_MATRIX_ID));
        const VariantHandle Variant::INT_32_MATRIX = VariantHandle(new Variant(INT_32_MATRIX_ID));
        const VariantHandle Variant::UINT_32_MATRIX = VariantHandle(new Variant(UINT_32_MATRIX_ID));
        const VariantHandle Variant::FLOAT_MATRIX = VariantHandle(new Variant(FLOAT_MATRIX_ID));
        const VariantHandle Variant::FLOAT_32_MATRIX = VariantHandle(new Variant(FLOAT_32_MATRIX_ID));
        const VariantHandle Variant::FLOAT_64_MATRIX = VariantHandle(new Variant(FLOAT_64_MATRIX_ID));
        const VariantHandle Variant::STRING = VariantHandle(new Variant(STRING_ID));
        const VariantHandle Variant::IMAGE = VariantHandle(new Variant(IMAGE_ID));
        const VariantHandle Variant::MONO_IMAGE = VariantHandle(new Variant(MONO_IMAGE_ID));
        const VariantHandle Variant::RGB_IMAGE = VariantHandle(new Variant(RGB_IMAGE_ID));
        const VariantHandle Variant::MONO_8_IMAGE = VariantHandle(new Variant(MONO_8_IMAGE_ID));
        const VariantHandle Variant::RGB_24_IMAGE = VariantHandle(new Variant(RGB_24_IMAGE_ID));
        const VariantHandle Variant::BGR_24_IMAGE = VariantHandle(new Variant(BGR_24_IMAGE_ID));
        const VariantHandle Variant::BAYERBG_8_IMAGE = VariantHandle(new Variant(BAYERBG_8_IMAGE_ID));
        const VariantHandle Variant::BAYERGB_8_IMAGE = VariantHandle(new Variant(BAYERGB_8_IMAGE_ID));
        const VariantHandle Variant::BAYERBG_16_IMAGE = VariantHandle(new Variant(BAYERBG_16_IMAGE_ID));
        const VariantHandle Variant::BAYERGB_16_IMAGE = VariantHandle(new Variant(BAYERGB_16_IMAGE_ID));
        const VariantHandle Variant::MONO_16_IMAGE = VariantHandle(new Variant(MONO_16_IMAGE_ID));
        const VariantHandle Variant::RGB_48_IMAGE = VariantHandle(new Variant(RGB_48_IMAGE_ID));
        const VariantHandle Variant::BGR_48_IMAGE = VariantHandle(new Variant(BGR_48_IMAGE_ID));
        const VariantHandle Variant::FILE = VariantHandle(new Variant(FILE_ID));
        const VariantHandle Variant::BINARY_FILE = VariantHandle(new Variant(BINARY_FILE_ID));
        const VariantHandle Variant::TEXT_FILE = VariantHandle(new Variant(TEXT_FILE_ID));
        
        const std::string Variant::title() const
        {
            if (package() != STROMX_RUNTIME_PACKAGE_NAME)
                return "";
            
            switch(id())
            {
            case NONE_ID:
                return  L_("None");
            case DATA_ID:
                return  L_("Data");
            case TRIGGER_ID:
                return  L_("Trigger");
            case PRIMITIVE_ID:
                return  L_("Primitive");
            case BOOL_ID:
                return  L_("Bool");
            case ENUM_ID:
                return  L_("Enum");
            case INT_ID:
                return  L_("Int");
            case UINT_ID:
                return  L_("UInt");
            case INT_8_ID:
                return  L_("Int8");
            case UINT_8_ID:
                return  L_("UInt8");
            case INT_16_ID:
                return  L_("Int16");
            case UINT_16_ID:
                return  L_("UInt16");
            case INT_32_ID:
                return  L_("Int32");
            case UINT_32_ID:
                return  L_("UInt32");
            case INT_64_ID:
                return  L_("Int64");
            case UINT_64_ID:
                return  L_("UInt64");
            case FLOAT_ID:
                return  L_("Float");
            case FLOAT_32_ID:
                return  L_("Float32");
            case FLOAT_64_ID:
                return  L_("Float64");
            case LIST_ID:
                return  L_("List");
            case MATRIX_ID:
                return  L_("Matrix");
            case INT_MATRIX_ID:
                return  L_("Int matrix");
            case UINT_MATRIX_ID:
                return  L_("UInt matrix");
            case INT_8_MATRIX_ID:
                return  L_("Int8 matrix");
            case UINT_8_MATRIX_ID:
                return  L_("UInt8 matrix");
            case INT_16_MATRIX_ID:
                return  L_("Int16 matrix");
            case UINT_16_MATRIX_ID:
                return  L_("UInt16 matrix");
            case INT_32_MATRIX_ID:
                return  L_("Int32 matrix");
            case UINT_32_MATRIX_ID:
                return  L_("UInt32 matrix");
            case FLOAT_MATRIX_ID:
                return  L_("Float matrix");
            case FLOAT_32_MATRIX_ID:
                return  L_("Float32 matrix");
            case FLOAT_64_MATRIX_ID:
                return  L_("Float64 matrix");
            case STRING_ID:
                return  L_("String");
            case IMAGE_ID:
                return  L_("Image");
            case MONO_IMAGE_ID:
                return  L_("Mono image");
            case RGB_IMAGE_ID:
                return  L_("RGB image");
            case MONO_8_IMAGE_ID:
                return  L_("Mono image 8-bit");
            case RGB_24_IMAGE_ID:
                return  L_("RGB image 24-bit");
            case BGR_24_IMAGE_ID:
                return  L_("BGR image 24-bit");
            case BAYERBG_8_IMAGE_ID:
                return  L_("Bayer BG image 8-bit");
            case BAYERGB_8_IMAGE_ID:
                return  L_("Bayer GB image 8-bit");
            case BAYERBG_16_IMAGE_ID:
                return  L_("Bayer BG image 16-bit");
            case BAYERGB_16_IMAGE_ID:
                return  L_("Bayer GB image 16-bit");
            case MONO_16_IMAGE_ID:
                return  L_("Mono image 16-bit");
            case RGB_48_IMAGE_ID:
                return  L_("RGB image 48-bit");
            case BGR_48_IMAGE_ID:
                return  L_("BGR image 48-bit");
            case FILE_ID:
                return  L_("File");
            case BINARY_FILE_ID:
                return  L_("Binary file");
            case TEXT_FILE_ID:
                return  L_("Text file");
            default:
                return "";
            }
        }
        
        bool Variant::isVariant(const VariantInterface& variant) const
        {
            if (variant.compositeType() == VariantInterface::AND_COMPOSITE)
            {
                return isVariant(variant.lhs()) && isVariant(variant.rhs());
            }
            
            if (variant.compositeType() == VariantInterface::OR_COMPOSITE)
            {
                return isVariant(variant.lhs()) || isVariant(variant.rhs());
            }
            
            if (variant.package() != package())
                return false;
            
            if (variant.id() == DATA_ID)
                return true;
            
            if (variant.id() == id())
                return true;
            
            switch(variant.id())
            {
            case PRIMITIVE_ID:
                return isVariant(BOOL)
                    || isVariant(INT) 
                    || isVariant(UINT) 
                    || isVariant(FLOAT_32) 
                    || isVariant(FLOAT_64);
            case UINT_ID:
                return isVariant(UINT_8) 
                    || isVariant(UINT_16) 
                    || isVariant(UINT_32)
                    || isVariant(UINT_64);
            case UINT_32_ID:
                return isVariant(ENUM);
            case INT_ID:
                return isVariant(UINT)
                    || isVariant(INT_8) 
                    || isVariant(INT_16) 
                    || isVariant(INT_32)
                    || isVariant(INT_64);
            case FLOAT_ID:
                return isVariant(FLOAT_32)
                    || isVariant(FLOAT_64);
            case MATRIX_ID:
                return isVariant(INT_MATRIX)
                    || isVariant(UINT_MATRIX)
                    || isVariant(FLOAT_MATRIX)
                    || isVariant(IMAGE);
            case INT_MATRIX_ID:
                return isVariant(UINT_MATRIX)
                    || isVariant(INT_8_MATRIX)
                    || isVariant(INT_16_MATRIX)
                    || isVariant(INT_32_MATRIX);
            case UINT_MATRIX_ID:
                return isVariant(UINT_8_MATRIX)
                    || isVariant(UINT_16_MATRIX)
                    || isVariant(UINT_32_MATRIX);
            case FLOAT_MATRIX_ID:
                return isVariant(FLOAT_32_MATRIX)
                    || isVariant(FLOAT_64_MATRIX);
            case IMAGE_ID:
                return isVariant(MONO_IMAGE)
                    || isVariant(RGB_IMAGE)
                    || isVariant(BAYERBG_8_IMAGE)
                    || isVariant(BAYERGB_8_IMAGE)
                    || isVariant(BAYERBG_16_IMAGE)
                    || isVariant(BAYERGB_16_IMAGE);;
            case MONO_IMAGE_ID:
                return isVariant(MONO_8_IMAGE)
                    || isVariant(MONO_16_IMAGE);
            case RGB_IMAGE_ID:
                return isVariant(RGB_24_IMAGE)
                    || isVariant(BGR_24_IMAGE)
                    || isVariant(RGB_48_IMAGE)
                    || isVariant(BGR_48_IMAGE);
            case FILE_ID:
                return isVariant(BINARY_FILE)
                    || isVariant(TEXT_FILE);
            default:
                return false;
            }
        }
    }
}

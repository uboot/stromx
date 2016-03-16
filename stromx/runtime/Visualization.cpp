/* 
 *  Copyright 2016 Matthias Fuchs
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
#include "stromx/runtime/Visualization.h"

namespace stromx
{
    namespace runtime
    {
        const VariantHandle Visualization::POINT = VariantHandle(new Visualization(POINT_ID));
        const VariantHandle Visualization::LINE = VariantHandle(new Visualization(LINE_ID));
        const VariantHandle Visualization::LINE_SEGMENT = VariantHandle(new Visualization(LINE_SEGMENT_ID));
        const VariantHandle Visualization::POLYGON = VariantHandle(new Visualization(POLYGON_ID));
        const VariantHandle Visualization::POLYLINE = VariantHandle(new Visualization(POLYLINE_ID));
        const VariantHandle Visualization::RECTANGLE = VariantHandle(new Visualization(RECTANGLE_ID));
        const VariantHandle Visualization::ROTATED_RECTANGLE = VariantHandle(new Visualization(ROTATED_RECTANGLE_ID));
        const VariantHandle Visualization::ELLIPSE = VariantHandle(new Visualization(ELLIPSIS_ID));
        const VariantHandle Visualization::CIRCLE = VariantHandle(new Visualization(CIRCLE_ID));
        const VariantHandle Visualization::HISTOGRAM = VariantHandle(new Visualization(HISTOGRAM_ID));
        
        bool Visualization::isVariant(const VariantInterface& variant) const
        {
            if (variant.isVariant(Variant::DATA))
                return false;
                
            return Variant::isVariant(variant);
        }
        
        const std::string Visualization::title() const
        {
            if (package() != STROMX_RUNTIME_PACKAGE_NAME)
                return "";
            
            switch(id())
            {
            case POINT_ID:
                return  L_("Point");
            case LINE_ID:
                return  L_("Line");
            case LINE_SEGMENT_ID:
                return  L_("Line segment");
            case POLYGON_ID: 
                return  L_("Polygon");
            case POLYLINE_ID:
                return  L_("Polyline");
            case RECTANGLE_ID:
                return  L_("Rectangle");
            case ROTATED_RECTANGLE_ID:
                return  L_("Rotated rectangle");
            case ELLIPSIS_ID: 
                return  L_("Ellipsis");
            case CIRCLE_ID:
                return  L_("Circle");
            case HISTOGRAM_ID: 
                return  L_("Histogram");
            default:
                return "";
            }
        }
    }
}

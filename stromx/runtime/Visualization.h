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
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef STROMX_RUNTIME_VISUALIZATION_H
#define STROMX_RUNTIME_VISUALIZATION_H

#include "stromx/runtime/Variant.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief Visualization variants. */
        class STROMX_RUNTIME_API Visualization : public Variant
        {                   
        public:
            /** Matrix data which represents points. */
            const static VariantHandle POINT;
            
            /** Matrix data which represents lines. */
            const static VariantHandle LINE;
            
            /** Matrix data which represents line segments. */
            const static VariantHandle LINE_SEGMENT;
            
            /** Matrix data which represents polygons. */
            const static VariantHandle POLYGON;
            
            /** Matrix data which represents polylines. */
            const static VariantHandle POLYLINE;
            
            /** Matrix data which represents axis aligned rectangles. */
            const static VariantHandle RECTANGLE;
            
            /** Matrix data which represents rotated rectangles. */
            const static VariantHandle ROTATED_RECTANGLE;
            
            /** Matrix data which represents circles. */
            const static VariantHandle CIRCLE;
            
            /** Matrix data which represents rotated ellipses. */
            const static VariantHandle ELLIPSE;
            
            /** Matrix data which represents histograms. */
            const static VariantHandle HISTOGRAM;
            
            virtual const std::string title() const;
            virtual bool isVariant(const VariantInterface& variant) const;
        
        private:
            enum Types
            {
                POINT_ID = Variant::NUM_VARIANTS,
                LINE_ID,
                LINE_SEGMENT_ID,
                POLYGON_ID, 
                POLYLINE_ID,
                RECTANGLE_ID,
                ROTATED_RECTANGLE_ID,
                ELLIPSIS_ID,
                CIRCLE_ID,
                HISTOGRAM_ID
            };
            
            Visualization(const unsigned int id) : Variant(id)
            {}
        };
    }
}

#endif // STROMX_RUNTIME_VISUALIZATION_H

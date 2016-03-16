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

#include <stromx/runtime/Visualization.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

void exportVisualizationVariant()
{         
    scope in_Variant =
    class_<Visualization>("Visualization", no_init);
    
    in_Variant.attr("POINT") = Visualization::POINT;
    in_Variant.attr("LINE") = Visualization::LINE;
    in_Variant.attr("LINE_SEGMENT") = Visualization::LINE_SEGMENT;
    in_Variant.attr("POLYGON") = Visualization::POLYGON;
    in_Variant.attr("POLYLINE") = Visualization::POLYLINE;
    in_Variant.attr("RECTANGLE") = Visualization::RECTANGLE;
    in_Variant.attr("ROTATED_RECTANGLE") = Visualization::ROTATED_RECTANGLE;
    in_Variant.attr("ELLIPSE") = Visualization::ELLIPSE;                
    in_Variant.attr("CIRCLE") = Visualization::CIRCLE;               
    in_Variant.attr("HISTOGRAM") = Visualization::HISTOGRAM;                   
}

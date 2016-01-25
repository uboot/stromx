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

#include "stromx/runtime/DescriptionBase.h"
#include "stromx/runtime/Exception.h"

namespace stromx
{
    namespace runtime
    {
        const std::vector<EnumDescription> DescriptionBase::NO_DESCRIPTIONS = std::vector<EnumDescription>();
        const None DescriptionBase::NONE = None();
        
        void DescriptionBase::setVisualization(const VariantHandle & visualization)
        {
            if (! visualization.isValid())
                throw WrongArgument("Visualization of a description must be a valid variant.");
            
            m_visualization = visualization;
        }
    }
}
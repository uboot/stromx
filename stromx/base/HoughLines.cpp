/* 
*  Copyright 2012 Matthias Fuchs
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

#include "HoughLines.h"

#include "Image.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <stromx/core/DataContainer.h>
#include <stromx/core/DataProvider.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/ReadAccess.h>
#include <stromx/core/WriteAccess.h>
#include <stromx/core/NumericParameter.h>

namespace stromx
{
    using namespace core;

    namespace base
    {
        const std::string HoughLines::PACKAGE(STROMX_BASE_PACKAGE_NAME);
        const Version HoughLines::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR, BASE_VERSION_PATCH);
        const std::string HoughLines::TYPE("HoughLines");
        
        HoughLines::HoughLines()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupParameters())
        {
        }

        void HoughLines::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const Data& HoughLines::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            default:
                throw WrongParameterId(id, *this);
            }
        }    
                
        void HoughLines::execute(DataProvider& provider)
        {

        }
        
        const std::vector<const core::Description*> HoughLines::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            return inputs;
        }
        
        const std::vector<const Description*> HoughLines::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            return outputs;
        }
        
        const std::vector<const Parameter*> HoughLines::setupParameters()
        {
            std::vector<const core::Parameter*> parameters;
                                        
            return parameters;
        }
    } 
}
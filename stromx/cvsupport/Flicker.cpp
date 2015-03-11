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

#include "stromx/cvsupport/Flicker.h"
#include "stromx/cvsupport/Image.h"
#include "stromx/cvsupport/Locale.h"
#include "stromx/cvsupport/Utilities.h"
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/NumericParameter.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/WriteAccess.h>

#include <cstdlib>

namespace stromx
{
    using namespace runtime;
    
    namespace
    {
        template <class T>
        void applyFlicker(const Float64 amount, runtime::Image & image)
        {
            double coeff = 1.0 - (double(rand()) / RAND_MAX - 0.5) * 2 * amount;
            uint8_t* row = image.data();
            for (std::size_t i = 0; i < image.height(); ++i)
            {
                T* pixel = reinterpret_cast<T*>(row);
                for (std::size_t j = 0; j < image.width(); ++j)
                {
                    for (std::size_t k = 0; k < image.pixelSize(); ++k)
                    {
                        (*pixel) = cv::saturate_cast<T>((*pixel) * coeff);
                        pixel++;
                    }
                }
                
                row += image.stride();
            }
        }
    }

    namespace cvsupport
    {
        const std::string Flicker::TYPE("Flicker");
        const std::string Flicker::PACKAGE(STROMX_CVSUPPORT_PACKAGE_NAME);
        const Version Flicker::VERSION(0, 1, 0);
        
        Flicker::Flicker()
        : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
            m_amount(0.1)
        {
        }

        void Flicker::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case AMOUNT:
                    m_amount = stromx::runtime::data_cast<Float64>(value);
                    m_amount = std::max(0.0, std::min(double(1.0), double(m_amount)));
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const DataRef Flicker::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case AMOUNT:
                return m_amount;
            default:
                throw WrongParameterId(id, *this);
            }
        }  
        
        void Flicker::execute(DataProvider& provider)
        {
            Id2DataPair inputDataMapper(INPUT);
            provider.receiveInputData(inputDataMapper);
            
            DataContainer container = inputDataMapper.data();
            WriteAccess<runtime::Image> access(container);
            runtime::Image& image = access();
            
            switch(image.depth())
            {
            case 1:
                applyFlicker<uint8_t>(m_amount, image);
                break;
            case 2:
                applyFlicker<uint16_t>(m_amount, image);
                break;
            default:
                throw WrongInputType(INPUT, *this);
            }
            
            Id2DataPair outputDataMapper(OUTPUT, container);
            provider.sendOutputData( outputDataMapper);
        }
        
        const std::vector<const runtime::Description*> Flicker::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* input = new Description(INPUT, Variant::IMAGE);
            input->setTitle(L_("Input"));
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Description*> Flicker::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, Variant::IMAGE);
            output->setTitle(L_("Output"));
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Flicker::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            NumericParameter<Float64>* amount = new NumericParameter<Float64>(AMOUNT, Float64(0.0), Float64(1.0));
            amount->setTitle(L_("Relative amount"));
            amount->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(amount);
                                        
            return parameters;
        }
    } 
}

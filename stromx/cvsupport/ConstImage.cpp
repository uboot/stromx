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

#include <boost/assert.hpp>
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/OperatorException.h>
#include "stromx/cvsupport/ConstImage.h"
#include "stromx/cvsupport/Image.h"
#include "stromx/cvsupport/Utilities.h"

namespace stromx
{
    using namespace runtime;

    namespace cvsupport
    {
        const std::string ConstImage::TYPE("ConstImage");
        
        const std::string ConstImage::PACKAGE(STROMX_CVSUPPORT_PACKAGE_NAME);
        const Version ConstImage::VERSION(0, 1, 0);
        
        ConstImage::ConstImage()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
            m_image(0)
        {
            m_image = new Image(0, 0, runtime::Image::RGB_24);
        }
        
        ConstImage::~ConstImage()
        {
            delete m_image;
        }

        void ConstImage::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case IMAGE:
                {
                    delete m_image;
                    m_image = 0;
                    
                    const runtime::Image& image = stromx::runtime::data_cast<runtime::Image>(value);
                    m_image = new Image(image);
                    break;
                }
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const DataRef ConstImage::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case IMAGE:
                return *m_image;
            default:
                throw WrongParameterId(id, *this);
            }
        }  
        
        void ConstImage::execute(DataProvider& provider)
        {
            provider.unlockParameters();
            Data* outData = m_imageAccess();
            provider.lockParameters();
            
            if(! outData)
                outData = new cvsupport::Image(m_image->width(), m_image->height(), m_image->pixelType());
            
            cvsupport::Image* outImage = reinterpret_cast<cvsupport::Image*>(outData);
            
            if(outImage->pixelType() != m_image->pixelType()
                || outImage->width() != m_image->width()
                || outImage->height() != m_image->height())
            {
                outImage->resize(m_image->width(), m_image->height(), m_image->pixelType());
            }
            
            cv::Mat inCvImage = getOpenCvMat(*m_image);
            cv::Mat outCvImage = getOpenCvMat(*outImage);
            
            inCvImage.copyTo(outCvImage);
            
            DataContainer outContainer = DataContainer(outImage);
            m_imageAccess = RecycleAccess(outContainer);
            
            Id2DataPair outputDataMapper(OUTPUT, outContainer);
            provider.sendOutputData( outputDataMapper);
        }
        
        const std::vector<const runtime::Description*> ConstImage::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            return inputs;
        }
        
        const std::vector<const Description*> ConstImage::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, Variant::IMAGE);
            output->setTitle("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> ConstImage::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            Parameter* image = new Parameter(IMAGE, Variant::IMAGE);
            image->setTitle("Image");
            image->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(image);
                                        
            return parameters;
        }
    } 
}

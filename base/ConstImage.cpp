#include "ConstImage.h"

#include "Config.h"
#include "Image.h"
#include "Utilities.h"

#include <strom/DataContainer.h>
#include <strom/DataProvider.h>
#include <strom/Id2DataPair.h>
#include <strom/OperatorException.h>

using namespace strom;

namespace base
{
    const std::string ConstImage::TYPE("ConstImage");
    
    const std::string ConstImage::PACKAGE(PACKAGE_NAME);
    const Version ConstImage::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    ConstImage::ConstImage()
      : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
        m_image(0)
    {
        m_image = new Image(0, 0, strom::Image::MONO_8);
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
                
                try
                {
                    const strom::Image& image = dynamic_cast<const strom::Image&>(value);
                    m_image = new Image(image);
                }
                catch(std::bad_cast&)
                {
                    throw WrongParameterType(parameter(id), *this);
                }
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

    const Data& ConstImage::getParameter(unsigned int id)
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
        Data* outData = m_imageAccess();
        base::Image* outImage = dynamic_cast<base::Image*>(outData);
            
        adjustImage(m_image->width(), m_image->height(), m_image->pixelType(), outImage);
        
        cv::Mat inCvImage = getOpenCvMat(*m_image);
        cv::Mat outCvImage = getOpenCvMat(*outImage);
        
        inCvImage.copyTo(outCvImage);
        
        DataContainer outContainer = DataContainer(outImage);
        m_imageAccess = RecycleAccess(outContainer);
        
        Id2DataPair outputDataMapper(OUTPUT, outContainer);
        provider.sendOutputData( outputDataMapper);
    }
    
    const std::vector<const strom::Description*> ConstImage::setupInputs()
    {
        std::vector<const Description*> inputs;
        
        return inputs;
    }
    
    const std::vector<const Description*> ConstImage::setupOutputs()
    {
        std::vector<const Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataVariant::IMAGE);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector<const Parameter*> ConstImage::setupParameters()
    {
        std::vector<const strom::Parameter*> parameters;
        
        Parameter* image = new Parameter(IMAGE, DataVariant::IMAGE);
        image->setAccessMode(strom::Parameter::ACTIVATED_WRITE);
        parameters.push_back(image);
                                    
        return parameters;
    }
} 

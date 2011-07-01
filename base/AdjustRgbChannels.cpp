#include "AdjustRgbChannels.h"

#include "Config.h"
#include "Image.h"
#include "Utilities.h"

#include <stream/NumericParameter.h>
#include <stream/Primitive.h>
#include <stream/OperatorException.h>
#include <stream/DataContainer.h>
#include <stream/DataProvider.h>
#include <stream/Id2DataPair.h>
#include <stream/WriteAccess.h>

using namespace stream;

namespace base
{
    const std::string AdjustRgbChannels::NAME("Clip");
    
    const std::string AdjustRgbChannels::PACKAGE(PACKAGE_NAME);
    const Version AdjustRgbChannels::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    AdjustRgbChannels::AdjustRgbChannels()
      : Operator(NAME, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
        m_red(1.0),
        m_green(1.0),
        m_blue(1.0)
    {
    }

    void AdjustRgbChannels::setParameter(unsigned int id, const Data& value)
    {
        try
        {
            switch(id)
            {
            case RED:
                m_red = dynamic_cast<const Double&>(value);
                break;
            case GREEN:
                m_green = dynamic_cast<const Double&>(value);
                break;
            case BLUE:
                m_blue = dynamic_cast<const Double&>(value);
                break;
            default:
                throw ParameterIdException(id, *this);
            }
        }
        catch(std::bad_cast&)
        {
            throw ParameterTypeException(*parameters()[id], *this);
        }
    }

    const Data& AdjustRgbChannels::getParameter(unsigned int id)
    {
        switch(id)
        {
        case RED:
            return m_red;
        case GREEN:
            return m_green;
        case BLUE:
            return m_blue;
        default:
            throw ParameterIdException(id, *this);
        }
    }  
    
    void AdjustRgbChannels::execute(DataProvider& provider)
    {
        Id2DataPair inputDataMapper(INPUT);
        provider.receiveInputData(inputDataMapper);
        
        DataContainer container = inputDataMapper.data();
        WriteAccess access(container);
        Image* image = dynamic_cast<Image*>(access());

        cv::Mat cvImage = getOpenCvMat(*image);
        
        switch(image->pixelType())
        {
        case stream::Image::RGB_24:
        {
            typedef cv::Vec<unsigned char, 3> pixel_t;

            cv::MatIterator_<pixel_t> it = cvImage.begin<pixel_t>();
            cv::MatIterator_<pixel_t> it_end = cvImage.end<pixel_t>();
            for(; it != it_end; ++it)
            {
                (*it)[0] = cv::saturate_cast<unsigned char>((double((*it)[0]) * m_red));
                (*it)[1] = cv::saturate_cast<unsigned char>((double((*it)[1]) * m_green));
                (*it)[2] = cv::saturate_cast<unsigned char>((double((*it)[2]) * m_blue));
            }
            break;
        }
        case stream::Image::BGR_24:
        {
            typedef cv::Vec<unsigned char, 3> pixel_t;

            cv::MatIterator_<pixel_t> it = cvImage.begin<pixel_t>();
            cv::MatIterator_<pixel_t> it_end = cvImage.end<pixel_t>();
            for(; it != it_end; ++it)
            {
                (*it)[0] = cv::saturate_cast<unsigned char>((double((*it)[0]) * m_blue));
                (*it)[1] = cv::saturate_cast<unsigned char>((double((*it)[1]) * m_green));
                (*it)[2] = cv::saturate_cast<unsigned char>((double((*it)[2]) * m_red));
            }
            break;
        }
        default:
            throw InputTypeException(INPUT, *this);
        }
        
        Id2DataPair outputDataMapper(OUTPUT, container);
        provider.sendOutputData( outputDataMapper);
    }
    
    const std::vector< Description* > AdjustRgbChannels::setupInputs()
    {
        std::vector<Description*> inputs;
        
        Description* input = new Description(INPUT, DataType::RGB_IMAGE);
        input->setName("Input");
        inputs.push_back(input);
        
        return inputs;
    }
    
    const std::vector< stream::Description* > AdjustRgbChannels::setupOutputs()
    {
        std::vector<Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataType::RGB_IMAGE);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector< stream::Parameter* > AdjustRgbChannels::setupParameters()
    {
        std::vector<stream::Parameter*> parameters;
        
        NumericParameter<Double>* red = new NumericParameter<Double>(RED, DataType::DOUBLE, Double(0.0), Double::MAX);
        red->setName("Red");
        red->setInactiveAccessMode(stream::Parameter::READ_WRITE);
        red->setActiveAccessMode(stream::Parameter::READ_WRITE);
        parameters.push_back(red);
        
        NumericParameter<Double>* green = new NumericParameter<Double>(GREEN, DataType::DOUBLE, Double(0.0), Double::MAX);
        green->setName("Green");
        green->setInactiveAccessMode(stream::Parameter::READ_WRITE);
        green->setActiveAccessMode(stream::Parameter::READ_WRITE);
        parameters.push_back(green);
        
        NumericParameter<Double>* blue = new NumericParameter<Double>(BLUE, DataType::DOUBLE, Double(0.0), Double::MAX);
        blue->setName("Blue");
        blue->setInactiveAccessMode(stream::Parameter::READ_WRITE);
        blue->setActiveAccessMode(stream::Parameter::READ_WRITE);
        parameters.push_back(blue);
                                    
        return parameters;
    }
} 

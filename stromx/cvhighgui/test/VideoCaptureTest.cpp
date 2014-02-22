/* 
*  Copyright 2013 Thomas Fidler
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

#include "stromx/cvhighgui/test/VideoCaptureTest.h"

#include <stromx/runtime/OperatorTester.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/cvsupport/Image.h>
#include "stromx/cvhighgui//VideoCapture.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvhighgui::VideoCaptureTest);

namespace stromx
{
    namespace cvhighgui
    {
        void VideoCaptureTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new VideoCapture);
            try
            {
                m_operator->initialize();
            }
            catch(stromx::runtime::OperatorError&)
            {
                m_hasCamera = false;
            }
        }

        void VideoCaptureTest::testParameterFrameRate()
        {
            if(m_hasCamera)
            {
                if(checkParameter(VideoCapture::FRAMERATE))
                {
                    m_operator->setParameter(VideoCapture::FRAMERATE,runtime::Float64(0.1));
                    runtime::DataRef frameRate = m_operator->getParameter(VideoCapture::FRAMERATE);
                    runtime::Float64 doubleFrameRate = runtime::data_cast<runtime::Float64>(frameRate);
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(double(doubleFrameRate),double(runtime::Float64(0.1)),m_deltaAcceptance);
                    std::cout << "(Frame rate is configurable)";
                }
                else
                {
                    std::cout << "(Frame rate is not configurable)";
                }
            }
        }
        
        void VideoCaptureTest::testParameterBrightness()
        {
            if(m_hasCamera)
            {
                if(checkParameter(VideoCapture::BRIGHTNESS))
                {
                    m_operator->setParameter(VideoCapture::BRIGHTNESS,runtime::Float64(0.2));
                    runtime::DataRef brightness = m_operator->getParameter(VideoCapture::BRIGHTNESS);
                    runtime::Float64 doubleBrightness = runtime::data_cast<runtime::Float64>(brightness);
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(double(doubleBrightness),double(runtime::Float64(0.2)),m_deltaAcceptance);
                    std::cout << "(Brightness is configurable)";
                }
                else
                {
                    std::cout << "(Brightness is not configurable)";
                }
            }
        }
        
        void VideoCaptureTest::testParameterContrast()
        {
            if(m_hasCamera)
            {
                if(checkParameter(VideoCapture::CONTRAST))
                {
                    m_operator->setParameter(VideoCapture::CONTRAST,runtime::Float64(0.3));
                    runtime::DataRef contrast = m_operator->getParameter(VideoCapture::CONTRAST);
                    runtime::Float64 doubleContrast = runtime::data_cast<runtime::Float64>(contrast);
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(double(doubleContrast),double(runtime::Float64(0.3)),m_deltaAcceptance);
                    std::cout << "(Contrast is configurable)";
                }
                else
                {
                    std::cout << "(Contrast is not configurable)";
                }
            }
        }
        
        void VideoCaptureTest::testParameterSaturation()
        {
            if(m_hasCamera)
            {
                if(checkParameter(VideoCapture::SATURATION))
                {
                    m_operator->setParameter(VideoCapture::SATURATION,runtime::Float64(0.4));
                    runtime::DataRef saturation = m_operator->getParameter(VideoCapture::SATURATION);
                    runtime::Float64 doubleSaturation = runtime::data_cast<runtime::Float64>(saturation);
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(double(doubleSaturation),double(runtime::Float64(0.4)),m_deltaAcceptance);
                    std::cout << "(Saturation is configurable)";
                }
                else
                {
                    std::cout << "(Saturation is not configurable)";
                }
            }
        }
        
        void VideoCaptureTest::testParameterHue()
        {
            if(m_hasCamera)
            {
                if(checkParameter(VideoCapture::HUE))
                {
                    m_operator->setParameter(VideoCapture::HUE,runtime::Float64(0.5));
                    runtime::DataRef hue = m_operator->getParameter(VideoCapture::HUE);
                    runtime::Float64 doubleHue = runtime::data_cast<runtime::Float64>(hue);
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(double(doubleHue),double(runtime::Float64(0.5)),m_deltaAcceptance);
                    std::cout << "(Hue is configurable)";
                }
                else
                {
                    std::cout << "(Hue is not configurable)";
                }
            }
        }
        
        void VideoCaptureTest::testParameterGain()
        {
            if(m_hasCamera)
            {
                if(checkParameter(VideoCapture::GAIN))
                {
                    m_operator->setParameter(VideoCapture::GAIN,runtime::Float64(0.6));
                    runtime::DataRef gain = m_operator->getParameter(VideoCapture::GAIN);
                    runtime::Float64 doubleGain = runtime::data_cast<runtime::Float64>(gain);
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(double(doubleGain),double(runtime::Float64(0.6)),m_deltaAcceptance);
                    std::cout << "(Gain is configurable)";
                }
                else
                {
                    std::cout << "(Gain is not configurable)";
                }
            }
        }
        
        void VideoCaptureTest::testParameterExposure()
        {
            if(m_hasCamera)
            {
                if(checkParameter(VideoCapture::EXPOSURE))
                {
                    m_operator->setParameter(VideoCapture::EXPOSURE,runtime::Float64(0.7));
                    runtime::DataRef exposure = m_operator->getParameter(VideoCapture::EXPOSURE);
                    runtime::Float64 doubleExposure = runtime::data_cast<runtime::Float64>(exposure);
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(double(doubleExposure),double(runtime::Float64(0.7)),m_deltaAcceptance);
                    std::cout << "(Exposure is configurable)";
                }
                else
                {
                    std::cout << "(Exposure is not configurable)";
                }
            }
        }
        
        void VideoCaptureTest::testExecute()
        {
            if(m_hasCamera)
            {
                m_operator->activate();
                runtime::DataContainer output = m_operator->getOutputData(VideoCapture::OUTPUT);
            
                runtime::ReadAccess<runtime::Image> access(output);
                const runtime::Image& image = access();
            
                cvsupport::Image::save("VideoCaptureTest_testExecute.png", image);
                m_operator->deactivate();
            }
        }

        void VideoCaptureTest::tearDown()
        {
            delete m_operator;
        }
        
        bool VideoCaptureTest::checkParameter(const unsigned int id)
        {
            try
            {
                m_operator->info().parameter(id);
                return true;
            }
            catch(runtime::WrongId&)
            {
                return false;
            }
        }

    }
}

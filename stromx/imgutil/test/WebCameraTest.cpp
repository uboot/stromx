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

#include <stromx/runtime/OperatorTester.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/imgutil/Image.h"
#include "stromx/imgutil/WebCamera.h"
#include "stromx/imgutil/test/WebCameraTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::imgutil::WebCameraTest);

namespace stromx
{
    namespace imgutil
    {
        void WebCameraTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new WebCamera);
            try
            {
                m_operator->initialize();
            }
            catch(stromx::runtime::OperatorError&)
            {
                m_hasCamera = false;
            }
        }

        void WebCameraTest::testParameterFrameRate()
        {
            if(m_hasCamera)
            {
                if(checkParameter(WebCamera::FRAMERATE))
                {
                    m_operator->setParameter(WebCamera::FRAMERATE,runtime::Double(0.1));
                    runtime::DataRef frameRate = m_operator->getParameter(WebCamera::FRAMERATE);
                    runtime::Double doubleFrameRate = runtime::data_cast<runtime::Double>(frameRate);
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(double(doubleFrameRate),double(runtime::Double(0.1)),m_deltaAcceptance);
                    std::cout << "(Frame rate is configurable)";
                }
                else
                {
                    std::cout << "(Frame rate is not configurable)";
                }
            }
        }
        
        void WebCameraTest::testParameterBrightness()
        {
            if(m_hasCamera)
            {
                if(checkParameter(WebCamera::BRIGHTNESS))
                {
                    m_operator->setParameter(WebCamera::BRIGHTNESS,runtime::Double(0.2));
                    runtime::DataRef brightness = m_operator->getParameter(WebCamera::BRIGHTNESS);
                    runtime::Double doubleBrightness = runtime::data_cast<runtime::Double>(brightness);
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(double(doubleBrightness),double(runtime::Double(0.2)),m_deltaAcceptance);
                    std::cout << "(Brightness is configurable)";
                }
                else
                {
                    std::cout << "(Brightness is not configurable)";
                }
            }
        }
        
        void WebCameraTest::testParameterContrast()
        {
            if(m_hasCamera)
            {
                if(checkParameter(WebCamera::CONTRAST))
                {
                    m_operator->setParameter(WebCamera::CONTRAST,runtime::Double(0.3));
                    runtime::DataRef contrast = m_operator->getParameter(WebCamera::CONTRAST);
                    runtime::Double doubleContrast = runtime::data_cast<runtime::Double>(contrast);
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(double(doubleContrast),double(runtime::Double(0.3)),m_deltaAcceptance);
                    std::cout << "(Contrast is configurable)";
                }
                else
                {
                    std::cout << "(Contrast is not configurable)";
                }
            }
        }
        
        void WebCameraTest::testParameterSaturation()
        {
            if(m_hasCamera)
            {
                if(checkParameter(WebCamera::SATURATION))
                {
                    m_operator->setParameter(WebCamera::SATURATION,runtime::Double(0.4));
                    runtime::DataRef saturation = m_operator->getParameter(WebCamera::SATURATION);
                    runtime::Double doubleSaturation = runtime::data_cast<runtime::Double>(saturation);
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(double(doubleSaturation),double(runtime::Double(0.4)),m_deltaAcceptance);
                    std::cout << "(Saturation is configurable)";
                }
                else
                {
                    std::cout << "(Saturation is not configurable)";
                }
            }
        }
        
        void WebCameraTest::testParameterHue()
        {
            if(m_hasCamera)
            {
                if(checkParameter(WebCamera::HUE))
                {
                    m_operator->setParameter(WebCamera::HUE,runtime::Double(0.5));
                    runtime::DataRef hue = m_operator->getParameter(WebCamera::HUE);
                    runtime::Double doubleHue = runtime::data_cast<runtime::Double>(hue);
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(double(doubleHue),double(runtime::Double(0.5)),m_deltaAcceptance);
                    std::cout << "(Hue is configurable)";
                }
                else
                {
                    std::cout << "(Hue is not configurable)";
                }
            }
        }
        
        void WebCameraTest::testParameterGain()
        {
            if(m_hasCamera)
            {
                if(checkParameter(WebCamera::GAIN))
                {
                    m_operator->setParameter(WebCamera::GAIN,runtime::Double(0.6));
                    runtime::DataRef gain = m_operator->getParameter(WebCamera::GAIN);
                    runtime::Double doubleGain = runtime::data_cast<runtime::Double>(gain);
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(double(doubleGain),double(runtime::Double(0.6)),m_deltaAcceptance);
                    std::cout << "(Gain is configurable)";
                }
                else
                {
                    std::cout << "(Gain is not configurable)";
                }
            }
        }
        
        void WebCameraTest::testParameterExposure()
        {
            if(m_hasCamera)
            {
                if(checkParameter(WebCamera::EXPOSURE))
                {
                    m_operator->setParameter(WebCamera::EXPOSURE,runtime::Double(0.7));
                    runtime::DataRef exposure = m_operator->getParameter(WebCamera::EXPOSURE);
                    runtime::Double doubleExposure = runtime::data_cast<runtime::Double>(exposure);
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(double(doubleExposure),double(runtime::Double(0.7)),m_deltaAcceptance);
                    std::cout << "(Exposure is configurable)";
                }
                else
                {
                    std::cout << "(Exposure is not configurable)";
                }
            }
        }
        
        void WebCameraTest::testExecute()
        {
            if(m_hasCamera)
            {
                m_operator->activate();
                runtime::DataContainer output = m_operator->getOutputData(WebCamera::OUTPUT);
            
                runtime::ReadAccess<runtime::Image> access(output);
                const runtime::Image& image = access();
            
                imgutil::Image::save("WebCameraTest_testExecute.png", image);
                m_operator->deactivate();
            }
        }

        void WebCameraTest::tearDown()
        {
            delete m_operator;
        }
        
        bool WebCameraTest::checkParameter(const unsigned int id)
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

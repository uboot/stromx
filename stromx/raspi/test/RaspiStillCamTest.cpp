/* 
*  Copyright 2014 Matthias Fuchs
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

#include "stromx/raspi/test/RaspiStillCamTest.h"

#include <interface/mmal/util/mmal_util_params.h>
#include <boost/timer/timer.hpp>
#include <cppunit/TestAssert.h>
#include <stromx/cvsupport/Image.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/runtime/TriggerData.h>
#include "stromx/raspi/RaspiStillCam.h"


CPPUNIT_TEST_SUITE_REGISTRATION (stromx::raspi::RaspiStillCamTest);

namespace stromx
{
    using namespace runtime;

    namespace raspi
    {
        void RaspiStillCamTest::setUp()
        {
            m_operator = new OperatorTester(new RaspiStillCam());
        }

        void RaspiStillCamTest::testInitialize()
        {
            CPPUNIT_ASSERT_NO_THROW(m_operator->initialize());
        }

        void RaspiStillCamTest::testInitializeTwice()
        {
            m_operator->initialize();
            m_operator->deinitialize();
            CPPUNIT_ASSERT_NO_THROW(m_operator->initialize());
        }

        void RaspiStillCamTest::testDeinitialize()
        {
            CPPUNIT_ASSERT_NO_THROW(m_operator->deinitialize());
        }

        void RaspiStillCamTest::testActivate()
        {
            m_operator->initialize();
            
            CPPUNIT_ASSERT_NO_THROW(m_operator->activate());
        }

        void RaspiStillCamTest::testDeactivate()
        {
            m_operator->initialize();
            m_operator->activate();
            
            CPPUNIT_ASSERT_NO_THROW(m_operator->deactivate());
        }

        void RaspiStillCamTest::testExecute()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer result = m_operator->getOutputData(RaspiStillCam::IMAGE);
            
            runtime::ReadAccess access(result);
            cvsupport::Image::save("RaspiStillCamTest_testExecute.png", access.get<runtime::Image>());
        }

        void RaspiStillCamTest::testSetShutterSpeedAndExecute()
        {
            m_operator->initialize();
            m_operator->setParameter(RaspiStillCam::SHUTTER_SPEED, UInt32(200000));
            m_operator->activate();
            
            runtime::DataContainer result = m_operator->getOutputData(RaspiStillCam::IMAGE);
            
            runtime::ReadAccess access(result);
            cvsupport::Image::save("RaspiStillCamTest_testSetShutterSpeedAndExecute.png", access.get<runtime::Image>());
        }

        void RaspiStillCamTest::testExecuteWithTrigger()
        {
            m_operator->setParameter(RaspiStillCam::HAS_TRIGGER_INPUT, runtime::Bool(true));
            m_operator->initialize();
            m_operator->activate();
            m_operator->setInputData(RaspiStillCam::TRIGGER, runtime::DataContainer(new runtime::TriggerData()));
            
            runtime::DataContainer result = m_operator->getOutputData(RaspiStillCam::IMAGE);
            
            runtime::ReadAccess access(result);
            cvsupport::Image::save("RaspiStillCamTest_testExecute.png", access.get<runtime::Image>());
        }

        void RaspiStillCamTest::testExecuteTwice()
        {
            m_operator->initialize();
            m_operator->activate();
            
            {
                runtime::DataContainer result = m_operator->getOutputData(RaspiStillCam::IMAGE);
                runtime::ReadAccess access0(result);
                cvsupport::Image::save("RaspiStillCamTest_testExecuteTwice_0.png", access0.get<runtime::Image>());
            }
            
            m_operator->clearOutputData(RaspiStillCam::IMAGE);
            
            {
                runtime::DataContainer result = m_operator->getOutputData(RaspiStillCam::IMAGE);
                runtime::ReadAccess access1(result);
                cvsupport::Image::save("RaspiStillCamTest_testExecuteTwice_1.png", access1.get<runtime::Image>());
            }
        }

        void RaspiStillCamTest::testExecuteTwoBuffers()
        {
            m_operator->initialize();
            m_operator->setParameter(RaspiStillCam::NUM_BUFFERS, runtime::UInt32(2));
            m_operator->activate();
            
            runtime::DataContainer result;
            result = m_operator->getOutputData(RaspiStillCam::IMAGE);
            runtime::ReadAccess access0(result);
            cvsupport::Image::save("RaspiStillCamTest_testExecuteTwoBuffers_0.png", access0.get<runtime::Image>());
            
            m_operator->clearOutputData(RaspiStillCam::IMAGE);
            
            result = m_operator->getOutputData(RaspiStillCam::IMAGE);
            runtime::ReadAccess access1(result);
            cvsupport::Image::save("RaspiStillCamTest_testExecuteTwoBuffers_1.png", access1.get<runtime::Image>());
        }

        void RaspiStillCamTest::testGetShutterSpeed()
        {
            m_operator->initialize();
            m_operator->activate();
            
            DataRef value = m_operator->getParameter(RaspiStillCam::SHUTTER_SPEED);
             
            CPPUNIT_ASSERT_DOUBLES_EQUAL(UInt32(100000), data_cast<UInt32>(value), 100);
        }

        void RaspiStillCamTest::testSetShutterSpeed()
        {
            m_operator->initialize();
            m_operator->activate();
            
            m_operator->setParameter(RaspiStillCam::SHUTTER_SPEED, UInt32(200000));
            
            DataRef value = m_operator->getParameter(RaspiStillCam::SHUTTER_SPEED);          
            CPPUNIT_ASSERT_DOUBLES_EQUAL(UInt32(200000), data_cast<UInt32>(value), 100);
        }

        void RaspiStillCamTest::testGetAwbMode()
        {
            m_operator->initialize();
            m_operator->activate();
            
            DataRef value = m_operator->getParameter(RaspiStillCam::AWB_MODE);
            
            CPPUNIT_ASSERT_EQUAL(Enum(MMAL_PARAM_AWBMODE_OFF), data_cast<Enum>(value));
        }

        void RaspiStillCamTest::testSetAwbMode()
        {
            m_operator->initialize();
            m_operator->activate();
            
            m_operator->setParameter(RaspiStillCam::AWB_MODE, Enum(MMAL_PARAM_AWBMODE_CLOUDY));
            
            DataRef value = m_operator->getParameter(RaspiStillCam::AWB_MODE);      
            CPPUNIT_ASSERT_EQUAL(Enum(MMAL_PARAM_AWBMODE_CLOUDY), data_cast<Enum>(value));
        }

        void RaspiStillCamTest::testFramesPerSecond640()
        {
            m_operator->initialize();
            m_operator->setParameter(RaspiStillCam::SHUTTER_SPEED, UInt32(100));
            m_operator->setParameter(RaspiStillCam::RESOLUTION,
                                     runtime::Enum(RaspiStillCam::RESOLUTION_640_BY_480));
            m_operator->activate();
            
            boost::timer::auto_cpu_timer t;
            for (size_t i = 0; i < 100; ++i)
            {
                runtime::DataContainer result = m_operator->getOutputData(RaspiStillCam::IMAGE);
                m_operator->clearOutputData(RaspiStillCam::IMAGE);
            }
        }

        void RaspiStillCamTest::testFramesPerSecond1280()
        {
            m_operator->initialize();
            m_operator->setParameter(RaspiStillCam::SHUTTER_SPEED, UInt32(100));
            m_operator->setParameter(RaspiStillCam::RESOLUTION,
                                     runtime::Enum(RaspiStillCam::RESOLUTION_1280_BY_960));
            m_operator->activate();
            
            boost::timer::auto_cpu_timer t;
            for (size_t i = 0; i < 100; ++i)
            {
                runtime::DataContainer result = m_operator->getOutputData(RaspiStillCam::IMAGE);
                m_operator->clearOutputData(RaspiStillCam::IMAGE);
            }
        }

        void RaspiStillCamTest::testFramesPerSecond2560()
        {
            m_operator->initialize();
            m_operator->setParameter(RaspiStillCam::SHUTTER_SPEED, UInt32(100));
            m_operator->setParameter(RaspiStillCam::RESOLUTION,
                                     runtime::Enum(RaspiStillCam::RESOLUTION_2560_BY_1920));
            m_operator->activate();
            
            boost::timer::auto_cpu_timer t;
            for (size_t i = 0; i < 100; ++i)
            {
                runtime::DataContainer result = m_operator->getOutputData(RaspiStillCam::IMAGE);
                m_operator->clearOutputData(RaspiStillCam::IMAGE);
            }
        }

        void RaspiStillCamTest::testFramesPerSecondRoi()
        {
            m_operator->initialize();
            m_operator->setParameter(RaspiStillCam::SHUTTER_SPEED, UInt32(100));
            m_operator->setParameter(RaspiStillCam::RESOLUTION,
                                     runtime::Enum(RaspiStillCam::RESOLUTION_2560_BY_1920));
            m_operator->setParameter(RaspiStillCam::WIDTH, Float32(0.5));  
            m_operator->setParameter(RaspiStillCam::HEIGHT, Float32(0.5)); 
            
            m_operator->activate();
            
            boost::timer::auto_cpu_timer t;
            for (size_t i = 0; i < 100; ++i)
            {
                runtime::DataContainer result = m_operator->getOutputData(RaspiStillCam::IMAGE);
                m_operator->clearOutputData(RaspiStillCam::IMAGE);
            }
        }
        
        void RaspiStillCamTest::testSetResolution()
        {
            m_operator->initialize();
            m_operator->setParameter(RaspiStillCam::RESOLUTION,
                                     runtime::Enum(RaspiStillCam::RESOLUTION_2560_BY_1920));
            m_operator->activate();
            
            DataRef value = m_operator->getParameter(RaspiStillCam::RESOLUTION);          
            CPPUNIT_ASSERT_EQUAL(Enum(RaspiStillCam::RESOLUTION_2560_BY_1920), data_cast<Enum>(value));
        }

        void RaspiStillCamTest::testGetRoi()
        {
            m_operator->initialize();
            
            DataRef value;
            value = m_operator->getParameter(RaspiStillCam::LEFT);          
            CPPUNIT_ASSERT_DOUBLES_EQUAL(UInt32(0), data_cast<Float32>(value), 1e-3);
            value = m_operator->getParameter(RaspiStillCam::TOP);          
            CPPUNIT_ASSERT_DOUBLES_EQUAL(Float32(0.0), data_cast<Float32>(value), 1e-3);
            value = m_operator->getParameter(RaspiStillCam::WIDTH);          
            CPPUNIT_ASSERT_DOUBLES_EQUAL(Float32(1.0), data_cast<Float32>(value), 1e-3);
            value = m_operator->getParameter(RaspiStillCam::HEIGHT);          
            CPPUNIT_ASSERT_DOUBLES_EQUAL(Float32(1.0), data_cast<Float32>(value), 1e-3);
        }

        void RaspiStillCamTest::testSetRoi()
        {
            m_operator->initialize();
            
            m_operator->setParameter(RaspiStillCam::LEFT, Float32(0.25));  
            m_operator->setParameter(RaspiStillCam::TOP, Float32(0.3));   
            m_operator->setParameter(RaspiStillCam::WIDTH, Float32(0.5));  
            m_operator->setParameter(RaspiStillCam::HEIGHT, Float32(0.4));  
            
            DataRef value;
            value = m_operator->getParameter(RaspiStillCam::LEFT);         
            CPPUNIT_ASSERT_DOUBLES_EQUAL(Float32(0.25), data_cast<Float32>(value), 1e-3);
            value = m_operator->getParameter(RaspiStillCam::TOP);           
            CPPUNIT_ASSERT_DOUBLES_EQUAL(Float32(0.3), data_cast<Float32>(value), 1e-3);    
            value = m_operator->getParameter(RaspiStillCam::WIDTH);   
            CPPUNIT_ASSERT_DOUBLES_EQUAL(Float32(0.5), data_cast<Float32>(value), 1e-3);
            value = m_operator->getParameter(RaspiStillCam::HEIGHT);         
            CPPUNIT_ASSERT_DOUBLES_EQUAL(Float32(0.4), data_cast<Float32>(value), 1e-3);
        }

        void RaspiStillCamTest::testSetRoiAndExecute()
        {
            m_operator->initialize();             
            m_operator->activate();
            
            m_operator->setParameter(RaspiStillCam::LEFT, Float32(0.25));  
            m_operator->setParameter(RaspiStillCam::TOP, Float32(0.3));   
            m_operator->setParameter(RaspiStillCam::WIDTH, Float32(0.5));  
            m_operator->setParameter(RaspiStillCam::HEIGHT, Float32(0.4)); 
            
            runtime::DataContainer result = m_operator->getOutputData(RaspiStillCam::IMAGE);
            
            runtime::ReadAccess access(result);
            cvsupport::Image::save("RaspiStillCamTest_testSetRoiAndExecute.png", access.get<runtime::Image>());
        }
        
        void RaspiStillCamTest::testSetAwbModeOffAndExecute()
        {
            m_operator->initialize();
            m_operator->setParameter(RaspiStillCam::AWB_MODE, Enum(MMAL_PARAM_AWBMODE_OFF));
            m_operator->activate();
            
            for (int i = 0; i < 2; ++i)
                m_operator->getOutputData(RaspiStillCam::IMAGE);
            
            runtime::DataContainer result = m_operator->getOutputData(RaspiStillCam::IMAGE);
            runtime::ReadAccess access0(result);
            cvsupport::Image::save("RaspiStillCamTest_testSetAwbModeOffAndExecute.png", access0.get<runtime::Image>());
        }
        
        void RaspiStillCamTest::testSetAwbModeSunlightAndExecute()
        {
            m_operator->initialize();
            m_operator->setParameter(RaspiStillCam::AWB_MODE, Enum(MMAL_PARAM_AWBMODE_SUNLIGHT));
            m_operator->activate();
            
            for (int i = 0; i < 2; ++i)
                m_operator->getOutputData(RaspiStillCam::IMAGE);
            
            runtime::DataContainer result = m_operator->getOutputData(RaspiStillCam::IMAGE);
            runtime::ReadAccess access0(result);
            cvsupport::Image::save("RaspiStillCamTest_testSetAwbModeSunlightAndExecute.png", access0.get<runtime::Image>());
        }

        void RaspiStillCamTest::testGetAwbGains()
        {
            m_operator->initialize();
            m_operator->activate();
            DataRef red = m_operator->getParameter(RaspiStillCam::AWB_GAIN_RED);
            DataRef blue = m_operator->getParameter(RaspiStillCam::AWB_GAIN_BLUE);
            
            CPPUNIT_ASSERT_DOUBLES_EQUAL(Float32(1.0), data_cast<Float32>(red), 1e-3);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(Float32(1.0), data_cast<Float32>(blue), 1e-3);
        }

        void RaspiStillCamTest::testSetAwbGains()
        {
            m_operator->initialize();
            m_operator->activate();
            
            m_operator->setParameter(RaspiStillCam::AWB_GAIN_RED, Float32(2.0));
            m_operator->setParameter(RaspiStillCam::AWB_GAIN_BLUE, Float32(3.0));
            
            DataRef red = m_operator->getParameter(RaspiStillCam::AWB_GAIN_RED);
            DataRef blue = m_operator->getParameter(RaspiStillCam::AWB_GAIN_BLUE);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(Float32(2.0), data_cast<Float32>(red), 1e-3);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(Float32(3.0), data_cast<Float32>(blue), 1e-3);
        }
        
        void RaspiStillCamTest::tearDown()
        {
            delete m_operator;
        }
    }
}

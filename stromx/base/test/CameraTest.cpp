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

#include <boost/thread.hpp>
#include <cppunit/TestAssert.h>
#include "CameraTest.h"
#include "../Camera.h"
#include "../Image.h"
#include <stromx/core/DataContainer.h>
#include <stromx/core/Enum.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/OperatorTester.h>
#include <stromx/core/ReadAccess.h>
#include <stromx/core/TriggerData.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::CameraTest);

namespace stromx
{
    using namespace core;

    namespace base
    {
        void CameraTest::setUp ( void )
        {
            Image image("lenna.jpg");
            
            m_operator = new core::OperatorTester(new Camera());
            m_operator->setParameter(Camera::OUTPUT_INDEX, Bool(true));
            m_operator->initialize();
            m_operator->setParameter(Camera::NUM_BUFFERS, UInt32(1));
            m_operator->setParameter(Camera::BUFFER_SIZE, UInt32(image.bufferSize()));
            m_operator->setParameter(Camera::IMAGE, image);
            
            // default to immediate, automatic trigger
            m_operator->setParameter(Camera::TRIGGER_MODE, Enum(Camera::INTERNAL));
            m_operator->setParameter(Camera::FRAME_PERIOD, UInt32(0));
        }
        
        void CameraTest::testExecuteSoftwareTrigger()
        {
            m_operator->setParameter(Camera::TRIGGER_MODE, Enum(Camera::SOFTWARE));
            m_operator->activate();
            
            {
                boost::this_thread::sleep(boost::posix_time::seconds(1));
                m_operator->setParameter(Camera::TRIGGER, core::TriggerData());
                DataContainer imageContainer = m_operator->getOutputData(Camera::OUTPUT);
                DataContainer indexContainer = m_operator->getOutputData(Camera::INDEX);
                UInt32 index = ReadAccess<UInt32>(indexContainer)();
                CPPUNIT_ASSERT_EQUAL(UInt32(0), index);
                
                const Image & image = ReadAccess<Image>(imageContainer)();
                image.save("CameraTest_testExecuteSoftwareTrigger.png");
            }
            
            m_operator->clearOutputData(Camera::OUTPUT);
            m_operator->clearOutputData(Camera::INDEX);
            
            {
                boost::this_thread::sleep(boost::posix_time::seconds(1));
                m_operator->setParameter(Camera::TRIGGER, core::TriggerData());
                DataContainer imageContainer = m_operator->getOutputData(Camera::OUTPUT);
                DataContainer indexContainer = m_operator->getOutputData(Camera::INDEX);
                UInt32 index = ReadAccess<UInt32>(indexContainer)();
                CPPUNIT_ASSERT_EQUAL(UInt32(1), index);
            }
            
        }
        
        void CameraTest::testExecuteInternalTrigger()
        {
            m_operator->setParameter(Camera::TRIGGER_MODE, Enum(Camera::INTERNAL));
            m_operator->setParameter(Camera::FRAME_PERIOD, UInt32(1000));
            m_operator->activate();
            
            {
                DataContainer imageContainer = m_operator->getOutputData(Camera::OUTPUT);
                DataContainer indexContainer = m_operator->getOutputData(Camera::INDEX);
                UInt32 index = ReadAccess<UInt32>(indexContainer)();
                CPPUNIT_ASSERT_EQUAL(UInt32(0), index);
            }
            
            m_operator->clearOutputData(Camera::OUTPUT);
            m_operator->clearOutputData(Camera::INDEX);
            
            {
                DataContainer imageContainer = m_operator->getOutputData(Camera::OUTPUT);
                DataContainer indexContainer = m_operator->getOutputData(Camera::INDEX);
                UInt32 index = ReadAccess<UInt32>(indexContainer)();
                CPPUNIT_ASSERT_EQUAL(UInt32(1), index);
            }
        }
        
        void CameraTest::testAdjustPixelType()
        {
            m_operator->setParameter(Camera::PIXEL_TYPE, Enum(core::Image::BAYERBG_8));
            m_operator->activate();
            DataContainer imageContainer = m_operator->getOutputData(Camera::OUTPUT);
            const Image & image = ReadAccess<Image>(imageContainer)();
            
            CPPUNIT_ASSERT_EQUAL(image.pixelType(), core::Image::BAYERBG_8);
            image.save("CameraTest_testAdjustPixelTypeBayerBg8.png");
        }

        void CameraTest::testAdjustRoi()
        {
            m_operator->setParameter(Camera::WIDTH, UInt32(319));
            m_operator->setParameter(Camera::LEFT, UInt32(51));
            m_operator->setParameter(Camera::HEIGHT, UInt32(217));
            m_operator->setParameter(Camera::TOP, UInt32(46));
            m_operator->activate();
            DataContainer imageContainer = m_operator->getOutputData(Camera::OUTPUT);
            const Image & image = ReadAccess<Image>(imageContainer)();
            
            CPPUNIT_ASSERT_EQUAL(image.width(), (unsigned int)(319));
            CPPUNIT_ASSERT_EQUAL(image.height(), (unsigned int)(217));
            image.save("CameraTest_testAdjustRoi.png");
        }
        
        void CameraTest::testAdjustExposure()
        {
            m_operator->setParameter(Camera::EXPOSURE, UInt32(5));
            m_operator->setParameter(Camera::PIXEL_TYPE, Enum(core::Image::RGB_24));
            m_operator->activate();
            DataContainer imageContainer = m_operator->getOutputData(Camera::OUTPUT);
            const Image & image = ReadAccess<Image>(imageContainer)();
            
            image.save("CameraTest_testAdjustExposure.png");
        }

        void CameraTest::testAdjustWhiteBalance()
        {
            m_operator->setParameter(Camera::WHITE_BALANCE_RED, Double(2.0));
            m_operator->setParameter(Camera::WHITE_BALANCE_GREEN, Double(1.0));
            m_operator->setParameter(Camera::WHITE_BALANCE_BLUE, Double(0.0));
            m_operator->setParameter(Camera::PIXEL_TYPE, Enum(core::Image::RGB_24));
            m_operator->activate();
            DataContainer imageContainer = m_operator->getOutputData(Camera::OUTPUT);
            const Image & image = ReadAccess<Image>(imageContainer)();
            
            image.save("CameraTest_testAdjustWhiteBalance.png");
        }
        
        void CameraTest::testValidateBufferSize()
        {
            UInt32 bufferSize = data_cast<const UInt32 &>(m_operator->getParameter(Camera::BUFFER_SIZE));
            UInt32 singleChannelSize((unsigned int)(bufferSize) / 3);
            
            m_operator->setParameter(Camera::PIXEL_TYPE, Enum(core::Image::RGB_24));
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(Camera::BUFFER_SIZE, singleChannelSize),
                                 WrongParameterValue);
            
            m_operator->setParameter(Camera::PIXEL_TYPE, Enum(core::Image::MONO_8));
            CPPUNIT_ASSERT_NO_THROW(m_operator->setParameter(Camera::BUFFER_SIZE, singleChannelSize));
            
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(Camera::PIXEL_TYPE, Enum(core::Image::RGB_24)),
                                 WrongParameterValue);
        }

        void CameraTest::tearDown ( void )
        {
            sleep(1);
            m_operator->deactivate();
            delete m_operator;
        }
    }
}

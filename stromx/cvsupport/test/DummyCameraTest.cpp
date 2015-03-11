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
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/Enum.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/OperatorTester.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/runtime/TriggerData.h>
#include "stromx/cvsupport/DummyCamera.h"
#include "stromx/cvsupport/Image.h"
#include "stromx/cvsupport/test/DummyCameraTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvsupport::DummyCameraTest);

namespace stromx
{
    using namespace runtime;

    namespace cvsupport
    {
        void DummyCameraTest::setUp ( void )
        {
            Image image("lenna.jpg");
            
            m_operator = new runtime::OperatorTester(new DummyCamera());
            m_operator->setParameter(DummyCamera::OUTPUT_INDEX, Bool(true));
            m_operator->initialize();
            m_operator->setParameter(DummyCamera::NUM_BUFFERS, UInt32(1));
            m_operator->setParameter(DummyCamera::BUFFER_SIZE, UInt32(image.bufferSize()));
            m_operator->setParameter(DummyCamera::IMAGE, image);
            
            // default to immediate, automatic trigger
            m_operator->setParameter(DummyCamera::TRIGGER_MODE, Enum(DummyCamera::INTERNAL));
            m_operator->setParameter(DummyCamera::FRAME_PERIOD, UInt32(100));
        }
        
        void DummyCameraTest::testExecuteSoftwareTrigger()
        {
            m_operator->setParameter(DummyCamera::TRIGGER_MODE, Enum(DummyCamera::SOFTWARE));
            m_operator->activate();
            
            {
                boost::this_thread::sleep_for(boost::chrono::seconds(1));
                m_operator->setParameter(DummyCamera::TRIGGER, runtime::TriggerData());
                DataContainer imageContainer = m_operator->getOutputData(DummyCamera::OUTPUT);
                DataContainer indexContainer = m_operator->getOutputData(DummyCamera::INDEX);
                UInt32 index = ReadAccess<UInt32>(indexContainer)();
                CPPUNIT_ASSERT_EQUAL(UInt32(0), index);
                
                const runtime::Image & image = ReadAccess<runtime::Image>(imageContainer)();
                cvsupport::Image::save("DummyCameraTest_testExecuteSoftwareTrigger.png", image);
            }
            
            m_operator->clearOutputData(DummyCamera::OUTPUT);
            m_operator->clearOutputData(DummyCamera::INDEX);
            
            {
                boost::this_thread::sleep_for(boost::chrono::seconds(1));
                m_operator->setParameter(DummyCamera::TRIGGER, runtime::TriggerData());
                DataContainer imageContainer = m_operator->getOutputData(DummyCamera::OUTPUT);
                DataContainer indexContainer = m_operator->getOutputData(DummyCamera::INDEX);
                UInt32 index = ReadAccess<UInt32>(indexContainer)();
                CPPUNIT_ASSERT_EQUAL(UInt32(1), index);
            }
            
        }
        
        void DummyCameraTest::testExecuteInternalTrigger()
        {
            m_operator->setParameter(DummyCamera::TRIGGER_MODE, Enum(DummyCamera::INTERNAL));
            m_operator->setParameter(DummyCamera::FRAME_PERIOD, UInt32(1000));
            m_operator->activate();
            
            {
                DataContainer imageContainer = m_operator->getOutputData(DummyCamera::OUTPUT);
                DataContainer indexContainer = m_operator->getOutputData(DummyCamera::INDEX);
                UInt32 index = ReadAccess<UInt32>(indexContainer)();
                CPPUNIT_ASSERT_EQUAL(UInt32(0), index);
            }
            
            m_operator->clearOutputData(DummyCamera::OUTPUT);
            m_operator->clearOutputData(DummyCamera::INDEX);
            
            {
                DataContainer imageContainer = m_operator->getOutputData(DummyCamera::OUTPUT);
                DataContainer indexContainer = m_operator->getOutputData(DummyCamera::INDEX);
                UInt32 index = ReadAccess<UInt32>(indexContainer)();
                CPPUNIT_ASSERT_EQUAL(UInt32(1), index);
            }
        }
        
        void DummyCameraTest::testAdjustPixelType()
        {
            m_operator->setParameter(DummyCamera::PIXEL_TYPE, Enum(runtime::Image::BAYERBG_8));
            m_operator->activate();
            DataContainer imageContainer = m_operator->getOutputData(DummyCamera::OUTPUT);
            const runtime::Image & image = ReadAccess<runtime::Image>(imageContainer)();
            
            CPPUNIT_ASSERT_EQUAL(image.pixelType(), runtime::Image::BAYERBG_8);
            cvsupport::Image::save("DummyCameraTest_testAdjustPixelTypeBayerBg8.png", image);
        }

        void DummyCameraTest::testAdjustRoi()
        {
            m_operator->setParameter(DummyCamera::WIDTH, UInt32(319));
            m_operator->setParameter(DummyCamera::LEFT, UInt32(51));
            m_operator->setParameter(DummyCamera::HEIGHT, UInt32(217));
            m_operator->setParameter(DummyCamera::TOP, UInt32(46));
            m_operator->activate();
            DataContainer imageContainer = m_operator->getOutputData(DummyCamera::OUTPUT);
            const runtime::Image & image = ReadAccess<runtime::Image>(imageContainer)();
            
            CPPUNIT_ASSERT_EQUAL(image.width(), (unsigned int)(319));
            CPPUNIT_ASSERT_EQUAL(image.height(), (unsigned int)(217));
            cvsupport::Image::save("DummyCameraTest_testAdjustRoi.png", image);
        }
        
        void DummyCameraTest::testAdjustExposure()
        {
            m_operator->setParameter(DummyCamera::EXPOSURE, UInt32(5));
            m_operator->setParameter(DummyCamera::PIXEL_TYPE, Enum(runtime::Image::RGB_24));
            m_operator->activate();
            DataContainer imageContainer = m_operator->getOutputData(DummyCamera::OUTPUT);
            const runtime::Image & image = ReadAccess<runtime::Image>(imageContainer)();
            
            cvsupport::Image::save("DummyCameraTest_testAdjustExposure.png", image);
        }

        void DummyCameraTest::testAdjustWhiteBalance()
        {
            m_operator->setParameter(DummyCamera::WHITE_BALANCE_RED, Float64(2.0));
            m_operator->setParameter(DummyCamera::WHITE_BALANCE_GREEN, Float64(1.0));
            m_operator->setParameter(DummyCamera::WHITE_BALANCE_BLUE, Float64(0.0));
            m_operator->setParameter(DummyCamera::PIXEL_TYPE, Enum(runtime::Image::RGB_24));
            m_operator->activate();
            DataContainer imageContainer = m_operator->getOutputData(DummyCamera::OUTPUT);
            const runtime::Image & image = ReadAccess<runtime::Image>(imageContainer)();
            
            cvsupport::Image::save("DummyCameraTest_testAdjustWhiteBalance.png", image);
        }

        void DummyCameraTest::testFlicker()
        {
            m_operator->setParameter(DummyCamera::FLICKER_AMOUNT, Float64(0.5));
            m_operator->setParameter(DummyCamera::PIXEL_TYPE, Enum(runtime::Image::RGB_24));
            m_operator->activate();
            DataContainer imageContainer = m_operator->getOutputData(DummyCamera::OUTPUT);
            const runtime::Image & image = ReadAccess<runtime::Image>(imageContainer)();
            
            cvsupport::Image::save("DummyCameraTest_testFlicker.png", image);
        }
        
        void DummyCameraTest::testValidateBufferSize()
        {
            UInt32 bufferSize = data_cast<UInt32>(m_operator->getParameter(DummyCamera::BUFFER_SIZE));
            UInt32 singleChannelSize((unsigned int)(bufferSize) / 3);
            
            m_operator->setParameter(DummyCamera::PIXEL_TYPE, Enum(runtime::Image::RGB_24));
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(DummyCamera::BUFFER_SIZE, singleChannelSize),
                                 WrongParameterValue);
            
            m_operator->setParameter(DummyCamera::PIXEL_TYPE, Enum(runtime::Image::MONO_8));
            CPPUNIT_ASSERT_NO_THROW(m_operator->setParameter(DummyCamera::BUFFER_SIZE, singleChannelSize));
            
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(DummyCamera::PIXEL_TYPE, Enum(runtime::Image::RGB_24)),
                                 WrongParameterValue);
        }

        void DummyCameraTest::tearDown ( void )
        {
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
            delete m_operator;
        }
    }
}

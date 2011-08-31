#include "CameraTest.h"

#include <base/Camera.h>
#include <base/Image.h>

#include <stream/DataContainer.h>
#include <stream/Trigger.h>
#include <stream/ReadAccess.h>
#include <stream/Enum.h>

#include <stream/impl/SynchronizedOperatorKernel.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::CameraTest);

using namespace stream;

namespace base
{
    void CameraTest::setUp ( void )
    {
        Image image("lenna.jpg");
        
        m_operator = new stream::impl::SynchronizedOperatorKernel(new Camera());
        m_operator->initialize();
        m_operator->setParameter(Camera::IMAGE, image);
        m_operator->setParameter(Camera::NUM_BUFFERS, UInt32(1));
        m_operator->setParameter(Camera::BUFFER_SIZE, UInt32(image.size()));
        
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
            m_operator->setParameter(Camera::TRIGGER, stream::Trigger());
            DataContainer imageContainer = m_operator->getOutputData(Camera::OUTPUT);
            DataContainer indexContainer = m_operator->getOutputData(Camera::INDEX);
            UInt32 index = dynamic_cast<const UInt32 &>(ReadAccess(indexContainer)());
            CPPUNIT_ASSERT_EQUAL(UInt32(0), index);
            
            const Image & image = dynamic_cast<const Image &>(ReadAccess(imageContainer)());
            image.save("CameraTest_testExecuteSoftwareTrigger.png");
        }
        
        m_operator->clearOutputData(Camera::OUTPUT);
        m_operator->clearOutputData(Camera::INDEX);
        
        {
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            m_operator->setParameter(Camera::TRIGGER, stream::Trigger());
            DataContainer imageContainer = m_operator->getOutputData(Camera::OUTPUT);
            DataContainer indexContainer = m_operator->getOutputData(Camera::INDEX);
            UInt32 index = dynamic_cast<const UInt32 &>(ReadAccess(indexContainer)());
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
            UInt32 index = dynamic_cast<const UInt32 &>(ReadAccess(indexContainer)());
            CPPUNIT_ASSERT_EQUAL(UInt32(0), index);
        }
        
        m_operator->clearOutputData(Camera::OUTPUT);
        m_operator->clearOutputData(Camera::INDEX);
        
        {
            DataContainer imageContainer = m_operator->getOutputData(Camera::OUTPUT);
            DataContainer indexContainer = m_operator->getOutputData(Camera::INDEX);
            UInt32 index = dynamic_cast<const UInt32 &>(ReadAccess(indexContainer)());
            CPPUNIT_ASSERT_EQUAL(UInt32(1), index);
        }
    }
    
    void CameraTest::testAdjustPixelType()
    {
        m_operator->setParameter(Camera::PIXEL_TYPE, Enum(stream::Image::BAYERBG_8));
        m_operator->activate();
        DataContainer imageContainer = m_operator->getOutputData(Camera::OUTPUT);
        const Image & image = dynamic_cast<const Image &>(ReadAccess(imageContainer)());
        
        CPPUNIT_ASSERT_EQUAL(image.pixelType(), stream::Image::BAYERBG_8);
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
        const Image & image = dynamic_cast<const Image &>(ReadAccess(imageContainer)());
        
        CPPUNIT_ASSERT_EQUAL(image.width(), (unsigned int)(319));
        CPPUNIT_ASSERT_EQUAL(image.height(), (unsigned int)(217));
        image.save("CameraTest_testAdjustRoi.png");
    }
    
    void CameraTest::testAdjustExposure()
    {
        m_operator->setParameter(Camera::EXPOSURE, UInt32(5));
        m_operator->setParameter(Camera::PIXEL_TYPE, Enum(stream::Image::RGB_24));
        m_operator->activate();
        DataContainer imageContainer = m_operator->getOutputData(Camera::OUTPUT);
        const Image & image = dynamic_cast<const Image &>(ReadAccess(imageContainer)());
        
        image.save("CameraTest_testAdjustExposure.png");
    }

    void CameraTest::testAdjustWhiteBalance()
    {
        m_operator->setParameter(Camera::WHITE_BALANCE_RED, Double(2.0));
        m_operator->setParameter(Camera::WHITE_BALANCE_GREEN, Double(1.0));
        m_operator->setParameter(Camera::WHITE_BALANCE_BLUE, Double(0.0));
        m_operator->setParameter(Camera::PIXEL_TYPE, Enum(stream::Image::RGB_24));
        m_operator->activate();
        DataContainer imageContainer = m_operator->getOutputData(Camera::OUTPUT);
        const Image & image = dynamic_cast<const Image &>(ReadAccess(imageContainer)());
        
        image.save("CameraTest_testAdjustWhiteBalance.png");
    }

    void CameraTest::tearDown ( void )
    {
        m_operator->deactivate();
        delete m_operator;
    }
}
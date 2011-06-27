#include "ClipTest.h"

#include <base/Clip.h>
#include <base/Image.h>
#include <stream/DataContainer.h>
#include <stream/OperatorWrapper.h>
#include <stream/Primitive.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::ClipTest);

using namespace stream;

namespace base
{
    void ClipTest::setUp ( void )
    {
        m_operator = new OperatorWrapper(new Clip());
        m_operator->activate();
        m_image = new DataContainer(new Image("lenna.jpg"));
        m_operator->setInputData(Clip::INPUT, m_image);
    }
    
    void ClipTest::testExecute()
    {
        m_operator->setParameter(Clip::LEFT, UInt32(200));
        m_operator->setParameter(Clip::TOP, UInt32(210));
        m_operator->setParameter(Clip::WIDTH, UInt32(100));
        m_operator->setParameter(Clip::HEIGHT, UInt32(90));
        
        stream::DataContainer* result = m_operator->getOutputData(Clip::OUTPUT);
        
        const Image* image = dynamic_cast<const Image*>(result->getReadAccess());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(100), image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(90), image->height());
        
        // simulate a reference by a following operator
        result->reference();
        m_operator->clearOutputData(Clip::OUTPUT);
        
        m_operator->setParameter(Clip::LEFT, UInt32(210));
        m_operator->setParameter(Clip::TOP, UInt32(200));
        m_operator->setParameter(Clip::WIDTH, UInt32(90));
        m_operator->setParameter(Clip::HEIGHT, UInt32(100));
        m_operator->setInputData(Clip::INPUT, m_image);
        
        result = m_operator->getOutputData(Clip::OUTPUT);
        
        image = dynamic_cast<const Image*>(result->getReadAccess());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(90), image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(100), image->height());
    }
    
    void ClipTest::testAdjustClipRegion1()
    {
        m_operator->setParameter(Clip::LEFT, UInt32(450));
        m_operator->setParameter(Clip::TOP, UInt32(500));
        m_operator->setParameter(Clip::WIDTH, UInt32(100));
        m_operator->setParameter(Clip::HEIGHT, UInt32(90));
        
        stream::DataContainer* result = m_operator->getOutputData(Clip::OUTPUT);
        
        const Image* image = dynamic_cast<const Image*>(result->getReadAccess());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(50), image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(12), image->height());
    }
    
    void ClipTest::testAdjustClipRegion2()
    {
        m_operator->setParameter(Clip::LEFT, UInt32(650));
        m_operator->setParameter(Clip::TOP, UInt32(700));
        m_operator->setParameter(Clip::WIDTH, UInt32(100));
        m_operator->setParameter(Clip::HEIGHT, UInt32(90));
        
        stream::DataContainer* result = m_operator->getOutputData(Clip::OUTPUT);
        
        const Image* image = dynamic_cast<const Image*>(result->getReadAccess());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), image->height());
    }

    void ClipTest::tearDown ( void )
    {
        delete m_operator;
        delete m_image;
    }
}
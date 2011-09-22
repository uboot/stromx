#include "ClipTest.h"

#include <base/Clip.h>
#include <base/Image.h>
#include <strom/DataContainer.h>
#include <strom/Primitive.h>
#include <strom/ReadAccess.h>

#include <strom/OperatorTester.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::ClipTest);

using namespace strom;

namespace base
{
    void ClipTest::setUp ( void )
    {
        m_operator = new strom::OperatorTester(new Clip());
        m_operator->initialize();
        m_operator->activate();
        DataContainer image(new Image("lenna.jpg"));
        m_operator->setInputData(Clip::INPUT, image);
    }
    
    void ClipTest::testExecute()
    {
        m_operator->setParameter(Clip::LEFT, UInt32(200));
        m_operator->setParameter(Clip::TOP, UInt32(210));
        m_operator->setParameter(Clip::WIDTH, UInt32(100));
        m_operator->setParameter(Clip::HEIGHT, UInt32(90));

        strom::DataContainer result = m_operator->getOutputData(Clip::OUTPUT);
            
        ReadAccess<Image> access(result);
        const Image& image = access();
        CPPUNIT_ASSERT_EQUAL((unsigned int)(100), image.width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(90), image.height());
        
        image.save("ClipTest_testExecute.png");
    }
    
    void ClipTest::testAdjustClipRegion1()
    {
        m_operator->setParameter(Clip::LEFT, UInt32(450));
        m_operator->setParameter(Clip::TOP, UInt32(500));
        m_operator->setParameter(Clip::WIDTH, UInt32(100));
        m_operator->setParameter(Clip::HEIGHT, UInt32(90));
        
        strom::DataContainer result = m_operator->getOutputData(Clip::OUTPUT);
        ReadAccess<Image> access(result);
        
        const Image& image = access();
        CPPUNIT_ASSERT_EQUAL((unsigned int)(50), image.width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(12), image.height());
        
        image.save("ClipTest_testAdjustClipRegion1.png");
    }
    
    void ClipTest::testAdjustClipRegion2()
    {
        m_operator->setParameter(Clip::LEFT, UInt32(650));
        m_operator->setParameter(Clip::TOP, UInt32(700));
        m_operator->setParameter(Clip::WIDTH, UInt32(100));
        m_operator->setParameter(Clip::HEIGHT, UInt32(90));
        
        strom::DataContainer result = m_operator->getOutputData(Clip::OUTPUT);
        ReadAccess<Image> access(result);
        
        const Image& image = access();
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), image.width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), image.height());
    }

    void ClipTest::tearDown ( void )
    {
        delete m_operator;
    }
}
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
    }
    
    void ClipTest::testExecute()
    {
        m_operator->setParameter(Clip::LEFT, UInt32(10));
        m_operator->setParameter(Clip::TOP, UInt32(20));
        m_operator->setParameter(Clip::WIDTH, UInt32(100));
        m_operator->setParameter(Clip::HEIGHT, UInt32(90));
        m_operator->setInputData(Clip::INPUT, m_image);
        
        stream::DataContainer* result = m_operator->getOutputData(Clip::OUTPUT);
        
        const Image* image = dynamic_cast<const Image*>(result->getReadAccess());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(100), image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(90), image->height());
        image->save("testExecuteClip.jpg");
    }

    void ClipTest::tearDown ( void )
    {
        delete m_operator;
    }
}
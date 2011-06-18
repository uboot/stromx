#include "TriggerTest.h"

#include <base/Trigger.h>
#include <base/Image.h>

#include <stream/Trigger.h>
#include <stream/DataContainer.h>
#include <stream/OperatorWrapper.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::TriggerTest);

using namespace stream;

namespace base
{
    void TriggerTest::setUp ( void )
    {
        m_operator = new OperatorWrapper(new Trigger());
        m_operator->activate();
        m_image = new DataContainer(new Image("lenna.jpg"));
        m_operator->setInputData(Trigger::INPUT, m_image);
    }
    
    void TriggerTest::testExecute()
    {
        m_operator->setParameter(Trigger::TRIGGER, stream::Trigger());
        
        DataContainer* result = m_operator->getOutputData(Trigger::OUTPUT);
        
        const Image* image = dynamic_cast<const Image*>(result->getReadAccess());
    }

    void TriggerTest::tearDown ( void )
    {
        delete m_operator;
        delete m_image;
    }
}
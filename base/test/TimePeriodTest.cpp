#include "TimePeriodTest.h"

#include <base/TimePeriod.h>
#include <base/Image.h>

#include <stream/DataContainer.h>
#include <stream/OperatorWrapper.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::TimePeriodTest);

using namespace stream;

namespace base
{
    void TimePeriodTest::setUp ( void )
    {
        m_operator = new OperatorWrapper(new TimePeriod());
        m_operator->activate();
        m_image = new DataContainer(new Image("lenna.jpg"));
        m_operator->setInputData(TimePeriod::INPUT, m_image);
    }
    
    void TimePeriodTest::testExecute()
    {
        m_operator->setParameter(TimePeriod::PERIOD, stream::UInt32(2000));
        
        DataContainer* result = m_operator->getOutputData(TimePeriod::PERIOD);
        
        const Image* image = dynamic_cast<const Image*>(result->getReadAccess());
    }

    void TimePeriodTest::tearDown ( void )
    {
        delete m_operator;
        delete m_image;
    }
}
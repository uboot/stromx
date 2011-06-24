#include "TriggerTest.h"

#include <base/Trigger.h>
#include <base/Image.h>

#include <stream/Trigger.h>
#include <stream/OperatorWrapper.h>
#include <stream/Exception.h>
#include <stream/ReadAccess.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::TriggerTest);

using namespace stream;

namespace base
{
    void TriggerTest::setUp ( void )
    {
        m_operator = new OperatorWrapper(new Trigger());
        m_operator->activate();
        m_image = DataContainer(new Image("lenna.jpg"));
        m_operator->setInputData(Trigger::INPUT, m_image);
    }
    
    void TriggerTest::testExecuteActive()
    {
        /*** Test 1 ***/
        boost::thread t1(boost::bind(&TriggerTest::triggerDelayed, this));
        DataContainer result = m_operator->getOutputData(Trigger::OUTPUT);
        
        ReadAccess access(result);
        const Image* image = dynamic_cast<const Image*>(access());
        CPPUNIT_ASSERT(image);
        
        m_operator->clearOutputData(Trigger::OUTPUT);
        m_operator->setInputData(Trigger::INPUT, m_image);
        t1.join();
        
        /*** Test 2 ***/
        boost::thread t2(boost::bind(&TriggerTest::triggerDelayed, this));
        result = m_operator->getOutputData(Trigger::OUTPUT);
        
        m_operator->clearOutputData(Trigger::OUTPUT);
        m_operator->setInputData(Trigger::INPUT, m_image);
        t2.join();
        
        /*** Test 2 ***/
        boost::thread t3(boost::bind(&TriggerTest::getOutputDataInterrupted, this));
        t3.interrupt();
        t3.join();
    }
    
    void TriggerTest::testExecuteInactive()
    {
        m_operator->setParameter(Trigger::ACTIVE, Bool(false));
        
        DataContainer result = m_operator->getOutputData(Trigger::OUTPUT);
        ReadAccess access(result);
        const Image* image = dynamic_cast<const Image*>(access());
        CPPUNIT_ASSERT(image);
    }
    
    void TriggerTest::getOutputDataInterrupted()
    {
        CPPUNIT_ASSERT_THROW(m_operator->getOutputData(Trigger::OUTPUT), InterruptException);
    }
        
    void TriggerTest::triggerDelayed()
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        m_operator->setParameter(Trigger::TRIGGER, stream::Trigger());
    }

    void TriggerTest::tearDown ( void )
    {
        delete m_operator;
    }
}
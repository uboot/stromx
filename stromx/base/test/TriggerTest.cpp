#include "TriggerTest.h"


#include <stromx/base/Trigger.h>

#include <stromx/base/Image.h>

#include <stromx/core/Trigger.h>
#include <stromx/core/Exception.h>
#include <stromx/core/ReadAccess.h>

#include <stromx/core/OperatorTester.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::TriggerTest);

namespace stromx
{
    using namespace core;

    namespace base
    {
        void TriggerTest::setUp ( void )
        {
            m_operator = new core::OperatorTester(new Trigger());
            m_operator->initialize();
            m_operator->activate();
            m_image = DataContainer(new Image("lenna.jpg"));
            m_operator->setInputData(Trigger::INPUT, m_image);
        }
        
        void TriggerTest::testExecuteActive()
        {
            /*** Test 1 ***/
            boost::thread t1(boost::bind(&TriggerTest::triggerDelayed, this));
            DataContainer result = m_operator->getOutputData(Trigger::OUTPUT);
            
            ReadAccess<Image> access(result);
            access();
            
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
            ReadAccess<Image> access(result);
            access();
        }
        
        void TriggerTest::getOutputDataInterrupted()
        {
            CPPUNIT_ASSERT_THROW(m_operator->getOutputData(Trigger::OUTPUT), Interrupt);
        }
            
        void TriggerTest::triggerDelayed()
        {
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            m_operator->setParameter(Trigger::TRIGGER, core::Trigger());
        }

        void TriggerTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}

#include "PeriodicDelayTest.h"

#include <base/PeriodicDelay.h>
#include <base/Image.h>

#include <stream/OperatorWrapper.h>
#include <stream/Exception.h>
#include <stream/ReadAccess.h>

#include <boost/thread.hpp>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::PeriodicDelayTest);

using namespace stream;

namespace base
{
    void PeriodicDelayTest::setUp ( void )
    {
        m_operator = new OperatorWrapper(new PeriodicDelay());
        m_operator->initialize();
        m_operator->activate();
        m_image = DataContainer(new Image("lenna.jpg"));
        m_operator->setInputData(PeriodicDelay::INPUT, m_image);
    }
    
    void PeriodicDelayTest::testExecute()
    {
        m_operator->setParameter(PeriodicDelay::PERIOD, stream::UInt32(1000));
        
        {
            DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
            ReadAccess access(result);
            const Image* image = dynamic_cast<const Image*>(access());
            CPPUNIT_ASSERT(image);
        }
        
        {
            m_operator->clearOutputData(PeriodicDelay::OUTPUT);
            m_operator->setInputData(PeriodicDelay::INPUT, m_image);
            DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
        }
        
        {
            m_operator->clearOutputData(PeriodicDelay::OUTPUT);
            m_operator->setInputData(PeriodicDelay::INPUT, m_image);
            DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
        }
        
        {
            m_operator->clearOutputData(PeriodicDelay::OUTPUT);
            m_operator->setInputData(PeriodicDelay::INPUT, m_image);
            DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
        }

        m_operator->clearOutputData(PeriodicDelay::OUTPUT);
        m_operator->setInputData(PeriodicDelay::INPUT, m_image);
        boost::thread t1(boost::bind(&PeriodicDelayTest::getOutputDataInterrupted, this));
        t1.interrupt();
        t1.join();
    }
    
    void PeriodicDelayTest::testExecuteZeroPeriod()
    {
        m_operator->setParameter(PeriodicDelay::PERIOD, stream::UInt32(0));
        
        {
            DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
            ReadAccess access(result);
            const Image* image = dynamic_cast<const Image*>(access());
            CPPUNIT_ASSERT(image);
        }
        
        {
            m_operator->clearOutputData(PeriodicDelay::OUTPUT);
            m_operator->setInputData(PeriodicDelay::INPUT, m_image);
            DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
        }
        
        {
            m_operator->clearOutputData(PeriodicDelay::OUTPUT);
            m_operator->setInputData(PeriodicDelay::INPUT, m_image);
            DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
        }   
        
        {
            m_operator->clearOutputData(PeriodicDelay::OUTPUT);
            m_operator->setInputData(PeriodicDelay::INPUT, m_image);
            DataContainer result = m_operator->getOutputData(PeriodicDelay::PERIOD);
        }
    }
    
    void PeriodicDelayTest::getOutputDataInterrupted()
    {
        CPPUNIT_ASSERT_THROW(m_operator->getOutputData(PeriodicDelay::OUTPUT), InterruptException);
    }
    
    void PeriodicDelayTest::tearDown ( void )
    {
        delete m_operator;
    }
}
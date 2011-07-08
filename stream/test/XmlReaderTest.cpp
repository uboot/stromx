#include "XmlReaderTest.h"

#include "TestOperator.h"

#include <cppunit/TestAssert.h>

#include <stream/XmlReader.h>
#include <stream/OperatorFactory.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::XmlReaderTest);

namespace stream
{
    void XmlReaderTest::setUp()
    {
        m_factory = new OperatorFactory;
        m_factory->registerOperator(new TestOperator());
    }
    
    void XmlReaderTest::testRead()
    {
        XmlReader reader(m_factory);
        
        Stream* stream = reader.read("network.xml");
    }

    void XmlReaderTest::tearDown()
    {
        delete m_factory;
    }
}

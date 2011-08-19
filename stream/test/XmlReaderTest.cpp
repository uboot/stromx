#include "XmlReaderTest.h"

#include "TestOperator.h"

#include <cppunit/TestAssert.h>

#include <stream/XmlReader.h>
#include <stream/Factory.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::XmlReaderTest);

namespace stream
{
    void XmlReaderTest::setUp()
    {
        m_factory = new Factory;
        m_factory->registerOperator(new TestOperator());
    }
    
    void XmlReaderTest::testRead()
    {
        Stream* stream = XmlReader::read("stream.xml", m_factory);
    }

    void XmlReaderTest::tearDown()
    {
        delete m_factory;
    }
}

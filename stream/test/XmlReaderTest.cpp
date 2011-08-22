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
        m_factory->registerData(new UInt32());
    }
    
    void XmlReaderTest::testRead()
    {
        Stream* stream = XmlReader(m_factory).read("stream.xml");
    }
    
    void XmlReaderTest::testComputePath()
    {
        CPPUNIT_ASSERT_EQUAL(std::string(""), XmlReader::computePath("test.xml"));
        CPPUNIT_ASSERT_EQUAL(std::string("/"), XmlReader::computePath("/test.xml"));
        CPPUNIT_ASSERT_EQUAL(std::string("dir/"), XmlReader::computePath("dir/test.xml"));
        CPPUNIT_ASSERT_EQUAL(std::string("/dir/"), XmlReader::computePath("/dir/test.xml"));
    }

    void XmlReaderTest::tearDown()
    {
        delete m_factory;
    }
}

#include "XmlWriterTest.h"

#include "TestOperator.h"

#include <cppunit/TestAssert.h>

#include <stream/XmlWriter.h>
#include <stream/Operator.h>
#include <stream/Node.h>
#include <stream/Thread.h>
#include <stream/Stream.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::XmlWriterTest);

namespace stream
{
    void XmlWriterTest::setUp()
    {
        Stream* stream = new Stream;
        stream->setName("TestStream");
        
        Operator* op0 = new Operator(new TestOperator);
        op0->setParameter(TestOperator::BUFFER_SIZE, UInt32(5000));
        op0->initialize();
        op0->setParameter(TestOperator::SLEEP_TIME, UInt32(200));
        op0->setName("Number 1");
        stream->addOperator(op0);
        
        Operator* op1 = new Operator(new TestOperator);
        op1->setParameter(TestOperator::BUFFER_SIZE, UInt32(6000));
        op1->initialize();
        op1->setParameter(TestOperator::SLEEP_TIME, UInt32(250));
        op1->setName("Number 2");
        stream->addOperator(op1);
        
        Operator* op2 = new Operator(new TestOperator);
        op2->setParameter(TestOperator::BUFFER_SIZE, UInt32(7000));
        op2->initialize();
        op2->setParameter(TestOperator::SLEEP_TIME, UInt32(300));
        op2->setName("Number 3");
        stream->addOperator(op2);
        
        stream->connect(op1, TestOperator::INPUT_1, op0, TestOperator::OUTPUT_1);
        stream->connect(op1, TestOperator::INPUT_2, op0, TestOperator::OUTPUT_2);
        
        stream->connect(op2, TestOperator::INPUT_1, op1, TestOperator::OUTPUT_1);
        stream->connect(op2, TestOperator::INPUT_2, op1, TestOperator::OUTPUT_2);
        
        Thread* thread = stream->addThread();
        thread->setName("Processing thread");
        
        thread->addNode(op1, TestOperator::INPUT_1);
        thread->addNode(op1, TestOperator::INPUT_2);
        thread->addNode(op2, TestOperator::INPUT_1);
        thread->addNode(op2, TestOperator::INPUT_2);
    }
    
    void XmlWriterTest::testWrite()
    {
        XmlWriter::write("XmlWriterTest_testWrite.xml", *m_stream);
    }

    void XmlWriterTest::tearDown()
    {
        delete m_stream;
    }

}

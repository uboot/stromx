#include "XmlWriterTest.h"

#include "TestOperator.h"

#include <cppunit/TestAssert.h>

#include <stream/XmlWriter.h>
#include <stream/Network.h>
#include <stream/OperatorNode.h>
#include <stream/InputNode.h>
#include <stream/Thread.h>
#include <stream/Stream.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::XmlWriterTest);

namespace stream
{
    void XmlWriterTest::setUp()
    {
        Network* network = new Network;
        network->setName("TestNetwork");
        
        OperatorNode* op0 = network->addOperator(new TestOperator());
        op0->setName("Number 1");
        
        OperatorNode* op1 = network->addOperator(new TestOperator());
        op1->setName("Number 2");
        
        OperatorNode* op2 = network->addOperator(new TestOperator());
        op2->setName("Number 3");
        
        op1->getInputNode(TestOperator::INPUT_1)->connect(op0->getOutputNode(TestOperator::OUTPUT_1));
        op1->getInputNode(TestOperator::INPUT_2)->connect(op0->getOutputNode(TestOperator::OUTPUT_2));
        
        op2->getInputNode(TestOperator::INPUT_1)->connect(op1->getOutputNode(TestOperator::OUTPUT_1));
        op2->getInputNode(TestOperator::INPUT_2)->connect(op1->getOutputNode(TestOperator::OUTPUT_2));
        
        Thread* thread = new Thread();
        thread->setName("Processing thread");
        
        thread->addNode(op1->getInputNode(TestOperator::INPUT_1));
        thread->addNode(op1->getInputNode(TestOperator::INPUT_2));
        thread->addNode(op2->getInputNode(TestOperator::INPUT_1));
        thread->addNode(op2->getInputNode(TestOperator::INPUT_2));
        
        m_stream = new Stream(network);
        m_stream->addThread(thread);
    }
    
    void XmlWriterTest::testWrite()
    {
        XmlWriter writer;
        
        writer.write("XmlWriterTest_testWrite.xml", *m_stream);
    }

    void XmlWriterTest::tearDown()
    {
        delete m_stream;
    }

}

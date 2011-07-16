#include "XmlWriterTest.h"

#include "TestOperator.h"

#include <cppunit/TestAssert.h>

#include <stream/XmlWriter.h>
#include <stream/Network.h>
#include <stream/OperatorWrapper.h>
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
        
        OperatorWrapper* wrapper0 = new OperatorWrapper(new TestOperator());
        wrapper0->setParameter(TestOperator::BUFFER_SIZE, UInt32(5000));
        wrapper0->initialize();
        wrapper0->setParameter(TestOperator::SLEEP_TIME, UInt32(200));
        OperatorNode* op0 = network->addOperator(wrapper0);
        op0->setName("Number 1");
        
        OperatorWrapper* wrapper1 = new OperatorWrapper(new TestOperator());
        wrapper1->setParameter(TestOperator::BUFFER_SIZE, UInt32(6000));
        wrapper1->initialize();
        wrapper1->setParameter(TestOperator::SLEEP_TIME, UInt32(250));
        OperatorNode* op1 = network->addOperator(wrapper1);
        op1->setName("Number 2");
        
        OperatorWrapper* wrapper2 = new OperatorWrapper(new TestOperator());
        wrapper2->setParameter(TestOperator::BUFFER_SIZE, UInt32(7000));
        wrapper2->initialize();
        wrapper2->setParameter(TestOperator::SLEEP_TIME, UInt32(300));
        OperatorNode* op2 = network->addOperator(wrapper2);
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

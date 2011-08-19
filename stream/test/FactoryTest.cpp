#include "FactoryTest.h"

#include <cppunit/TestAssert.h>

#include <stream/Factory.h>
#include <stream/Exception.h>

#include "TestOperator.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stream::FactoryTest);

namespace stream
{
    void FactoryTest::setUp()
    {
        // allocate a new empty factory
        m_factory = new Factory;
    }

    void FactoryTest::tearDown()
    {
        // the factory automatically deletes all its operators
        // (cf. aggregation, i.e. the connection with the black diamond, in
        // the UML diagram)
        delete m_factory;
    }
    
    void FactoryTest::testNewOperator()
    {
        // add a test operator to the factory
        OperatorKernel* op = new TestOperator;
        m_factory->registerOperator(op);
        
        // try to get an operator which does not exist in the factory
        CPPUNIT_ASSERT_THROW(m_factory->newOperator("FunnyPackage", "RareOperator"), WrongArgument);
        
        // get the existing operator
        OperatorKernel* newOp;
        CPPUNIT_ASSERT_NO_THROW(newOp = m_factory->newOperator("TestPackage", "test"));
        CPPUNIT_ASSERT(newOp);
        
        delete newOp;
    }
    
    void FactoryTest::testRegisterOperator()
    {
        // only non-zero input arguments are allowed
        CPPUNIT_ASSERT_THROW(m_factory->registerOperator(0), WrongArgument);
        
        // add a new operator to the factory
        OperatorKernel* op = new TestOperator;
        CPPUNIT_ASSERT_NO_THROW(m_factory->registerOperator(op));
        
        // test if it has been successfully added
        CPPUNIT_ASSERT_EQUAL(1, (int)m_factory->availableOperators().size());
        CPPUNIT_ASSERT_EQUAL((const OperatorKernel*)(op), m_factory->availableOperators()[0]);
        
        // now we try to add another operator of the same type
        // this should result in an exception, because an operator with the same name()
        // and package() has already been added to the factory
        OperatorKernel* duplicateOp = new TestOperator;
        CPPUNIT_ASSERT_THROW(m_factory->registerOperator(duplicateOp), WrongArgument);
        
        // delete duplicateOperator to avoid a memory leak
        // this is not necessary for op because it belongs to the factory
        delete duplicateOp;
    }
}

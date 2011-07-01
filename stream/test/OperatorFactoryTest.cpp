#include "OperatorFactoryTest.h"

#include <cppunit/TestAssert.h>

#include <stream/OperatorFactory.h>
#include <stream/Exception.h>

#include "TestOperator.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stream::OperatorFactoryTest);

namespace stream
{
    void OperatorFactoryTest::setUp()
    {
        // allocate a new empty factory
        m_factory = new OperatorFactory;
    }

    void OperatorFactoryTest::tearDown()
    {
        // the factory automatically deletes all its operators
        // (cf. aggregation, i.e. the connection with the black diamond, in
        // the UML diagram)
        delete m_factory;
    }
    
    void OperatorFactoryTest::testNewOperator()
    {
        // add a test operator to the factory
        Operator* op = new TestOperator;
        m_factory->registerOperator(op);
        
        // try to get an operator which does not exist in the factory
        CPPUNIT_ASSERT_THROW(m_factory->newOperator("FunnyPackage", "RareOperator"), ArgumentException);
        
        // get the existing operator
        Operator* newOp;
        CPPUNIT_ASSERT_NO_THROW(newOp = m_factory->newOperator("TestPackage", "TestOperator"));
        CPPUNIT_ASSERT(newOp);
        
        delete newOp;
    }
    
    void OperatorFactoryTest::testRegisterOperator()
    {
        // only non-zero input arguments are allowed
        CPPUNIT_ASSERT_THROW(m_factory->registerOperator(0), ArgumentException);
        
        // add a new operator to the factory
        Operator* op = new TestOperator;
        CPPUNIT_ASSERT_NO_THROW(m_factory->registerOperator(op));
        
        // test if it has been successfully added
        CPPUNIT_ASSERT_EQUAL(1, (int)m_factory->availableOperators().size());
        CPPUNIT_ASSERT_EQUAL((const Operator*)(op), m_factory->availableOperators()[0]);
        
        // now we try to add another operator of the same type
        // this should result in an exception, because an operator with the same name()
        // and package() has already been added to the factory
        Operator* duplicateOp = new TestOperator;
        CPPUNIT_ASSERT_THROW(m_factory->registerOperator(duplicateOp), ArgumentException);
        
        // delete duplicateOperator to avoid a memory leak
        // this is not necessary for op because it belongs to the factory
        delete duplicateOp;
    }
}

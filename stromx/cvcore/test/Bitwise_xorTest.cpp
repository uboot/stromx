#include "stromx/cvcore/test/Bitwise_xorTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvcore/Bitwise_xor.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvcore::Bitwise_xorTest);

namespace stromx
{
    namespace cvcore
    {
        void Bitwise_xorTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Bitwise_xor);
        }
        
        void Bitwise_xorTest::tearDown()
        {
            delete m_operator;
        }
        
        void Bitwise_xorTest::testManual0()
        {
            m_operator->setParameter(Bitwise_xor::DATA_FLOW, runtime::Enum(Bitwise_xor::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            
            m_operator->setInputData(Bitwise_xor::SRC_1, src1);
            m_operator->setInputData(Bitwise_xor::SRC_2, src2);
            m_operator->setInputData(Bitwise_xor::DST, dst);
            
            runtime::DataContainer result = m_operator->getOutputData(Bitwise_xor::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("Bitwise_xorTest_testManual0.png", access());
        }
        
        void Bitwise_xorTest::testManual1()
        {
            m_operator->setParameter(Bitwise_xor::DATA_FLOW, runtime::Enum(Bitwise_xor::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            
            m_operator->setInputData(Bitwise_xor::SRC_1, src1);
            m_operator->setInputData(Bitwise_xor::SRC_2, src2);
            m_operator->setInputData(Bitwise_xor::DST, dst);
            
            runtime::DataContainer result = m_operator->getOutputData(Bitwise_xor::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("Bitwise_xorTest_testManual1.png", access());
        }
        
        void Bitwise_xorTest::testAllocate0()
        {
            m_operator->setParameter(Bitwise_xor::DATA_FLOW, runtime::Enum(Bitwise_xor::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::DEPTH_16));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg", cvsupport::Image::DEPTH_16));
            
            m_operator->setInputData(Bitwise_xor::SRC_1, src1);
            m_operator->setInputData(Bitwise_xor::SRC_2, src2);
            
            runtime::DataContainer result = m_operator->getOutputData(Bitwise_xor::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("Bitwise_xorTest_testAllocate0.png", access());
        }
        
    }
}


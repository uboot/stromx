#include "stromx/cvcore/test/Bitwise_andTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvcore/Bitwise_and.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvcore::Bitwise_andTest);

namespace stromx
{
    namespace cvcore
    {
        void Bitwise_andTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Bitwise_and);
        }
        
        void Bitwise_andTest::tearDown()
        {
            delete m_operator;
        }
        
        void Bitwise_andTest::testManual0()
        {
            m_operator->setParameter(Bitwise_and::DATA_FLOW, runtime::Enum(Bitwise_and::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            
            m_operator->setInputData(Bitwise_and::SRC_1, src1);
            m_operator->setInputData(Bitwise_and::SRC_2, src2);
            m_operator->setInputData(Bitwise_and::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Bitwise_and::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("Bitwise_andTest_testManual0_dst.png", dstAccess());
        }
        
        void Bitwise_andTest::testManual1()
        {
            m_operator->setParameter(Bitwise_and::DATA_FLOW, runtime::Enum(Bitwise_and::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            
            m_operator->setInputData(Bitwise_and::SRC_1, src1);
            m_operator->setInputData(Bitwise_and::SRC_2, src2);
            m_operator->setInputData(Bitwise_and::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Bitwise_and::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("Bitwise_andTest_testManual1_dst.png", dstAccess());
        }
        
        void Bitwise_andTest::testAllocate0()
        {
            m_operator->setParameter(Bitwise_and::DATA_FLOW, runtime::Enum(Bitwise_and::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::DEPTH_16));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg", cvsupport::Image::DEPTH_16));
            
            m_operator->setInputData(Bitwise_and::SRC_1, src1);
            m_operator->setInputData(Bitwise_and::SRC_2, src2);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Bitwise_and::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("Bitwise_andTest_testAllocate0_dst.png", dstAccess());
        }
        
    } // cvcore
} // stromx


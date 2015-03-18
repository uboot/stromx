#include "stromx/cvcore/test/Bitwise_orTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvcore/Bitwise_or.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvcore::Bitwise_orTest);

namespace stromx
{
    namespace cvcore
    {
        void Bitwise_orTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Bitwise_or);
        }
        
        void Bitwise_orTest::tearDown()
        {
            delete m_operator;
        }
        
        void Bitwise_orTest::testManual0()
        {
            m_operator->setParameter(Bitwise_or::DATA_FLOW, runtime::Enum(Bitwise_or::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            
            m_operator->setInputData(Bitwise_or::SRC_1, src1);
            m_operator->setInputData(Bitwise_or::SRC_2, src2);
            m_operator->setInputData(Bitwise_or::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Bitwise_or::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("Bitwise_orTest_testManual0_dst.png", dstAccess());
        }
        
        void Bitwise_orTest::testManual1()
        {
            m_operator->setParameter(Bitwise_or::DATA_FLOW, runtime::Enum(Bitwise_or::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            
            m_operator->setInputData(Bitwise_or::SRC_1, src1);
            m_operator->setInputData(Bitwise_or::SRC_2, src2);
            m_operator->setInputData(Bitwise_or::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Bitwise_or::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("Bitwise_orTest_testManual1_dst.png", dstAccess());
        }
        
        void Bitwise_orTest::testAllocate0()
        {
            m_operator->setParameter(Bitwise_or::DATA_FLOW, runtime::Enum(Bitwise_or::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::DEPTH_16));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg", cvsupport::Image::DEPTH_16));
            
            m_operator->setInputData(Bitwise_or::SRC_1, src1);
            m_operator->setInputData(Bitwise_or::SRC_2, src2);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Bitwise_or::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("Bitwise_orTest_testAllocate0_dst.png", dstAccess());
        }
        
    } // cvcore
} // stromx


#include "stromx/cvcore/test/Bitwise_notTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvcore/Bitwise_not.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvcore::Bitwise_notTest);

namespace stromx
{
    namespace cvcore
    {
        void Bitwise_notTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Bitwise_not);
        }
        
        void Bitwise_notTest::tearDown()
        {
            delete m_operator;
        }
        
        void Bitwise_notTest::testManual0()
        {
            m_operator->setParameter(Bitwise_not::DATA_FLOW, runtime::Enum(Bitwise_not::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            
            m_operator->setInputData(Bitwise_not::SRC_1, src1);
            m_operator->setInputData(Bitwise_not::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Bitwise_not::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("Bitwise_notTest_testManual0_dst.png", dstAccess());
        }
        
        void Bitwise_notTest::testManual1()
        {
            m_operator->setParameter(Bitwise_not::DATA_FLOW, runtime::Enum(Bitwise_not::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            
            m_operator->setInputData(Bitwise_not::SRC_1, src1);
            m_operator->setInputData(Bitwise_not::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Bitwise_not::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("Bitwise_notTest_testManual1_dst.png", dstAccess());
        }
        
        void Bitwise_notTest::testAllocate0()
        {
            m_operator->setParameter(Bitwise_not::DATA_FLOW, runtime::Enum(Bitwise_not::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::DEPTH_16));
            
            m_operator->setInputData(Bitwise_not::SRC_1, src1);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Bitwise_not::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("Bitwise_notTest_testAllocate0_dst.png", dstAccess());
        }
        
    } // cvcore
} // stromx


#include "stromx/cvcore/test/AddWeightedTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvcore/AddWeighted.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvcore::AddWeightedTest);

namespace stromx
{
    namespace cvcore
    {
        void AddWeightedTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new AddWeighted);
        }
        
        void AddWeightedTest::tearDown()
        {
            delete m_operator;
        }
        
        void AddWeightedTest::testManual0()
        {
            m_operator->setParameter(AddWeighted::DATA_FLOW, runtime::Enum(AddWeighted::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            
            m_operator->setInputData(AddWeighted::SRC_1, src1);
            m_operator->setInputData(AddWeighted::SRC_2, src2);
            m_operator->setInputData(AddWeighted::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(AddWeighted::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("AddWeightedTest_testManual0_dst.png", dstAccess());
        }
        
        void AddWeightedTest::testManual1()
        {
            m_operator->setParameter(AddWeighted::DATA_FLOW, runtime::Enum(AddWeighted::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Float64 alpha(2.0);
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Float64 beta(0.5);
            runtime::Float64 gamma(3.0);
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            
            m_operator->setInputData(AddWeighted::SRC_1, src1);
            m_operator->setParameter(AddWeighted::ALPHA, alpha);
            m_operator->setInputData(AddWeighted::SRC_2, src2);
            m_operator->setParameter(AddWeighted::BETA, beta);
            m_operator->setParameter(AddWeighted::GAMMA, gamma);
            m_operator->setInputData(AddWeighted::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(AddWeighted::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("AddWeightedTest_testManual1_dst.png", dstAccess());
        }
        
        void AddWeightedTest::testManual2()
        {
            m_operator->setParameter(AddWeighted::DATA_FLOW, runtime::Enum(AddWeighted::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::DEPTH_16));
            runtime::Float64 alpha(1.0);
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg"));
            runtime::Float64 beta(0.5);
            runtime::Float64 gamma(-10);
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            runtime::Enum ddepth(1);
            
            m_operator->setInputData(AddWeighted::SRC_1, src1);
            m_operator->setParameter(AddWeighted::ALPHA, alpha);
            m_operator->setInputData(AddWeighted::SRC_2, src2);
            m_operator->setParameter(AddWeighted::BETA, beta);
            m_operator->setParameter(AddWeighted::GAMMA, gamma);
            m_operator->setInputData(AddWeighted::DST, dst);
            m_operator->setParameter(AddWeighted::DDEPTH, ddepth);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(AddWeighted::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("AddWeightedTest_testManual2_dst.png", dstAccess());
        }
        
        void AddWeightedTest::testAllocate0()
        {
            m_operator->setParameter(AddWeighted::DATA_FLOW, runtime::Enum(AddWeighted::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::DEPTH_16));
            runtime::Float64 alpha(-1.0);
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg", cvsupport::Image::DEPTH_16));
            runtime::Float64 beta(10.0);
            runtime::Float64 gamma(2.0);
            
            m_operator->setInputData(AddWeighted::SRC_1, src1);
            m_operator->setParameter(AddWeighted::ALPHA, alpha);
            m_operator->setInputData(AddWeighted::SRC_2, src2);
            m_operator->setParameter(AddWeighted::BETA, beta);
            m_operator->setParameter(AddWeighted::GAMMA, gamma);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(AddWeighted::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("AddWeightedTest_testAllocate0_dst.png", dstAccess());
        }
        
        void AddWeightedTest::testAllocate1()
        {
            m_operator->setParameter(AddWeighted::DATA_FLOW, runtime::Enum(AddWeighted::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::DEPTH_16));
            runtime::Float64 alpha(-10.0);
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg"));
            runtime::Float64 beta(2.0);
            runtime::Float64 gamma(0.0);
            runtime::Enum ddepth(2);
            
            m_operator->setInputData(AddWeighted::SRC_1, src1);
            m_operator->setParameter(AddWeighted::ALPHA, alpha);
            m_operator->setInputData(AddWeighted::SRC_2, src2);
            m_operator->setParameter(AddWeighted::BETA, beta);
            m_operator->setParameter(AddWeighted::GAMMA, gamma);
            m_operator->setParameter(AddWeighted::DDEPTH, ddepth);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(AddWeighted::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("AddWeightedTest_testAllocate1_dst.png", dstAccess());
        }
        
    } // cvcore
} // stromx


#include "stromx/cvimgproc/test/BilateralFilterTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/BilateralFilter.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::BilateralFilterTest);

namespace stromx
{
    namespace cvimgproc
    {
        void BilateralFilterTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new BilateralFilter);
        }
        
        void BilateralFilterTest::tearDown()
        {
            delete m_operator;
        }
        
        void BilateralFilterTest::testManual0()
        {
            m_operator->setParameter(BilateralFilter::DATA_FLOW, runtime::Enum(BilateralFilter::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            runtime::UInt32 d(9);
            runtime::Float64 sigmaColor(100);
            runtime::Float64 sigmaSpace(75);
            
            m_operator->setInputData(BilateralFilter::SRC, src);
            m_operator->setInputData(BilateralFilter::DST, dst);
            m_operator->setParameter(BilateralFilter::D, d);
            m_operator->setParameter(BilateralFilter::SIGMA_COLOR, sigmaColor);
            m_operator->setParameter(BilateralFilter::SIGMA_SPACE, sigmaSpace);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(BilateralFilter::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("BilateralFilterTest_testManual0_dst.png", dstAccess());
        }
        
        void BilateralFilterTest::testAllocate0()
        {
            m_operator->setParameter(BilateralFilter::DATA_FLOW, runtime::Enum(BilateralFilter::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            
            m_operator->setInputData(BilateralFilter::SRC, src);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(BilateralFilter::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("BilateralFilterTest_testAllocate0_dst.png", dstAccess());
        }
        
        void BilateralFilterTest::testAllocate1()
        {
            m_operator->setParameter(BilateralFilter::DATA_FLOW, runtime::Enum(BilateralFilter::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::UInt32 d(9);
            runtime::Float64 sigmaColor(100);
            runtime::Float64 sigmaSpace(75);
            
            m_operator->setInputData(BilateralFilter::SRC, src);
            m_operator->setParameter(BilateralFilter::D, d);
            m_operator->setParameter(BilateralFilter::SIGMA_COLOR, sigmaColor);
            m_operator->setParameter(BilateralFilter::SIGMA_SPACE, sigmaSpace);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(BilateralFilter::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("BilateralFilterTest_testAllocate1_dst.png", dstAccess());
        }
        
    } // cvimgproc
} // stromx


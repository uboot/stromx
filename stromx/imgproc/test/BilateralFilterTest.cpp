#include "stromx/imgproc/test/BilateralFilterTest.h"

#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/imgutil/Image.h"
#include "stromx/imgproc/BilateralFilter.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::imgproc::BilateralFilterTest);

namespace stromx
{
    namespace imgproc
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
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg"));
            runtime::DataContainer dst(new imgutil::Image(1000000));
            runtime::UInt32 d(9);
            runtime::Double sigmaColor(100);
            runtime::Double sigmaSpace(75);
            
            m_operator->setInputData(BilateralFilter::SRC, src);
            m_operator->setInputData(BilateralFilter::DST, dst);
            m_operator->setParameter(BilateralFilter::D, d);
            m_operator->setParameter(BilateralFilter::SIGMA_COLOR, sigmaColor);
            m_operator->setParameter(BilateralFilter::SIGMA_SPACE, sigmaSpace);
            
            runtime::DataContainer result = m_operator->getOutputData(BilateralFilter::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("BilateralFilterTest_testManual0.png", access());
        }
        
        void BilateralFilterTest::testAllocate0()
        {
            m_operator->setParameter(BilateralFilter::DATA_FLOW, runtime::Enum(BilateralFilter::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg"));
            
            m_operator->setInputData(BilateralFilter::SRC, src);
            
            runtime::DataContainer result = m_operator->getOutputData(BilateralFilter::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("BilateralFilterTest_testAllocate0.png", access());
        }
        
        void BilateralFilterTest::testAllocate1()
        {
            m_operator->setParameter(BilateralFilter::DATA_FLOW, runtime::Enum(BilateralFilter::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg", imgutil::Image::GRAYSCALE));
            runtime::UInt32 d(9);
            runtime::Double sigmaColor(100);
            runtime::Double sigmaSpace(75);
            
            m_operator->setInputData(BilateralFilter::SRC, src);
            m_operator->setParameter(BilateralFilter::D, d);
            m_operator->setParameter(BilateralFilter::SIGMA_COLOR, sigmaColor);
            m_operator->setParameter(BilateralFilter::SIGMA_SPACE, sigmaSpace);
            
            runtime::DataContainer result = m_operator->getOutputData(BilateralFilter::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("BilateralFilterTest_testAllocate1.png", access());
        }
        
    }
}


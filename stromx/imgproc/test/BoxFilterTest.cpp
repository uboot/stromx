#include "stromx/imgproc/test/BoxFilterTest.h"

#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/imgutil/Image.h"
#include "stromx/imgproc/BoxFilter.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::imgproc::BoxFilterTest);

namespace stromx
{
    namespace imgproc
    {
        void BoxFilterTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new BoxFilter);
        }
        
        void BoxFilterTest::tearDown()
        {
            delete m_operator;
        }
        
        void BoxFilterTest::testManual0()
        {
            m_operator->setParameter(BoxFilter::DATA_FLOW, runtime::Enum(BoxFilter::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg"));
            runtime::DataContainer dst(new imgutil::Image(1000000));
            runtime::UInt32 ksizex(5);
            runtime::UInt32 ksizey(4);
            
            m_operator->setInputData(BoxFilter::SRC, src);
            m_operator->setInputData(BoxFilter::DST, dst);
            m_operator->setParameter(BoxFilter::KSIZEX, ksizex);
            m_operator->setParameter(BoxFilter::KSIZEY, ksizey);
            
            runtime::DataContainer result = m_operator->getOutputData(BoxFilter::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("BoxFilterTest_testManual0.png", access());
        }
        
        void BoxFilterTest::testManual1()
        {
            m_operator->setParameter(BoxFilter::DATA_FLOW, runtime::Enum(BoxFilter::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg"));
            
            m_operator->setInputData(BoxFilter::SRC, src);
            m_operator->setInputData(BoxFilter::DST, src);
            
            runtime::DataContainer result = m_operator->getOutputData(BoxFilter::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("BoxFilterTest_testManual1.png", access());
        }
        
        void BoxFilterTest::testAllocate0()
        {
            m_operator->setParameter(BoxFilter::DATA_FLOW, runtime::Enum(BoxFilter::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg", imgutil::Image::GRAYSCALE));
            runtime::UInt32 ksizex(4);
            runtime::UInt32 ksizey(5);
            
            m_operator->setInputData(BoxFilter::SRC, src);
            m_operator->setParameter(BoxFilter::KSIZEX, ksizex);
            m_operator->setParameter(BoxFilter::KSIZEY, ksizey);
            
            runtime::DataContainer result = m_operator->getOutputData(BoxFilter::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("BoxFilterTest_testAllocate0.png", access());
        }
        
        void BoxFilterTest::testInPlace0()
        {
            m_operator->setParameter(BoxFilter::DATA_FLOW, runtime::Enum(BoxFilter::IN_PLACE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg"));
            
            m_operator->setInputData(BoxFilter::SRC, src);
            
            runtime::DataContainer result = m_operator->getOutputData(BoxFilter::SRC);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("BoxFilterTest_testInPlace0.png", access());
        }
        
    }
}


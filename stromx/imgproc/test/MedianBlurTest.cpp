#include "stromx/imgproc/test/MedianBlurTest.h"

#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/imgutil/Image.h"
#include "stromx/imgproc/MedianBlur.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::imgproc::MedianBlurTest);

namespace stromx
{
    namespace imgproc
    {
        void MedianBlurTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new MedianBlur);
        }
        
        void MedianBlurTest::tearDown()
        {
            delete m_operator;
        }
        
        void MedianBlurTest::testManual0()
        {
            m_operator->setParameter(MedianBlur::DATA_FLOW, runtime::Enum(MedianBlur::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg"));
            runtime::DataContainer dst(new imgutil::Image(1000000));
            runtime::UInt32 ksize(3);
            
            m_operator->setInputData(MedianBlur::SRC, src);
            m_operator->setInputData(MedianBlur::DST, dst);
            m_operator->setParameter(MedianBlur::KSIZE, ksize);
            
            runtime::DataContainer result = m_operator->getOutputData(MedianBlur::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("MedianBlurTest_testManual0.png", access());
        }
        
        void MedianBlurTest::testManual1()
        {
            m_operator->setParameter(MedianBlur::DATA_FLOW, runtime::Enum(MedianBlur::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg", imgutil::Image::GRAYSCALE));
            runtime::UInt32 ksize(5);
            
            m_operator->setInputData(MedianBlur::SRC, src);
            m_operator->setInputData(MedianBlur::DST, src);
            m_operator->setParameter(MedianBlur::KSIZE, ksize);
            
            runtime::DataContainer result = m_operator->getOutputData(MedianBlur::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("MedianBlurTest_testManual1.png", access());
        }
        
        void MedianBlurTest::testAllocate0()
        {
            m_operator->setParameter(MedianBlur::DATA_FLOW, runtime::Enum(MedianBlur::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg", imgutil::Image::GRAYSCALE));
            
            m_operator->setInputData(MedianBlur::SRC, src);
            
            runtime::DataContainer result = m_operator->getOutputData(MedianBlur::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("MedianBlurTest_testAllocate0.png", access());
        }
        
        void MedianBlurTest::testInPlace0()
        {
            m_operator->setParameter(MedianBlur::DATA_FLOW, runtime::Enum(MedianBlur::IN_PLACE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg"));
            
            m_operator->setInputData(MedianBlur::SRC, src);
            
            runtime::DataContainer result = m_operator->getOutputData(MedianBlur::SRC);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("MedianBlurTest_testInPlace0.png", access());
        }
        
    }
}


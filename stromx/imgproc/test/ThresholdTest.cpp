#include "stromx/imgproc/test/ThresholdTest.h"

#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/imgutil/Image.h"
#include "stromx/imgproc/Threshold.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::imgproc::ThresholdTest);

namespace stromx
{
    namespace imgproc
    {
        void ThresholdTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Threshold);
        }
        
        void ThresholdTest::tearDown()
        {
            delete m_operator;
        }
        
        void ThresholdTest::testManual0()
        {
            m_operator->setParameter(Threshold::DATA_FLOW, runtime::Enum(Threshold::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg", imgutil::Image::GRAYSCALE));
            runtime::DataContainer dst(new imgutil::Image(1000000));
            
            m_operator->setInputData(Threshold::SRC, src);
            m_operator->setInputData(Threshold::DST, dst);
            
            runtime::DataContainer result = m_operator->getOutputData(Threshold::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("ThresholdTest_testManual0.png", access());
        }
        
        void ThresholdTest::testManual1()
        {
            m_operator->setParameter(Threshold::DATA_FLOW, runtime::Enum(Threshold::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg", imgutil::Image::GRAYSCALE));
            runtime::Double threshold(128);
            runtime::Enum thresholdType(2);
            
            m_operator->setInputData(Threshold::SRC, src);
            m_operator->setInputData(Threshold::DST, src);
            m_operator->setParameter(Threshold::THRESHOLD, threshold);
            m_operator->setParameter(Threshold::THRESHOLD_TYPE, thresholdType);
            
            runtime::DataContainer result = m_operator->getOutputData(Threshold::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("ThresholdTest_testManual1.png", access());
        }
        
        void ThresholdTest::testAllocate0()
        {
            m_operator->setParameter(Threshold::DATA_FLOW, runtime::Enum(Threshold::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg", imgutil::Image::GRAYSCALE));
            runtime::Enum thresholdType(3);
            
            m_operator->setInputData(Threshold::SRC, src);
            m_operator->setParameter(Threshold::THRESHOLD_TYPE, thresholdType);
            
            runtime::DataContainer result = m_operator->getOutputData(Threshold::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("ThresholdTest_testAllocate0.png", access());
        }
        
        void ThresholdTest::testInPlace0()
        {
            m_operator->setParameter(Threshold::DATA_FLOW, runtime::Enum(Threshold::IN_PLACE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg", imgutil::Image::GRAYSCALE));
            runtime::Enum thresholdType(4);
            
            m_operator->setInputData(Threshold::SRC, src);
            m_operator->setParameter(Threshold::THRESHOLD_TYPE, thresholdType);
            
            runtime::DataContainer result = m_operator->getOutputData(Threshold::SRC);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("ThresholdTest_testInPlace0.png", access());
        }
        
    }
}


#include "stromx/cvimgproc/test/ThresholdTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/Threshold.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::ThresholdTest);

namespace stromx
{
    namespace cvimgproc
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
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            
            m_operator->setInputData(Threshold::SRC, src);
            m_operator->setInputData(Threshold::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Threshold::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("ThresholdTest_testManual0_dst.png", dstAccess());
        }
        
        void ThresholdTest::testManual1()
        {
            m_operator->setParameter(Threshold::DATA_FLOW, runtime::Enum(Threshold::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Float64 threshold(128);
            runtime::Enum thresholdType(2);
            
            m_operator->setInputData(Threshold::SRC, src);
            m_operator->setInputData(Threshold::DST, src);
            m_operator->setParameter(Threshold::THRESHOLD, threshold);
            m_operator->setParameter(Threshold::THRESHOLD_TYPE, thresholdType);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Threshold::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("ThresholdTest_testManual1_dst.png", dstAccess());
        }
        
        void ThresholdTest::testAllocate0()
        {
            m_operator->setParameter(Threshold::DATA_FLOW, runtime::Enum(Threshold::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Enum thresholdType(3);
            
            m_operator->setInputData(Threshold::SRC, src);
            m_operator->setParameter(Threshold::THRESHOLD_TYPE, thresholdType);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Threshold::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("ThresholdTest_testAllocate0_dst.png", dstAccess());
        }
        
        void ThresholdTest::testInPlace0()
        {
            m_operator->setParameter(Threshold::DATA_FLOW, runtime::Enum(Threshold::IN_PLACE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Enum thresholdType(4);
            
            m_operator->setInputData(Threshold::SRC, src);
            m_operator->setParameter(Threshold::THRESHOLD_TYPE, thresholdType);
            
            runtime::DataContainer srcResult = m_operator->getOutputData(Threshold::SRC);
            
            runtime::ReadAccess<runtime::Image> srcAccess(srcResult);
            cvsupport::Image::save("ThresholdTest_testInPlace0_src.png", srcAccess());
        }
        
    } // cvimgproc
} // stromx


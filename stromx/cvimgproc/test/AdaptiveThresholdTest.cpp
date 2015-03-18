#include "stromx/cvimgproc/test/AdaptiveThresholdTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/AdaptiveThreshold.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::AdaptiveThresholdTest);

namespace stromx
{
    namespace cvimgproc
    {
        void AdaptiveThresholdTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new AdaptiveThreshold);
        }
        
        void AdaptiveThresholdTest::tearDown()
        {
            delete m_operator;
        }
        
        void AdaptiveThresholdTest::testManual0()
        {
            m_operator->setParameter(AdaptiveThreshold::DATA_FLOW, runtime::Enum(AdaptiveThreshold::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            
            m_operator->setInputData(AdaptiveThreshold::SRC, src);
            m_operator->setInputData(AdaptiveThreshold::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(AdaptiveThreshold::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("AdaptiveThresholdTest_testManual0_dst.png", dstAccess());
        }
        
        void AdaptiveThresholdTest::testManual1()
        {
            m_operator->setParameter(AdaptiveThreshold::DATA_FLOW, runtime::Enum(AdaptiveThreshold::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Float64 maxval(128);
            runtime::Enum adaptiveMethod(1);
            runtime::Enum thresholdType(1);
            runtime::UInt32 blockSize(5);
            
            m_operator->setInputData(AdaptiveThreshold::SRC, src);
            m_operator->setInputData(AdaptiveThreshold::DST, src);
            m_operator->setParameter(AdaptiveThreshold::MAXVAL, maxval);
            m_operator->setParameter(AdaptiveThreshold::ADAPTIVE_METHOD, adaptiveMethod);
            m_operator->setParameter(AdaptiveThreshold::THRESHOLD_TYPE, thresholdType);
            m_operator->setParameter(AdaptiveThreshold::BLOCK_SIZE, blockSize);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(AdaptiveThreshold::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("AdaptiveThresholdTest_testManual1_dst.png", dstAccess());
        }
        
        void AdaptiveThresholdTest::testAllocate0()
        {
            m_operator->setParameter(AdaptiveThreshold::DATA_FLOW, runtime::Enum(AdaptiveThreshold::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Float64 maxval(200);
            runtime::Enum adaptiveMethod(1);
            runtime::Enum thresholdType(0);
            runtime::UInt32 blockSize(9);
            
            m_operator->setInputData(AdaptiveThreshold::SRC, src);
            m_operator->setParameter(AdaptiveThreshold::MAXVAL, maxval);
            m_operator->setParameter(AdaptiveThreshold::ADAPTIVE_METHOD, adaptiveMethod);
            m_operator->setParameter(AdaptiveThreshold::THRESHOLD_TYPE, thresholdType);
            m_operator->setParameter(AdaptiveThreshold::BLOCK_SIZE, blockSize);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(AdaptiveThreshold::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("AdaptiveThresholdTest_testAllocate0_dst.png", dstAccess());
        }
        
        void AdaptiveThresholdTest::testInPlace0()
        {
            m_operator->setParameter(AdaptiveThreshold::DATA_FLOW, runtime::Enum(AdaptiveThreshold::IN_PLACE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Float64 maxval(80);
            runtime::Enum adaptiveMethod(0);
            runtime::Enum thresholdType(1);
            runtime::UInt32 blockSize(7);
            
            m_operator->setInputData(AdaptiveThreshold::SRC, src);
            m_operator->setParameter(AdaptiveThreshold::MAXVAL, maxval);
            m_operator->setParameter(AdaptiveThreshold::ADAPTIVE_METHOD, adaptiveMethod);
            m_operator->setParameter(AdaptiveThreshold::THRESHOLD_TYPE, thresholdType);
            m_operator->setParameter(AdaptiveThreshold::BLOCK_SIZE, blockSize);
            
            runtime::DataContainer srcResult = m_operator->getOutputData(AdaptiveThreshold::SRC);
            
            runtime::ReadAccess<runtime::Image> srcAccess(srcResult);
            cvsupport::Image::save("AdaptiveThresholdTest_testInPlace0_src.png", srcAccess());
        }
        
    } // cvimgproc
} // stromx


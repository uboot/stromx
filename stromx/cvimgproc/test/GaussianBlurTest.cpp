#include "stromx/cvimgproc/test/GaussianBlurTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/GaussianBlur.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::GaussianBlurTest);

namespace stromx
{
    namespace cvimgproc
    {
        void GaussianBlurTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new GaussianBlur);
        }
        
        void GaussianBlurTest::tearDown()
        {
            delete m_operator;
        }
        
        void GaussianBlurTest::testManual0()
        {
            m_operator->setParameter(GaussianBlur::DATA_FLOW, runtime::Enum(GaussianBlur::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            runtime::UInt32 ksizex(3);
            runtime::UInt32 ksizey(5);
            runtime::Float64 sigmaX(1.5);
            runtime::Float64 sigmaY(2.5);
            
            m_operator->setInputData(GaussianBlur::SRC, src);
            m_operator->setInputData(GaussianBlur::DST, dst);
            m_operator->setParameter(GaussianBlur::KSIZEX, ksizex);
            m_operator->setParameter(GaussianBlur::KSIZEY, ksizey);
            m_operator->setParameter(GaussianBlur::SIGMA_X, sigmaX);
            m_operator->setParameter(GaussianBlur::SIGMA_Y, sigmaY);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(GaussianBlur::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("GaussianBlurTest_testManual0_dst.png", dstAccess());
        }
        
        void GaussianBlurTest::testManual1()
        {
            m_operator->setParameter(GaussianBlur::DATA_FLOW, runtime::Enum(GaussianBlur::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            
            m_operator->setInputData(GaussianBlur::SRC, src);
            m_operator->setInputData(GaussianBlur::DST, src);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(GaussianBlur::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("GaussianBlurTest_testManual1_dst.png", dstAccess());
        }
        
        void GaussianBlurTest::testAllocate0()
        {
            m_operator->setParameter(GaussianBlur::DATA_FLOW, runtime::Enum(GaussianBlur::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::UInt32 ksizex(3);
            runtime::UInt32 ksizey(5);
            runtime::Float64 sigmaX(-1);
            runtime::Float64 sigmaY(-1);
            
            m_operator->setInputData(GaussianBlur::SRC, src);
            m_operator->setParameter(GaussianBlur::KSIZEX, ksizex);
            m_operator->setParameter(GaussianBlur::KSIZEY, ksizey);
            m_operator->setParameter(GaussianBlur::SIGMA_X, sigmaX);
            m_operator->setParameter(GaussianBlur::SIGMA_Y, sigmaY);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(GaussianBlur::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("GaussianBlurTest_testAllocate0_dst.png", dstAccess());
        }
        
        void GaussianBlurTest::testInPlace0()
        {
            m_operator->setParameter(GaussianBlur::DATA_FLOW, runtime::Enum(GaussianBlur::IN_PLACE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::Float64 sigmaX(0);
            runtime::Float64 sigmaY(0);
            
            m_operator->setInputData(GaussianBlur::SRC, src);
            m_operator->setParameter(GaussianBlur::SIGMA_X, sigmaX);
            m_operator->setParameter(GaussianBlur::SIGMA_Y, sigmaY);
            
            runtime::DataContainer srcResult = m_operator->getOutputData(GaussianBlur::SRC);
            
            runtime::ReadAccess<runtime::Image> srcAccess(srcResult);
            cvsupport::Image::save("GaussianBlurTest_testInPlace0_src.png", srcAccess());
        }
        
    } // cvimgproc
} // stromx


#include "stromx/cvimgproc/test/CalcHist1DTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/CalcHist1D.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::CalcHist1DTest);

namespace stromx
{
    namespace cvimgproc
    {
        void CalcHist1DTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new CalcHist1D);
        }
        
        void CalcHist1DTest::tearDown()
        {
            delete m_operator;
        }
        
        void CalcHist1DTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("circle.png", cvsupport::Image::GRAYSCALE));
            runtime::Float32 histMin(0);
            runtime::Float32 histMax(256);
            runtime::UInt32 histSize(5);
            
            m_operator->setInputData(CalcHist1D::SRC, src);
            m_operator->setParameter(CalcHist1D::HIST_MIN, histMin);
            m_operator->setParameter(CalcHist1D::HIST_MAX, histMax);
            m_operator->setParameter(CalcHist1D::HIST_SIZE, histSize);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(CalcHist1D::DST);
            
            runtime::ReadAccess<runtime::Matrix> dstAccess(dstResult);
            cvsupport::Matrix::save("CalcHist1DTest_testAllocate0_dst.npy", dstAccess());
        }
        
        void CalcHist1DTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Float32 histMin(0);
            runtime::Float32 histMax(256);
            runtime::UInt32 histSize(20);
            
            m_operator->setInputData(CalcHist1D::SRC, src);
            m_operator->setParameter(CalcHist1D::HIST_MIN, histMin);
            m_operator->setParameter(CalcHist1D::HIST_MAX, histMax);
            m_operator->setParameter(CalcHist1D::HIST_SIZE, histSize);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(CalcHist1D::DST);
            
            runtime::ReadAccess<runtime::Matrix> dstAccess(dstResult);
            cvsupport::Matrix::save("CalcHist1DTest_testAllocate1_dst.npy", dstAccess());
        }
        
    } // cvimgproc
} // stromx


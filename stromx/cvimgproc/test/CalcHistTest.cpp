#include "stromx/cvimgproc/test/CalcHistTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/CalcHist.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::CalcHistTest);

namespace stromx
{
    namespace cvimgproc
    {
        void CalcHistTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new CalcHist);
        }
        
        void CalcHistTest::tearDown()
        {
            delete m_operator;
        }
        
        void CalcHistTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("circle.png", cvsupport::Image::GRAYSCALE));
            runtime::Float32 histMin(0);
            runtime::Float32 histMax(256);
            runtime::UInt32 histSize(5);
            
            m_operator->setInputData(CalcHist::SRC, src);
            m_operator->setParameter(CalcHist::HIST_MIN, histMin);
            m_operator->setParameter(CalcHist::HIST_MAX, histMax);
            m_operator->setParameter(CalcHist::HIST_SIZE, histSize);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(CalcHist::DST);
            
            runtime::ReadAccess dstAccess(dstResult);
            cvsupport::Matrix::save("CalcHistTest_testAllocate0_dst.npy", dstAccess.get<runtime::Matrix>());
        }
        
        void CalcHistTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Float32 histMin(0);
            runtime::Float32 histMax(256);
            runtime::UInt32 histSize(20);
            
            m_operator->setInputData(CalcHist::SRC, src);
            m_operator->setParameter(CalcHist::HIST_MIN, histMin);
            m_operator->setParameter(CalcHist::HIST_MAX, histMax);
            m_operator->setParameter(CalcHist::HIST_SIZE, histSize);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(CalcHist::DST);
            
            runtime::ReadAccess dstAccess(dstResult);
            cvsupport::Matrix::save("CalcHistTest_testAllocate1_dst.npy", dstAccess.get<runtime::Matrix>());
        }
        
    } // cvimgproc
} // stromx


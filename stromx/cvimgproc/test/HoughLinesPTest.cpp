#include "stromx/cvimgproc/test/HoughLinesPTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/HoughLinesP.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::HoughLinesPTest);

namespace stromx
{
    namespace cvimgproc
    {
        void HoughLinesPTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new HoughLinesP);
        }
        
        void HoughLinesPTest::tearDown()
        {
            delete m_operator;
        }
        
        void HoughLinesPTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("edges.png", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(HoughLinesP::SRC, src);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(HoughLinesP::DST);
            
            runtime::ReadAccess<runtime::Matrix> dstAccess(dstResult);
            cvsupport::Matrix::save("HoughLinesPTest_testAllocate0_dst.npy", dstAccess());
        }
        
    } // cvimgproc
} // stromx


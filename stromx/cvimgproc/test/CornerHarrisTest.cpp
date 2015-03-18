#include "stromx/cvimgproc/test/CornerHarrisTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/CornerHarris.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::CornerHarrisTest);

namespace stromx
{
    namespace cvimgproc
    {
        void CornerHarrisTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new CornerHarris);
        }
        
        void CornerHarrisTest::tearDown()
        {
            delete m_operator;
        }
        
        void CornerHarrisTest::testManual0()
        {
            m_operator->setParameter(CornerHarris::DATA_FLOW, runtime::Enum(CornerHarris::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(10000000));
            
            m_operator->setInputData(CornerHarris::SRC, src);
            m_operator->setInputData(CornerHarris::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(CornerHarris::DST);
            
            runtime::ReadAccess<runtime::Matrix> dstAccess(dstResult);
            cvsupport::Matrix::save("CornerHarrisTest_testManual0_dst.npy", dstAccess());
        }
        
        void CornerHarrisTest::testAllocate0()
        {
            m_operator->setParameter(CornerHarris::DATA_FLOW, runtime::Enum(CornerHarris::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(CornerHarris::SRC, src);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(CornerHarris::DST);
            
            runtime::ReadAccess<runtime::Matrix> dstAccess(dstResult);
            cvsupport::Matrix::save("CornerHarrisTest_testAllocate0_dst.npy", dstAccess());
        }
        
    } // cvimgproc
} // stromx


#include "stromx/cvimgproc/test/CornerMinEigenValTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/CornerMinEigenVal.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::CornerMinEigenValTest);

namespace stromx
{
    namespace cvimgproc
    {
        void CornerMinEigenValTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new CornerMinEigenVal);
        }
        
        void CornerMinEigenValTest::tearDown()
        {
            delete m_operator;
        }
        
        void CornerMinEigenValTest::testManual0()
        {
            m_operator->setParameter(CornerMinEigenVal::DATA_FLOW, runtime::Enum(CornerMinEigenVal::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(10000000));
            
            m_operator->setInputData(CornerMinEigenVal::SRC, src);
            m_operator->setInputData(CornerMinEigenVal::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(CornerMinEigenVal::DST);
            
            runtime::ReadAccess<runtime::Matrix> dstAccess(dstResult);
            cvsupport::Matrix::save("CornerMinEigenValTest_testManual0_dst.npy", dstAccess());
        }
        
        void CornerMinEigenValTest::testAllocate0()
        {
            m_operator->setParameter(CornerMinEigenVal::DATA_FLOW, runtime::Enum(CornerMinEigenVal::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(CornerMinEigenVal::SRC, src);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(CornerMinEigenVal::DST);
            
            runtime::ReadAccess<runtime::Matrix> dstAccess(dstResult);
            cvsupport::Matrix::save("CornerMinEigenValTest_testAllocate0_dst.npy", dstAccess());
        }
        
    } // cvimgproc
} // stromx


#include "stromx/cvimgproc/test/PreCornerDetectTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/PreCornerDetect.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::PreCornerDetectTest);

namespace stromx
{
    namespace cvimgproc
    {
        void PreCornerDetectTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new PreCornerDetect);
        }
        
        void PreCornerDetectTest::tearDown()
        {
            delete m_operator;
        }
        
        void PreCornerDetectTest::testManual0()
        {
            m_operator->setParameter(PreCornerDetect::DATA_FLOW, runtime::Enum(PreCornerDetect::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(10000000));
            
            m_operator->setInputData(PreCornerDetect::SRC, src);
            m_operator->setInputData(PreCornerDetect::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(PreCornerDetect::DST);
            
            runtime::ReadAccess<runtime::Matrix> dstAccess(dstResult);
            cvsupport::Matrix::save("PreCornerDetectTest_testManual0_dst.npy", dstAccess());
        }
        
        void PreCornerDetectTest::testAllocate0()
        {
            m_operator->setParameter(PreCornerDetect::DATA_FLOW, runtime::Enum(PreCornerDetect::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::UInt32 ksize(5);
            runtime::Enum borderType(2);
            
            m_operator->setInputData(PreCornerDetect::SRC, src);
            m_operator->setParameter(PreCornerDetect::KSIZE, ksize);
            m_operator->setParameter(PreCornerDetect::BORDER_TYPE, borderType);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(PreCornerDetect::DST);
            
            runtime::ReadAccess<runtime::Matrix> dstAccess(dstResult);
            cvsupport::Matrix::save("PreCornerDetectTest_testAllocate0_dst.npy", dstAccess());
        }
        
    } // cvimgproc
} // stromx


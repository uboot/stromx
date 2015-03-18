#include "stromx/cvimgproc/test/LaplacianTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/Laplacian.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::LaplacianTest);

namespace stromx
{
    namespace cvimgproc
    {
        void LaplacianTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Laplacian);
        }
        
        void LaplacianTest::tearDown()
        {
            delete m_operator;
        }
        
        void LaplacianTest::testManual0()
        {
            m_operator->setParameter(Laplacian::DATA_FLOW, runtime::Enum(Laplacian::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            runtime::Enum ddepth(0);
            runtime::UInt32 ksize(3);
            
            m_operator->setInputData(Laplacian::SRC, src);
            m_operator->setInputData(Laplacian::DST, dst);
            m_operator->setParameter(Laplacian::DDEPTH, ddepth);
            m_operator->setParameter(Laplacian::KSIZE, ksize);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Laplacian::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("LaplacianTest_testManual0_dst.png", dstAccess());
        }
        
        void LaplacianTest::testManual1()
        {
            m_operator->setParameter(Laplacian::DATA_FLOW, runtime::Enum(Laplacian::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            runtime::Enum ddepth(1);
            runtime::UInt32 ksize(3);
            runtime::Float64 scale(1);
            runtime::Float64 delta(0);
            
            m_operator->setInputData(Laplacian::SRC, src);
            m_operator->setInputData(Laplacian::DST, dst);
            m_operator->setParameter(Laplacian::DDEPTH, ddepth);
            m_operator->setParameter(Laplacian::KSIZE, ksize);
            m_operator->setParameter(Laplacian::SCALE, scale);
            m_operator->setParameter(Laplacian::DELTA, delta);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Laplacian::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("LaplacianTest_testManual1_dst.png", dstAccess());
        }
        
        void LaplacianTest::testAllocate0()
        {
            m_operator->setParameter(Laplacian::DATA_FLOW, runtime::Enum(Laplacian::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Enum ddepth(2);
            runtime::UInt32 ksize(5);
            runtime::Float64 scale(100);
            runtime::Float64 delta(1000);
            
            m_operator->setInputData(Laplacian::SRC, src);
            m_operator->setParameter(Laplacian::DDEPTH, ddepth);
            m_operator->setParameter(Laplacian::KSIZE, ksize);
            m_operator->setParameter(Laplacian::SCALE, scale);
            m_operator->setParameter(Laplacian::DELTA, delta);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Laplacian::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("LaplacianTest_testAllocate0_dst.png", dstAccess());
        }
        
        void LaplacianTest::testAllocate1()
        {
            m_operator->setParameter(Laplacian::DATA_FLOW, runtime::Enum(Laplacian::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::Enum ddepth(2);
            runtime::UInt32 ksize(7);
            runtime::Float64 scale(50);
            runtime::Float64 delta(500);
            
            m_operator->setInputData(Laplacian::SRC, src);
            m_operator->setParameter(Laplacian::DDEPTH, ddepth);
            m_operator->setParameter(Laplacian::KSIZE, ksize);
            m_operator->setParameter(Laplacian::SCALE, scale);
            m_operator->setParameter(Laplacian::DELTA, delta);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Laplacian::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("LaplacianTest_testAllocate1_dst.png", dstAccess());
        }
        
    } // cvimgproc
} // stromx


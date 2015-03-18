#include "stromx/cvimgproc/test/ScharrTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/Scharr.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::ScharrTest);

namespace stromx
{
    namespace cvimgproc
    {
        void ScharrTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Scharr);
        }
        
        void ScharrTest::tearDown()
        {
            delete m_operator;
        }
        
        void ScharrTest::testManual0()
        {
            m_operator->setParameter(Scharr::DATA_FLOW, runtime::Enum(Scharr::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            runtime::Enum ddepth(0);
            runtime::UInt32 dx(0);
            runtime::UInt32 dy(1);
            runtime::Float64 scale(1);
            runtime::Float64 delta(0);
            
            m_operator->setInputData(Scharr::SRC, src);
            m_operator->setInputData(Scharr::DST, dst);
            m_operator->setParameter(Scharr::DDEPTH, ddepth);
            m_operator->setParameter(Scharr::DX, dx);
            m_operator->setParameter(Scharr::DY, dy);
            m_operator->setParameter(Scharr::SCALE, scale);
            m_operator->setParameter(Scharr::DELTA, delta);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Scharr::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("ScharrTest_testManual0_dst.png", dstAccess());
        }
        
        void ScharrTest::testManual1()
        {
            m_operator->setParameter(Scharr::DATA_FLOW, runtime::Enum(Scharr::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            runtime::Enum ddepth(1);
            runtime::UInt32 dx(1);
            runtime::UInt32 dy(0);
            runtime::Float64 scale(1);
            runtime::Float64 delta(0);
            
            m_operator->setInputData(Scharr::SRC, src);
            m_operator->setInputData(Scharr::DST, dst);
            m_operator->setParameter(Scharr::DDEPTH, ddepth);
            m_operator->setParameter(Scharr::DX, dx);
            m_operator->setParameter(Scharr::DY, dy);
            m_operator->setParameter(Scharr::SCALE, scale);
            m_operator->setParameter(Scharr::DELTA, delta);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Scharr::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("ScharrTest_testManual1_dst.png", dstAccess());
        }
        
        void ScharrTest::testAllocate0()
        {
            m_operator->setParameter(Scharr::DATA_FLOW, runtime::Enum(Scharr::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::Enum ddepth(0);
            runtime::Float64 scale(2);
            
            m_operator->setInputData(Scharr::SRC, src);
            m_operator->setParameter(Scharr::DDEPTH, ddepth);
            m_operator->setParameter(Scharr::SCALE, scale);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Scharr::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("ScharrTest_testAllocate0_dst.png", dstAccess());
        }
        
        void ScharrTest::testAllocate1()
        {
            m_operator->setParameter(Scharr::DATA_FLOW, runtime::Enum(Scharr::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Enum ddepth(2);
            runtime::UInt32 dx(0);
            runtime::UInt32 dy(1);
            runtime::Float64 scale(100);
            
            m_operator->setInputData(Scharr::SRC, src);
            m_operator->setParameter(Scharr::DDEPTH, ddepth);
            m_operator->setParameter(Scharr::DX, dx);
            m_operator->setParameter(Scharr::DY, dy);
            m_operator->setParameter(Scharr::SCALE, scale);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Scharr::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("ScharrTest_testAllocate1_dst.png", dstAccess());
        }
        
    } // cvimgproc
} // stromx


#include "stromx/cvimgproc/test/SobelTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/Sobel.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::SobelTest);

namespace stromx
{
    namespace cvimgproc
    {
        void SobelTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Sobel);
        }
        
        void SobelTest::tearDown()
        {
            delete m_operator;
        }
        
        void SobelTest::testManual0()
        {
            m_operator->setParameter(Sobel::DATA_FLOW, runtime::Enum(Sobel::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            runtime::Enum ddepth(0);
            runtime::UInt32 dx(1);
            runtime::UInt32 dy(1);
            runtime::UInt32 ksize(1);
            runtime::Float64 scale(1);
            runtime::Float64 delta(0);
            
            m_operator->setInputData(Sobel::SRC, src);
            m_operator->setInputData(Sobel::DST, dst);
            m_operator->setParameter(Sobel::DDEPTH, ddepth);
            m_operator->setParameter(Sobel::DX, dx);
            m_operator->setParameter(Sobel::DY, dy);
            m_operator->setParameter(Sobel::KSIZE, ksize);
            m_operator->setParameter(Sobel::SCALE, scale);
            m_operator->setParameter(Sobel::DELTA, delta);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Sobel::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("SobelTest_testManual0_dst.png", dstAccess());
        }
        
        void SobelTest::testManual1()
        {
            m_operator->setParameter(Sobel::DATA_FLOW, runtime::Enum(Sobel::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            runtime::Enum ddepth(1);
            runtime::UInt32 dx(2);
            runtime::UInt32 dy(0);
            runtime::UInt32 ksize(3);
            runtime::Float64 scale(1);
            runtime::Float64 delta(0);
            
            m_operator->setInputData(Sobel::SRC, src);
            m_operator->setInputData(Sobel::DST, dst);
            m_operator->setParameter(Sobel::DDEPTH, ddepth);
            m_operator->setParameter(Sobel::DX, dx);
            m_operator->setParameter(Sobel::DY, dy);
            m_operator->setParameter(Sobel::KSIZE, ksize);
            m_operator->setParameter(Sobel::SCALE, scale);
            m_operator->setParameter(Sobel::DELTA, delta);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Sobel::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("SobelTest_testManual1_dst.png", dstAccess());
        }
        
        void SobelTest::testAllocate0()
        {
            m_operator->setParameter(Sobel::DATA_FLOW, runtime::Enum(Sobel::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::Enum ddepth(0);
            runtime::UInt32 dy(2);
            runtime::UInt32 ksize(5);
            runtime::Float64 scale(2);
            
            m_operator->setInputData(Sobel::SRC, src);
            m_operator->setParameter(Sobel::DDEPTH, ddepth);
            m_operator->setParameter(Sobel::DY, dy);
            m_operator->setParameter(Sobel::KSIZE, ksize);
            m_operator->setParameter(Sobel::SCALE, scale);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Sobel::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("SobelTest_testAllocate0_dst.png", dstAccess());
        }
        
        void SobelTest::testAllocate1()
        {
            m_operator->setParameter(Sobel::DATA_FLOW, runtime::Enum(Sobel::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Enum ddepth(2);
            runtime::Float64 scale(100);
            
            m_operator->setInputData(Sobel::SRC, src);
            m_operator->setParameter(Sobel::DDEPTH, ddepth);
            m_operator->setParameter(Sobel::SCALE, scale);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Sobel::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("SobelTest_testAllocate1_dst.png", dstAccess());
        }
        
    } // cvimgproc
} // stromx


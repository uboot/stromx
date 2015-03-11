#include "stromx/cvimgproc/test/ResizeTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/Resize.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::ResizeTest);

namespace stromx
{
    namespace cvimgproc
    {
        void ResizeTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Resize);
        }
        
        void ResizeTest::tearDown()
        {
            delete m_operator;
        }
        
        void ResizeTest::testManual0()
        {
            m_operator->setParameter(Resize::DATA_FLOW, runtime::Enum(Resize::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            
            m_operator->setInputData(Resize::SRC, src);
            m_operator->setInputData(Resize::DST, dst);
            
            runtime::DataContainer result = m_operator->getOutputData(Resize::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("ResizeTest_testManual0.png", access());
        }
        
        void ResizeTest::testManual1()
        {
            m_operator->setParameter(Resize::DATA_FLOW, runtime::Enum(Resize::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            runtime::UInt32 dsizex(100);
            runtime::UInt32 dsizey(200);
            runtime::Float64 fx(0);
            
            m_operator->setInputData(Resize::SRC, src);
            m_operator->setInputData(Resize::DST, dst);
            m_operator->setParameter(Resize::DSIZEX, dsizex);
            m_operator->setParameter(Resize::DSIZEY, dsizey);
            m_operator->setParameter(Resize::FX, fx);
            
            runtime::DataContainer result = m_operator->getOutputData(Resize::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("ResizeTest_testManual1.png", access());
        }
        
        void ResizeTest::testManual2()
        {
            m_operator->setParameter(Resize::DATA_FLOW, runtime::Enum(Resize::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            runtime::UInt32 dsizex(100);
            runtime::UInt32 dsizey(200);
            runtime::Float64 fx(0.5);
            runtime::Float64 fy(0.3);
            
            m_operator->setInputData(Resize::SRC, src);
            m_operator->setInputData(Resize::DST, dst);
            m_operator->setParameter(Resize::DSIZEX, dsizex);
            m_operator->setParameter(Resize::DSIZEY, dsizey);
            m_operator->setParameter(Resize::FX, fx);
            m_operator->setParameter(Resize::FY, fy);
            
            runtime::DataContainer result = m_operator->getOutputData(Resize::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("ResizeTest_testManual2.png", access());
        }
        
        void ResizeTest::testAllocate0()
        {
            m_operator->setParameter(Resize::DATA_FLOW, runtime::Enum(Resize::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Float64 fx(0.5);
            runtime::Float64 fy(0.3);
            
            m_operator->setInputData(Resize::SRC, src);
            m_operator->setParameter(Resize::FX, fx);
            m_operator->setParameter(Resize::FY, fy);
            
            runtime::DataContainer result = m_operator->getOutputData(Resize::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("ResizeTest_testAllocate0.png", access());
        }
        
    } // cvimgproc
} // stromx


#include "stromx/cvimgproc/test/DilateTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/Dilate.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::DilateTest);

namespace stromx
{
    namespace cvimgproc
    {
        void DilateTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Dilate);
        }
        
        void DilateTest::tearDown()
        {
            delete m_operator;
        }
        
        void DilateTest::testManual0()
        {
            m_operator->setParameter(Dilate::DATA_FLOW, runtime::Enum(Dilate::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            runtime::UInt32 ksizex(3);
            runtime::UInt32 ksizey(4);
            runtime::Enum shape(1);
            runtime::UInt32 iterations(2);
            
            m_operator->setInputData(Dilate::SRC, src);
            m_operator->setInputData(Dilate::DST, dst);
            m_operator->setParameter(Dilate::KSIZEX, ksizex);
            m_operator->setParameter(Dilate::KSIZEY, ksizey);
            m_operator->setParameter(Dilate::SHAPE, shape);
            m_operator->setParameter(Dilate::ITERATIONS, iterations);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Dilate::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("DilateTest_testManual0_dst.png", dstAccess());
        }
        
        void DilateTest::testManual1()
        {
            m_operator->setParameter(Dilate::DATA_FLOW, runtime::Enum(Dilate::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            
            m_operator->setInputData(Dilate::SRC, src);
            m_operator->setInputData(Dilate::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Dilate::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("DilateTest_testManual1_dst.png", dstAccess());
        }
        
        void DilateTest::testAllocate0()
        {
            m_operator->setParameter(Dilate::DATA_FLOW, runtime::Enum(Dilate::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            
            m_operator->setInputData(Dilate::SRC, src);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Dilate::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("DilateTest_testAllocate0_dst.png", dstAccess());
        }
        
        void DilateTest::testInPlace0()
        {
            m_operator->setParameter(Dilate::DATA_FLOW, runtime::Enum(Dilate::IN_PLACE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Enum shape(2);
            
            m_operator->setInputData(Dilate::SRC, src);
            m_operator->setParameter(Dilate::SHAPE, shape);
            
            runtime::DataContainer srcResult = m_operator->getOutputData(Dilate::SRC);
            
            runtime::ReadAccess<runtime::Image> srcAccess(srcResult);
            cvsupport::Image::save("DilateTest_testInPlace0_src.png", srcAccess());
        }
        
    } // cvimgproc
} // stromx


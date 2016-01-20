#include "stromx/cvimgproc/test/EqualizeHistTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/EqualizeHist.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::EqualizeHistTest);

namespace stromx
{
    namespace cvimgproc
    {
        void EqualizeHistTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new EqualizeHist);
        }
        
        void EqualizeHistTest::tearDown()
        {
            delete m_operator;
        }
        
        void EqualizeHistTest::testManual0()
        {
            m_operator->setParameter(EqualizeHist::DATA_FLOW, runtime::Enum(EqualizeHist::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            
            m_operator->setInputData(EqualizeHist::SRC, src);
            m_operator->setInputData(EqualizeHist::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(EqualizeHist::DST);
            
            runtime::ReadAccess dstAccess(dstResult);
            cvsupport::Image::save("EqualizeHistTest_testManual0_dst.png", dstAccess.get<runtime::Image>());
        }
        
        void EqualizeHistTest::testManual1()
        {
            m_operator->setParameter(EqualizeHist::DATA_FLOW, runtime::Enum(EqualizeHist::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(EqualizeHist::SRC, src);
            m_operator->setInputData(EqualizeHist::DST, src);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(EqualizeHist::DST);
            
            runtime::ReadAccess dstAccess(dstResult);
            cvsupport::Image::save("EqualizeHistTest_testManual1_dst.png", dstAccess.get<runtime::Image>());
        }
        
        void EqualizeHistTest::testAllocate0()
        {
            m_operator->setParameter(EqualizeHist::DATA_FLOW, runtime::Enum(EqualizeHist::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(EqualizeHist::SRC, src);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(EqualizeHist::DST);
            
            runtime::ReadAccess dstAccess(dstResult);
            cvsupport::Image::save("EqualizeHistTest_testAllocate0_dst.png", dstAccess.get<runtime::Image>());
        }
        
        void EqualizeHistTest::testInPlace0()
        {
            m_operator->setParameter(EqualizeHist::DATA_FLOW, runtime::Enum(EqualizeHist::IN_PLACE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(EqualizeHist::SRC, src);
            
            runtime::DataContainer srcResult = m_operator->getOutputData(EqualizeHist::SRC);
            
            runtime::ReadAccess srcAccess(srcResult);
            cvsupport::Image::save("EqualizeHistTest_testInPlace0_src.png", srcAccess.get<runtime::Image>());
        }
        
    } // cvimgproc
} // stromx


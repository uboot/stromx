#include "stromx/cvimgproc/test/MedianBlurTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/MedianBlur.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::MedianBlurTest);

namespace stromx
{
    namespace cvimgproc
    {
        void MedianBlurTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new MedianBlur);
        }
        
        void MedianBlurTest::tearDown()
        {
            delete m_operator;
        }
        
        void MedianBlurTest::testManual0()
        {
            m_operator->setParameter(MedianBlur::DATA_FLOW, runtime::Enum(MedianBlur::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            runtime::UInt32 ksize(3);
            
            m_operator->setInputData(MedianBlur::SRC, src);
            m_operator->setInputData(MedianBlur::DST, dst);
            m_operator->setParameter(MedianBlur::KSIZE, ksize);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(MedianBlur::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("MedianBlurTest_testManual0_dst.png", dstAccess());
        }
        
        void MedianBlurTest::testManual1()
        {
            m_operator->setParameter(MedianBlur::DATA_FLOW, runtime::Enum(MedianBlur::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::UInt32 ksize(5);
            
            m_operator->setInputData(MedianBlur::SRC, src);
            m_operator->setInputData(MedianBlur::DST, src);
            m_operator->setParameter(MedianBlur::KSIZE, ksize);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(MedianBlur::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("MedianBlurTest_testManual1_dst.png", dstAccess());
        }
        
        void MedianBlurTest::testAllocate0()
        {
            m_operator->setParameter(MedianBlur::DATA_FLOW, runtime::Enum(MedianBlur::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(MedianBlur::SRC, src);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(MedianBlur::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("MedianBlurTest_testAllocate0_dst.png", dstAccess());
        }
        
        void MedianBlurTest::testInPlace0()
        {
            m_operator->setParameter(MedianBlur::DATA_FLOW, runtime::Enum(MedianBlur::IN_PLACE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            
            m_operator->setInputData(MedianBlur::SRC, src);
            
            runtime::DataContainer srcResult = m_operator->getOutputData(MedianBlur::SRC);
            
            runtime::ReadAccess<runtime::Image> srcAccess(srcResult);
            cvsupport::Image::save("MedianBlurTest_testInPlace0_src.png", srcAccess());
        }
        
    } // cvimgproc
} // stromx


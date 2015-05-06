#include "stromx/cvimgproc/test/ExtractRectangleTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/ExtractRectangle.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::ExtractRectangleTest);

namespace stromx
{
    namespace cvimgproc
    {
        void ExtractRectangleTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new ExtractRectangle);
        }
        
        void ExtractRectangleTest::tearDown()
        {
            delete m_operator;
        }
        
        void ExtractRectangleTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer rect(new cvsupport::Matrix("rotated_rect.npy"));
            
            m_operator->setInputData(ExtractRectangle::SRC, src);
            m_operator->setInputData(ExtractRectangle::RECT, rect);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(ExtractRectangle::DST);
            
            runtime::ReadAccess dstAccess(dstResult);
            cvsupport::Image::save("ExtractRectangleTest_testAllocate0_dst.png", dstAccess.get<runtime::Image>());
        }
        
        void ExtractRectangleTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer rect(new cvsupport::Matrix("rotated_rect_top_right.npy"));
            
            m_operator->setInputData(ExtractRectangle::SRC, src);
            m_operator->setInputData(ExtractRectangle::RECT, rect);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(ExtractRectangle::DST);
            
            runtime::ReadAccess dstAccess(dstResult);
            cvsupport::Image::save("ExtractRectangleTest_testAllocate1_dst.png", dstAccess.get<runtime::Image>());
        }
        
        void ExtractRectangleTest::testAllocate2()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer rect(new cvsupport::Matrix("rotated_rect_bottom_left.npy"));
            
            m_operator->setInputData(ExtractRectangle::SRC, src);
            m_operator->setInputData(ExtractRectangle::RECT, rect);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(ExtractRectangle::DST);
            
            runtime::ReadAccess dstAccess(dstResult);
            cvsupport::Image::save("ExtractRectangleTest_testAllocate2_dst.png", dstAccess.get<runtime::Image>());
        }
        
    } // cvimgproc
} // stromx


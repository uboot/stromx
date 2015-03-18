#include "stromx/cvimgproc/test/MinAreaRectTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/MinAreaRect.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::MinAreaRectTest);

namespace stromx
{
    namespace cvimgproc
    {
        void MinAreaRectTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new MinAreaRect);
        }
        
        void MinAreaRectTest::tearDown()
        {
            delete m_operator;
        }
        
        void MinAreaRectTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer points(new cvsupport::Matrix("points_i32.npy"));
            
            m_operator->setInputData(MinAreaRect::POINTS, points);
            
            runtime::DataContainer rectResult = m_operator->getOutputData(MinAreaRect::RECT);
            
            runtime::ReadAccess<runtime::Matrix> rectAccess(rectResult);
            cvsupport::Matrix::save("MinAreaRectTest_testAllocate0_rect.npy", rectAccess());
        }
        
        void MinAreaRectTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer points(new cvsupport::Matrix("points_f32.npy"));
            
            m_operator->setInputData(MinAreaRect::POINTS, points);
            
            runtime::DataContainer rectResult = m_operator->getOutputData(MinAreaRect::RECT);
            
            runtime::ReadAccess<runtime::Matrix> rectAccess(rectResult);
            cvsupport::Matrix::save("MinAreaRectTest_testAllocate1_rect.npy", rectAccess());
        }
        
    } // cvimgproc
} // stromx


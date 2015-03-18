#include "stromx/cvimgproc/test/BoundingRectTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/BoundingRect.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::BoundingRectTest);

namespace stromx
{
    namespace cvimgproc
    {
        void BoundingRectTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new BoundingRect);
        }
        
        void BoundingRectTest::tearDown()
        {
            delete m_operator;
        }
        
        void BoundingRectTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer points(new cvsupport::Matrix("points_i32.npy"));
            
            m_operator->setInputData(BoundingRect::POINTS, points);
            
            runtime::DataContainer rectResult = m_operator->getOutputData(BoundingRect::RECT);
            
            runtime::ReadAccess<runtime::Matrix> rectAccess(rectResult);
            cvsupport::Matrix::save("BoundingRectTest_testAllocate0_rect.npy", rectAccess());
        }
        
        void BoundingRectTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer points(new cvsupport::Matrix("points_f32.npy"));
            
            m_operator->setInputData(BoundingRect::POINTS, points);
            
            runtime::DataContainer rectResult = m_operator->getOutputData(BoundingRect::RECT);
            
            runtime::ReadAccess<runtime::Matrix> rectAccess(rectResult);
            cvsupport::Matrix::save("BoundingRectTest_testAllocate1_rect.npy", rectAccess());
        }
        
    } // cvimgproc
} // stromx


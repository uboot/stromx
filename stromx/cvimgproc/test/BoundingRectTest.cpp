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
            
            runtime::DataContainer points(new cvsupport::Matrix("contour_1.npy"));
            
            m_operator->setInputData(BoundingRect::POINTS, points);
            
            runtime::DataContainer result = m_operator->getOutputData(BoundingRect::RECT);
            
            runtime::ReadAccess<runtime::Matrix> access(result);
            cvsupport::Matrix::save("BoundingRectTest_testAllocate0.npy", access());
        }
        
        void BoundingRectTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer points(new cvsupport::Matrix("contour_f32.npy"));
            
            m_operator->setInputData(BoundingRect::POINTS, points);
            
            runtime::DataContainer result = m_operator->getOutputData(BoundingRect::RECT);
            
            runtime::ReadAccess<runtime::Matrix> access(result);
            cvsupport::Matrix::save("BoundingRectTest_testAllocate1.npy", access());
        }
        
    }
}


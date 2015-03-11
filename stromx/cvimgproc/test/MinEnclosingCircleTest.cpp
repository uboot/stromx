#include "stromx/cvimgproc/test/MinEnclosingCircleTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/MinEnclosingCircle.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::MinEnclosingCircleTest);

namespace stromx
{
    namespace cvimgproc
    {
        void MinEnclosingCircleTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new MinEnclosingCircle);
        }
        
        void MinEnclosingCircleTest::tearDown()
        {
            delete m_operator;
        }
        
        void MinEnclosingCircleTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer points(new cvsupport::Matrix("points_i32.npy"));
            
            m_operator->setInputData(MinEnclosingCircle::POINTS, points);
            
            runtime::DataContainer result = m_operator->getOutputData(MinEnclosingCircle::CIRCLE);
            
            runtime::ReadAccess<runtime::Matrix> access(result);
            cvsupport::Matrix::save("MinEnclosingCircleTest_testAllocate0.npy", access());
        }
        
        void MinEnclosingCircleTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer points(new cvsupport::Matrix("points_f32.npy"));
            
            m_operator->setInputData(MinEnclosingCircle::POINTS, points);
            
            runtime::DataContainer result = m_operator->getOutputData(MinEnclosingCircle::CIRCLE);
            
            runtime::ReadAccess<runtime::Matrix> access(result);
            cvsupport::Matrix::save("MinEnclosingCircleTest_testAllocate1.npy", access());
        }
        
    } // cvimgproc
} // stromx


#include "stromx/cvimgproc/test/FitEllipseTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/FitEllipse.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::FitEllipseTest);

namespace stromx
{
    namespace cvimgproc
    {
        void FitEllipseTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new FitEllipse);
        }
        
        void FitEllipseTest::tearDown()
        {
            delete m_operator;
        }
        
        void FitEllipseTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer points(new cvsupport::Matrix("points_i32.npy"));
            
            m_operator->setInputData(FitEllipse::POINTS, points);
            
            runtime::DataContainer result = m_operator->getOutputData(FitEllipse::ELLIPSE);
            
            runtime::ReadAccess<runtime::Matrix> access(result);
            cvsupport::Matrix::save("FitEllipseTest_testAllocate0.npy", access());
        }
        
        void FitEllipseTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer points(new cvsupport::Matrix("points_f32.npy"));
            
            m_operator->setInputData(FitEllipse::POINTS, points);
            
            runtime::DataContainer result = m_operator->getOutputData(FitEllipse::ELLIPSE);
            
            runtime::ReadAccess<runtime::Matrix> access(result);
            cvsupport::Matrix::save("FitEllipseTest_testAllocate1.npy", access());
        }
        
    } // cvimgproc
} // stromx


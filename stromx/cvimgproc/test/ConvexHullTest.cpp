#include "stromx/cvimgproc/test/ConvexHullTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/ConvexHull.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::ConvexHullTest);

namespace stromx
{
    namespace cvimgproc
    {
        void ConvexHullTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new ConvexHull);
        }
        
        void ConvexHullTest::tearDown()
        {
            delete m_operator;
        }
        
        void ConvexHullTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer curve(new cvsupport::Matrix("non_convex_f32.npy"));
            
            m_operator->setInputData(ConvexHull::CURVE, curve);
            
            runtime::DataContainer outCurveResult = m_operator->getOutputData(ConvexHull::OUT_CURVE);
            
            runtime::ReadAccess<runtime::Matrix> outCurveAccess(outCurveResult);
            cvsupport::Matrix::save("ConvexHullTest_testAllocate0_outCurve.npy", outCurveAccess());
        }
        
        void ConvexHullTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer curve(new cvsupport::Matrix("points_i32.npy"));
            
            m_operator->setInputData(ConvexHull::CURVE, curve);
            
            runtime::DataContainer outCurveResult = m_operator->getOutputData(ConvexHull::OUT_CURVE);
            
            runtime::ReadAccess<runtime::Matrix> outCurveAccess(outCurveResult);
            cvsupport::Matrix::save("ConvexHullTest_testAllocate1_outCurve.npy", outCurveAccess());
        }
        
    } // cvimgproc
} // stromx


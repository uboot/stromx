#include "stromx/cvimgproc/test/ApproxPolyDPTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/ApproxPolyDP.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::ApproxPolyDPTest);

namespace stromx
{
    namespace cvimgproc
    {
        void ApproxPolyDPTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new ApproxPolyDP);
        }
        
        void ApproxPolyDPTest::tearDown()
        {
            delete m_operator;
        }
        
        void ApproxPolyDPTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer curve(new cvsupport::Matrix("contour_1.npy"));
            
            m_operator->setInputData(ApproxPolyDP::CURVE, curve);
            
            runtime::DataContainer outCurveResult = m_operator->getOutputData(ApproxPolyDP::OUT_CURVE);
            
            runtime::ReadAccess<runtime::Matrix> outCurveAccess(outCurveResult);
            cvsupport::Matrix::save("ApproxPolyDPTest_testAllocate0_outCurve.npy", outCurveAccess());
        }
        
        void ApproxPolyDPTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer curve(new cvsupport::Matrix("contour_f32.npy"));
            runtime::Float64 epsilon(5.0);
            
            m_operator->setInputData(ApproxPolyDP::CURVE, curve);
            m_operator->setParameter(ApproxPolyDP::EPSILON, epsilon);
            
            runtime::DataContainer outCurveResult = m_operator->getOutputData(ApproxPolyDP::OUT_CURVE);
            
            runtime::ReadAccess<runtime::Matrix> outCurveAccess(outCurveResult);
            cvsupport::Matrix::save("ApproxPolyDPTest_testAllocate1_outCurve.npy", outCurveAccess());
        }
        
    } // cvimgproc
} // stromx


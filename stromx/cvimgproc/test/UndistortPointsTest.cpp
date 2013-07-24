#include "stromx/cvimgproc/test/UndistortPointsTest.h"

#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/UndistortPoints.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::UndistortPointsTest);

namespace stromx
{
    namespace cvimgproc
    {
        void UndistortPointsTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new UndistortPoints);
        }
        
        void UndistortPointsTest::tearDown()
        {
            delete m_operator;
        }
        
        void UndistortPointsTest::testAllocate0()
        {
            m_operator->setParameter(UndistortPoints::DATA_FLOW, runtime::Enum(UndistortPoints::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Matrix("points_2d.npy"));
            cvsupport::Matrix cameraMatrix("camera_matrix.npy");
            cvsupport::Matrix distCoeffs("dist_coeffs.npy");
            
            m_operator->setInputData(UndistortPoints::SRC, src);
            m_operator->setParameter(UndistortPoints::CAMERA_MATRIX, cameraMatrix);
            m_operator->setParameter(UndistortPoints::DIST_COEFFS, distCoeffs);
            
            runtime::DataContainer result = m_operator->getOutputData(UndistortPoints::DST);
            
            runtime::ReadAccess<runtime::Matrix> access(result);
            cvsupport::Matrix::save("UndistortPointsTest_testAllocate0.npy", access());
        }
        
        void UndistortPointsTest::testAllocate1()
        {
            m_operator->setParameter(UndistortPoints::DATA_FLOW, runtime::Enum(UndistortPoints::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Matrix("points_2d.npy"));
            
            m_operator->setInputData(UndistortPoints::SRC, src);
            
            runtime::DataContainer result = m_operator->getOutputData(UndistortPoints::DST);
            
            runtime::ReadAccess<runtime::Matrix> access(result);
            cvsupport::Matrix::save("UndistortPointsTest_testAllocate1.npy", access());
        }
        
    }
}


#include "stromx/cvcalib3d/test/SolvePnPTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvcalib3d/SolvePnP.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvcalib3d::SolvePnPTest);

namespace stromx
{
    namespace cvcalib3d
    {
        void SolvePnPTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new SolvePnP);
        }
        
        void SolvePnPTest::tearDown()
        {
            delete m_operator;
        }
        
        void SolvePnPTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer objectPoints(new cvsupport::Matrix("chess_corners_3d.npy"));
            runtime::DataContainer imagePoints(new cvsupport::Matrix("chess_corners_1.npy"));
            cvsupport::Matrix cameraMatrix("camera_matrix_64f.npy");
            cvsupport::Matrix distCoeffs("dist_coeffs_32f.npy");
            
            m_operator->setInputData(SolvePnP::OBJECT_POINTS, objectPoints);
            m_operator->setInputData(SolvePnP::IMAGE_POINTS, imagePoints);
            m_operator->setParameter(SolvePnP::CAMERA_MATRIX, cameraMatrix);
            m_operator->setParameter(SolvePnP::DIST_COEFFS, distCoeffs);
            
            runtime::DataContainer rvecResult = m_operator->getOutputData(SolvePnP::RVEC);
            runtime::DataContainer tvecResult = m_operator->getOutputData(SolvePnP::TVEC);
            
            runtime::ReadAccess<runtime::Matrix> rvecAccess(rvecResult);
            cvsupport::Matrix::save("SolvePnPTest_testAllocate0_rvec.npy", rvecAccess());
            runtime::ReadAccess<runtime::Matrix> tvecAccess(tvecResult);
            cvsupport::Matrix::save("SolvePnPTest_testAllocate0_tvec.npy", tvecAccess());
        }
        
        void SolvePnPTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer objectPoints(new cvsupport::Matrix("chess_corners_3d.npy"));
            runtime::DataContainer imagePoints(new cvsupport::Matrix("chess_corners_1.npy"));
            cvsupport::Matrix cameraMatrix("camera_matrix_32f.npy");
            cvsupport::Matrix distCoeffs("dist_coeffs_64f.npy");
            
            m_operator->setInputData(SolvePnP::OBJECT_POINTS, objectPoints);
            m_operator->setInputData(SolvePnP::IMAGE_POINTS, imagePoints);
            m_operator->setParameter(SolvePnP::CAMERA_MATRIX, cameraMatrix);
            m_operator->setParameter(SolvePnP::DIST_COEFFS, distCoeffs);
            
            runtime::DataContainer rvecResult = m_operator->getOutputData(SolvePnP::RVEC);
            runtime::DataContainer tvecResult = m_operator->getOutputData(SolvePnP::TVEC);
            
            runtime::ReadAccess<runtime::Matrix> rvecAccess(rvecResult);
            cvsupport::Matrix::save("SolvePnPTest_testAllocate1_rvec.npy", rvecAccess());
            runtime::ReadAccess<runtime::Matrix> tvecAccess(tvecResult);
            cvsupport::Matrix::save("SolvePnPTest_testAllocate1_tvec.npy", tvecAccess());
        }
        
    } // cvcalib3d
} // stromx


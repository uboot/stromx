#include "stromx/cvcalib3d/test/CalibrateCameraTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvcalib3d/CalibrateCamera.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvcalib3d::CalibrateCameraTest);

namespace stromx
{
    namespace cvcalib3d
    {
        void CalibrateCameraTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new CalibrateCamera);
        }
        
        void CalibrateCameraTest::tearDown()
        {
            delete m_operator;
        }
        
        void CalibrateCameraTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::Data* objectPointsItem = 0;
            std::vector<runtime::Data*> objectPointsVector;
            objectPointsItem = new cvsupport::Matrix("chess_corners_3d.npy");
            objectPointsVector.push_back(objectPointsItem);
            objectPointsItem = new cvsupport::Matrix("chess_corners_3d.npy");
            objectPointsVector.push_back(objectPointsItem);
            objectPointsItem = new cvsupport::Matrix("chess_corners_3d.npy");
            objectPointsVector.push_back(objectPointsItem);
            objectPointsItem = new cvsupport::Matrix("chess_corners_3d.npy");
            objectPointsVector.push_back(objectPointsItem);
            objectPointsItem = new cvsupport::Matrix("chess_corners_3d.npy");
            objectPointsVector.push_back(objectPointsItem);
            objectPointsItem = new cvsupport::Matrix("chess_corners_3d.npy");
            objectPointsVector.push_back(objectPointsItem);
            objectPointsItem = new cvsupport::Matrix("chess_corners_3d.npy");
            objectPointsVector.push_back(objectPointsItem);
            runtime::DataContainer objectPoints(new runtime::List(objectPointsVector));
            
            runtime::Data* imagePointsItem = 0;
            std::vector<runtime::Data*> imagePointsVector;
            imagePointsItem = new cvsupport::Matrix("chess_corners_1.npy");
            imagePointsVector.push_back(imagePointsItem);
            imagePointsItem = new cvsupport::Matrix("chess_corners_2.npy");
            imagePointsVector.push_back(imagePointsItem);
            imagePointsItem = new cvsupport::Matrix("chess_corners_3.npy");
            imagePointsVector.push_back(imagePointsItem);
            imagePointsItem = new cvsupport::Matrix("chess_corners_4.npy");
            imagePointsVector.push_back(imagePointsItem);
            imagePointsItem = new cvsupport::Matrix("chess_corners_5.npy");
            imagePointsVector.push_back(imagePointsItem);
            imagePointsItem = new cvsupport::Matrix("chess_corners_6.npy");
            imagePointsVector.push_back(imagePointsItem);
            imagePointsItem = new cvsupport::Matrix("chess_corners_7.npy");
            imagePointsVector.push_back(imagePointsItem);
            runtime::DataContainer imagePoints(new runtime::List(imagePointsVector));
            
            runtime::UInt32 imageSizeX(320);
            runtime::UInt32 imageSizeY(240);
            
            m_operator->setInputData(CalibrateCamera::OBJECT_POINTS, objectPoints);
            m_operator->setInputData(CalibrateCamera::IMAGE_POINTS, imagePoints);
            m_operator->setParameter(CalibrateCamera::IMAGE_SIZE_X, imageSizeX);
            m_operator->setParameter(CalibrateCamera::IMAGE_SIZE_Y, imageSizeY);
            
            runtime::DataContainer cameraMatrixResult = m_operator->getOutputData(CalibrateCamera::CAMERA_MATRIX);
            runtime::DataContainer distCoeffsResult = m_operator->getOutputData(CalibrateCamera::DIST_COEFFS);
            
            runtime::ReadAccess<runtime::Matrix> cameraMatrixAccess(cameraMatrixResult);
            cvsupport::Matrix::save("CalibrateCameraTest_testAllocate0_cameraMatrix.npy", cameraMatrixAccess());
            runtime::ReadAccess<runtime::Matrix> distCoeffsAccess(distCoeffsResult);
            cvsupport::Matrix::save("CalibrateCameraTest_testAllocate0_distCoeffs.npy", distCoeffsAccess());
        }
        
    } // cvcalib3d
} // stromx


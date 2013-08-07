#include "stromx/cvimgproc/test/DistanceTransformTest.h"

#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/DistanceTransform.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::DistanceTransformTest);

namespace stromx
{
    namespace cvimgproc
    {
        void DistanceTransformTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new DistanceTransform);
        }
        
        void DistanceTransformTest::tearDown()
        {
            delete m_operator;
        }
        
        void DistanceTransformTest::testManual0()
        {
            m_operator->setParameter(DistanceTransform::DATA_FLOW, runtime::Enum(DistanceTransform::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("circle.png", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            runtime::UInt32 maskSize(5);
            
            m_operator->setInputData(DistanceTransform::SRC, src);
            m_operator->setInputData(DistanceTransform::DST, dst);
            m_operator->setParameter(DistanceTransform::MASK_SIZE, maskSize);
            
            runtime::DataContainer result = m_operator->getOutputData(DistanceTransform::DST);
            
            runtime::ReadAccess<runtime::Matrix> access(result);
            cvsupport::Matrix::save("DistanceTransformTest_testManual0.npy", access());
        }
        
        void DistanceTransformTest::testAllocate0()
        {
            m_operator->setParameter(DistanceTransform::DATA_FLOW, runtime::Enum(DistanceTransform::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("circle.png", cvsupport::Image::GRAYSCALE));
            runtime::Enum distanceType(1);
            runtime::UInt32 maskSize(3);
            
            m_operator->setInputData(DistanceTransform::SRC, src);
            m_operator->setParameter(DistanceTransform::DISTANCE_TYPE, distanceType);
            m_operator->setParameter(DistanceTransform::MASK_SIZE, maskSize);
            
            runtime::DataContainer result = m_operator->getOutputData(DistanceTransform::DST);
            
            runtime::ReadAccess<runtime::Matrix> access(result);
            cvsupport::Matrix::save("DistanceTransformTest_testAllocate0.npy", access());
        }
        
        void DistanceTransformTest::testAllocate1()
        {
            m_operator->setParameter(DistanceTransform::DATA_FLOW, runtime::Enum(DistanceTransform::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("circle.png", cvsupport::Image::GRAYSCALE));
            runtime::Enum distanceType(2);
            runtime::UInt32 maskSize(5);
            
            m_operator->setInputData(DistanceTransform::SRC, src);
            m_operator->setParameter(DistanceTransform::DISTANCE_TYPE, distanceType);
            m_operator->setParameter(DistanceTransform::MASK_SIZE, maskSize);
            
            runtime::DataContainer result = m_operator->getOutputData(DistanceTransform::DST);
            
            runtime::ReadAccess<runtime::Matrix> access(result);
            cvsupport::Matrix::save("DistanceTransformTest_testAllocate1.npy", access());
        }
        
    }
}


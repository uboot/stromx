#include "stromx/imgproc/test/MorphologyExTest.h"

#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/imgutil/Image.h"
#include "stromx/imgproc/MorphologyEx.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::imgproc::MorphologyExTest);

namespace stromx
{
    namespace imgproc
    {
        void MorphologyExTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new MorphologyEx);
        }
        
        void MorphologyExTest::tearDown()
        {
            delete m_operator;
        }
        
        void MorphologyExTest::testManual0()
        {
            m_operator->setParameter(MorphologyEx::DATA_FLOW, runtime::Enum(MorphologyEx::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg"));
            runtime::DataContainer dst(new imgutil::Image(1000000));
            runtime::Enum op(0);
            runtime::UInt32 ksizex(3);
            runtime::UInt32 ksizey(4);
            runtime::Enum shape(0);
            
            m_operator->setInputData(MorphologyEx::SRC, src);
            m_operator->setInputData(MorphologyEx::DST, dst);
            m_operator->setParameter(MorphologyEx::OP, op);
            m_operator->setParameter(MorphologyEx::KSIZEX, ksizex);
            m_operator->setParameter(MorphologyEx::KSIZEY, ksizey);
            m_operator->setParameter(MorphologyEx::SHAPE, shape);
            
            runtime::DataContainer result = m_operator->getOutputData(MorphologyEx::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("MorphologyExTest_testManual0.png", access());
        }
        
        void MorphologyExTest::testManual1()
        {
            m_operator->setParameter(MorphologyEx::DATA_FLOW, runtime::Enum(MorphologyEx::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg"));
            runtime::Enum op(2);
            runtime::Enum shape(1);
            runtime::UInt32 iterations(3);
            
            m_operator->setInputData(MorphologyEx::SRC, src);
            m_operator->setInputData(MorphologyEx::DST, src);
            m_operator->setParameter(MorphologyEx::OP, op);
            m_operator->setParameter(MorphologyEx::SHAPE, shape);
            m_operator->setParameter(MorphologyEx::ITERATIONS, iterations);
            
            runtime::DataContainer result = m_operator->getOutputData(MorphologyEx::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("MorphologyExTest_testManual1.png", access());
        }
        
        void MorphologyExTest::testAllocate0()
        {
            m_operator->setParameter(MorphologyEx::DATA_FLOW, runtime::Enum(MorphologyEx::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg", imgutil::Image::GRAYSCALE));
            runtime::Enum op(0);
            
            m_operator->setInputData(MorphologyEx::SRC, src);
            m_operator->setParameter(MorphologyEx::OP, op);
            
            runtime::DataContainer result = m_operator->getOutputData(MorphologyEx::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("MorphologyExTest_testAllocate0.png", access());
        }
        
        void MorphologyExTest::testAllocate1()
        {
            m_operator->setParameter(MorphologyEx::DATA_FLOW, runtime::Enum(MorphologyEx::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg"));
            runtime::Enum op(3);
            runtime::Enum shape(2);
            
            m_operator->setInputData(MorphologyEx::SRC, src);
            m_operator->setParameter(MorphologyEx::OP, op);
            m_operator->setParameter(MorphologyEx::SHAPE, shape);
            
            runtime::DataContainer result = m_operator->getOutputData(MorphologyEx::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("MorphologyExTest_testAllocate1.png", access());
        }
        
        void MorphologyExTest::testInPlace0()
        {
            m_operator->setParameter(MorphologyEx::DATA_FLOW, runtime::Enum(MorphologyEx::IN_PLACE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg", imgutil::Image::GRAYSCALE));
            runtime::Enum op(1);
            runtime::Enum shape(1);
            
            m_operator->setInputData(MorphologyEx::SRC, src);
            m_operator->setParameter(MorphologyEx::OP, op);
            m_operator->setParameter(MorphologyEx::SHAPE, shape);
            
            runtime::DataContainer result = m_operator->getOutputData(MorphologyEx::SRC);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("MorphologyExTest_testInPlace0.png", access());
        }
        
        void MorphologyExTest::testInPlace1()
        {
            m_operator->setParameter(MorphologyEx::DATA_FLOW, runtime::Enum(MorphologyEx::IN_PLACE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg"));
            runtime::Enum op(3);
            
            m_operator->setInputData(MorphologyEx::SRC, src);
            m_operator->setParameter(MorphologyEx::OP, op);
            
            runtime::DataContainer result = m_operator->getOutputData(MorphologyEx::SRC);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("MorphologyExTest_testInPlace1.png", access());
        }
        
    }
}


#include "stromx/cvimgproc/test/MorphologyExTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/MorphologyEx.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::MorphologyExTest);

namespace stromx
{
    namespace cvimgproc
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
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
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
            
            runtime::DataContainer dstResult = m_operator->getOutputData(MorphologyEx::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("MorphologyExTest_testManual0_dst.png", dstAccess());
        }
        
        void MorphologyExTest::testManual1()
        {
            m_operator->setParameter(MorphologyEx::DATA_FLOW, runtime::Enum(MorphologyEx::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::Enum op(2);
            runtime::Enum shape(1);
            runtime::UInt32 iterations(3);
            
            m_operator->setInputData(MorphologyEx::SRC, src);
            m_operator->setInputData(MorphologyEx::DST, src);
            m_operator->setParameter(MorphologyEx::OP, op);
            m_operator->setParameter(MorphologyEx::SHAPE, shape);
            m_operator->setParameter(MorphologyEx::ITERATIONS, iterations);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(MorphologyEx::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("MorphologyExTest_testManual1_dst.png", dstAccess());
        }
        
        void MorphologyExTest::testAllocate0()
        {
            m_operator->setParameter(MorphologyEx::DATA_FLOW, runtime::Enum(MorphologyEx::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Enum op(0);
            
            m_operator->setInputData(MorphologyEx::SRC, src);
            m_operator->setParameter(MorphologyEx::OP, op);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(MorphologyEx::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("MorphologyExTest_testAllocate0_dst.png", dstAccess());
        }
        
        void MorphologyExTest::testAllocate1()
        {
            m_operator->setParameter(MorphologyEx::DATA_FLOW, runtime::Enum(MorphologyEx::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::Enum op(3);
            runtime::Enum shape(2);
            
            m_operator->setInputData(MorphologyEx::SRC, src);
            m_operator->setParameter(MorphologyEx::OP, op);
            m_operator->setParameter(MorphologyEx::SHAPE, shape);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(MorphologyEx::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("MorphologyExTest_testAllocate1_dst.png", dstAccess());
        }
        
        void MorphologyExTest::testInPlace0()
        {
            m_operator->setParameter(MorphologyEx::DATA_FLOW, runtime::Enum(MorphologyEx::IN_PLACE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Enum op(1);
            runtime::Enum shape(1);
            
            m_operator->setInputData(MorphologyEx::SRC, src);
            m_operator->setParameter(MorphologyEx::OP, op);
            m_operator->setParameter(MorphologyEx::SHAPE, shape);
            
            runtime::DataContainer srcResult = m_operator->getOutputData(MorphologyEx::SRC);
            
            runtime::ReadAccess<runtime::Image> srcAccess(srcResult);
            cvsupport::Image::save("MorphologyExTest_testInPlace0_src.png", srcAccess());
        }
        
        void MorphologyExTest::testInPlace1()
        {
            m_operator->setParameter(MorphologyEx::DATA_FLOW, runtime::Enum(MorphologyEx::IN_PLACE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::Enum op(3);
            
            m_operator->setInputData(MorphologyEx::SRC, src);
            m_operator->setParameter(MorphologyEx::OP, op);
            
            runtime::DataContainer srcResult = m_operator->getOutputData(MorphologyEx::SRC);
            
            runtime::ReadAccess<runtime::Image> srcAccess(srcResult);
            cvsupport::Image::save("MorphologyExTest_testInPlace1_src.png", srcAccess());
        }
        
    } // cvimgproc
} // stromx


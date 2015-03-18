#include "stromx/cvimgproc/test/FindContoursTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/FindContours.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::FindContoursTest);

namespace stromx
{
    namespace cvimgproc
    {
        void FindContoursTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new FindContours);
        }
        
        void FindContoursTest::tearDown()
        {
            delete m_operator;
        }
        
        void FindContoursTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("contours.png", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(FindContours::SRC, src);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(FindContours::DST);
            
            std::size_t index = 0;
            runtime::ReadAccess<runtime::List> dstAccess(dstResult);
            const std::vector<const runtime::Data*> & content = dstAccess().content();
            for (std::vector<const runtime::Data*>::const_iterator iter = content.begin(); iter != content.end(); ++iter)
            {
                const runtime::Matrix* matrix = runtime::data_cast<runtime::Matrix>(*iter);
                std::string fileName = "FindContoursTest_testAllocate0_dst_" + boost::lexical_cast<std::string>(index) + ".npy";
                cvsupport::Matrix::save(fileName, *matrix);
                ++index;
            }
        }
        
        void FindContoursTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("contours.png", cvsupport::Image::GRAYSCALE));
            runtime::Enum method(1);
            
            m_operator->setInputData(FindContours::SRC, src);
            m_operator->setParameter(FindContours::METHOD, method);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(FindContours::DST);
            
            std::size_t index = 0;
            runtime::ReadAccess<runtime::List> dstAccess(dstResult);
            const std::vector<const runtime::Data*> & content = dstAccess().content();
            for (std::vector<const runtime::Data*>::const_iterator iter = content.begin(); iter != content.end(); ++iter)
            {
                const runtime::Matrix* matrix = runtime::data_cast<runtime::Matrix>(*iter);
                std::string fileName = "FindContoursTest_testAllocate1_dst_" + boost::lexical_cast<std::string>(index) + ".npy";
                cvsupport::Matrix::save(fileName, *matrix);
                ++index;
            }
        }
        
    } // cvimgproc
} // stromx


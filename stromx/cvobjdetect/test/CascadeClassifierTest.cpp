#include "stromx/cvobjdetect/test/CascadeClassifierTest.h"

#include <boost/lexical_cast.hpp>

#include <stromx/runtime/List.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/cvsupport/Image.h>
#include <stromx/cvsupport/Matrix.h>
#include "stromx/cvobjdetect/CascadeClassifier.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvobjdetect::CascadeClassifierTest);

namespace stromx
{
    namespace cvobjdetect
    {
        void CascadeClassifierTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new CascadeClassifier);
        }
        
        void CascadeClassifierTest::tearDown()
        {
            delete m_operator;
        }
        
        void CascadeClassifierTest::testExecute()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::File classifierFile("haarcascade_frontalface_alt.txt");
            m_operator->setParameter(CascadeClassifier::CLASSIFIER, classifierFile);
            
            runtime::DataContainer src(new cvsupport::Image("faces.jpg", cvsupport::Image::GRAYSCALE));
            m_operator->setInputData(CascadeClassifier::SRC, src);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(CascadeClassifier::DST);
            
            std::size_t index = 0;
            runtime::ReadAccess dstAccess(dstResult);
            const std::vector<const runtime::Data*> & content = dstAccess.get<runtime::List>().content();
            for (std::vector<const runtime::Data*>::const_iterator iter = content.begin(); iter != content.end(); ++iter)
            {
                const runtime::Matrix* matrix = runtime::data_cast<runtime::Matrix>(*iter);
                std::string fileName = "CascadeClassifierTest_testExecute_dst_" + boost::lexical_cast<std::string>(index) + ".npy";
                cvsupport::Matrix::save(fileName, *matrix);
                ++index;
            }
        }        
    } // cvobjdetect
} // stromx


#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include <stromx/cvcalib3d/Cvcalib3d.h>
#include <stromx/cvsupport/Cvsupport.h>
#include <stromx/runtime/AssignThreadsAlgorithm.h>
#include <stromx/runtime/Factory.h>
#include <stromx/runtime/Runtime.h>
#include <stromx/runtime/Stream.h>
#include <stromx/runtime/XmlReader.h>

using namespace stromx;
class IntegrationTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(IntegrationTest);
    CPPUNIT_TEST(testCalibration);
    CPPUNIT_TEST(testCamera);
    CPPUNIT_TEST_SUITE_END();
    
public:
    IntegrationTest() : m_stream(0) {}
    void setUp() {}
    
    void tearDown()
    {
        delete m_stream;
        delete m_factory;
        
        m_stream = 0;
        m_factory = 0;
    }
    
protected:
    void testCalibration()
    {
        m_factory = new runtime::Factory();
        stromxRegisterCvcalib3d(m_factory);
        stromxRegisterCvsupport(m_factory);
        stromxRegisterRuntime(m_factory);
        m_stream = runtime::XmlReader().readStream("calibration.stromx", m_factory);
        
        CPPUNIT_ASSERT_NO_THROW(runtime::AssignThreadsAlgorithm().apply(*m_stream));
    }
    
    void testCamera()
    {
        m_factory = new runtime::Factory();
        stromxRegisterCvsupport(m_factory);
        stromxRegisterRuntime(m_factory);
        m_stream = runtime::XmlReader().readStream("camera.stromx", m_factory);
        
        runtime::AssignThreadsAlgorithm().apply(*m_stream);
        
        CPPUNIT_ASSERT_EQUAL(std::size_t(1), m_stream->threads().size());
    }
    
private:
    runtime::Stream* m_stream;
    runtime::Factory* m_factory;
};

CPPUNIT_TEST_SUITE_REGISTRATION (IntegrationTest);

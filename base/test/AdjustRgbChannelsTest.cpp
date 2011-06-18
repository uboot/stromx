#include "AdjustRgbChannelsTest.h"

#include <base/AdjustRgbChannels.h>
#include <base/Image.h>
#include <stream/DataContainer.h>
#include <stream/OperatorWrapper.h>
#include <stream/Primitive.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::AdjustRgbChannelsTest);

using namespace stream;

namespace base
{
    void AdjustRgbChannelsTest::setUp ( void )
    {
        m_operator = new OperatorWrapper(new AdjustRgbChannels());
        m_operator->activate();
        m_image = new DataContainer(new Image("lenna.jpg"));
        m_operator->setInputData(AdjustRgbChannels::INPUT, m_image);
        m_image->dereference();
    }
    
    void AdjustRgbChannelsTest::testExecute()
    {
        m_operator->setParameter(AdjustRgbChannels::RED, Double(0.1));
        m_operator->setParameter(AdjustRgbChannels::GREEN, Double(1.0));
        m_operator->setParameter(AdjustRgbChannels::BLUE, Double(1.5));
        
        stream::DataContainer* result = m_operator->getOutputData(AdjustRgbChannels::OUTPUT);
        
        const Image* image = dynamic_cast<const Image*>(result->getReadAccess());
        CPPUNIT_ASSERT(image);
        
        m_operator->setInputData(AdjustRgbChannels::INPUT, m_image);
        m_operator->clearOutputData(AdjustRgbChannels::OUTPUT);
        
        result = m_operator->getOutputData(AdjustRgbChannels::OUTPUT);
    }
    
    void AdjustRgbChannelsTest::tearDown ( void )
    {
        delete m_operator;
        delete m_image;
    }
}
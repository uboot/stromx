/* 
 *  Copyright 2016 Matthias Fuchs
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/Variant.h"
#include "stromx/runtime/impl/SynchronizedOperatorKernel.h"

namespace stromx
{
namespace runtime
{
namespace
{
class ConnectorOperator : public OperatorKernel
{
public:
    ConnectorOperator()
        : OperatorKernel("", "", Version(), setupInputs(), 
            std::vector<const Output*>(), setupProperties())
    {}
            
    void initialize()
    {
        OperatorKernel::initialize(std::vector<const Input*>(), 
            setupOutputs(), std::vector<const Parameter*>());
    }
    
    OperatorKernel* clone() const { return 0; }
    void execute(DataProvider& provider)
    {
        Id2DataPair in(0);
        provider.receiveInputData(in);
        
        Int16* data = new Int16(ReadAccess(in.data()).get<UInt16>());
        Id2DataPair out(1, DataContainer(data));
        provider.sendOutputData(out);
    }
    
    static const std::vector<const Input* > setupInputs()
    {
        std::vector<const Input*> inputs;
        
        Input* input = new Input(0, Variant::UINT_16);
        inputs.push_back(input);
        
        input = new Input(3, Variant::UINT_16);
        input->setDefaultType(Input::PARAMETER);
        inputs.push_back(input);
        
        return inputs;
    }
    
    static const std::vector<const Output* > setupOutputs()
    {
        std::vector<const Output*> outputs;
        
        Output* output = new Output(1, Variant::INT_16);
        outputs.push_back(output);
        
        return outputs;
    }
    
    static const OperatorProperties setupProperties()
    {
        OperatorProperties properties;
        properties.isGreedy = true;
        return properties;
    }
};
}

using namespace impl;

class SynchronizedOperatorKernelTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (SynchronizedOperatorKernelTest);
    CPPUNIT_TEST (testSetConnectorNotInitialized);
    CPPUNIT_TEST (testSetConnectorActive);
    CPPUNIT_TEST (testSetConnectorTypeInputToParameter);
    CPPUNIT_TEST (testSetInputParameterNotActivated);
    CPPUNIT_TEST (testSetConnectorTypeOutputToParameter);
    CPPUNIT_TEST (testSetConnectorTypeInputToParameterWrongVariant);
    CPPUNIT_TEST (testSetConnectorTypeOutputToParameterWrongVariant);
    CPPUNIT_TEST (testSetConnectorTypeParameterToInput);
    CPPUNIT_TEST (testSetConnectorTypeParameterToOutput);
    CPPUNIT_TEST (testConnectorParameterWrongType);
    CPPUNIT_TEST (testInputParameterPersistent);
    CPPUNIT_TEST (testInputParameterDeactivate);
    CPPUNIT_TEST (testInputParameterPush);
    CPPUNIT_TEST (testOutputParameterPersistent);
    CPPUNIT_TEST (testOutputParameterPull);
    CPPUNIT_TEST_SUITE_END ();

public:
    SynchronizedOperatorKernelTest() : m_kernel(0) {}
    
    void setUp()
    {
        m_kernel = new SynchronizedOperatorKernel(new ConnectorOperator);
    }
    
    void tearDown()
    {
        delete m_kernel;
        m_kernel = 0;
    }
    
private:
    SynchronizedOperatorKernel* m_kernel;

protected:
    void testSetConnectorNotInitialized()
    {
        CPPUNIT_ASSERT_THROW(
            m_kernel->setConnectorType(0, Description::PARAMETER, Description::PERSISTENT), 
        WrongOperatorState);
    }
    
    void testSetConnectorActive()
    {
        m_kernel->initialize(0, 0);
        m_kernel->activate();
        
        CPPUNIT_ASSERT_THROW(
            m_kernel->setConnectorType(0, Description::PARAMETER, Description::PERSISTENT), 
        WrongOperatorState);
    }
    
    void testSetConnectorTypeInputToParameter()
    {
        m_kernel->initialize(0, 0);
        m_kernel->setConnectorType(0, Description::PARAMETER, Description::PERSISTENT);
        m_kernel->activate();
        
        UInt16 data(42);
        DataContainer container(new UInt16(43));
        CPPUNIT_ASSERT_THROW(m_kernel->setInputData(0, container), WrongId);
        CPPUNIT_ASSERT_NO_THROW(m_kernel->setParameter(0, data, false));
        CPPUNIT_ASSERT_EQUAL(UInt16(42), data_cast<UInt16>(m_kernel->getParameter(0, false)));
    }
    
    void testSetInputParameterNotActivated()
    {
        m_kernel->initialize(0, 0);
        
        UInt16 data(42);
        DataContainer container(new UInt16(43));
        CPPUNIT_ASSERT_NO_THROW(m_kernel->setParameter(3, data, false));
    }
    
    void testSetConnectorTypeOutputToParameter()
    {
        m_kernel->initialize(0, 0);
        m_kernel->setConnectorType(1, Description::PARAMETER, Description::PERSISTENT);
        m_kernel->activate();
        
        Int16 data(42);
        CPPUNIT_ASSERT_THROW(m_kernel->getOutputData(1), WrongId);
        CPPUNIT_ASSERT_THROW(m_kernel->setParameter(1, data, false), ParameterAccessViolation);
    }
    
    void testSetConnectorTypeInputToParameterWrongVariant()
    {
        m_kernel->initialize(0, 0);
        m_kernel->setConnectorType(0, Description::PARAMETER, Description::PERSISTENT);
        
        Int16 data;
        CPPUNIT_ASSERT_THROW(m_kernel->setParameter(0, data, false), WrongParameterType);
    }
    
    void testSetConnectorTypeOutputToParameterWrongVariant()
    {
        m_kernel->initialize(0, 0);
        m_kernel->setConnectorType(1, Description::PARAMETER, Description::PERSISTENT);
        
        UInt16 data;
        CPPUNIT_ASSERT_THROW(m_kernel->setParameter(1, data, false), WrongParameterType);
    }
    
    void testSetConnectorTypeParameterToInput()
    {
        m_kernel->initialize(0, 0);
        m_kernel->setConnectorType(0, Description::PARAMETER, Description::PERSISTENT);
        m_kernel->setConnectorType(0, Description::INPUT);
        m_kernel->activate();
        
        UInt16 data(42);
        DataContainer container(new UInt16(43));
        CPPUNIT_ASSERT_NO_THROW(m_kernel->setInputData(0, container));
        CPPUNIT_ASSERT_THROW(m_kernel->setParameter(0, data, false), WrongParameterId);
    }
    
    void testSetConnectorTypeParameterToOutput()
    {
        m_kernel->initialize(0, 0);
        m_kernel->setConnectorType(1, Description::PARAMETER, Description::PERSISTENT);
        m_kernel->setConnectorType(1, Description::OUTPUT);
        m_kernel->activate();
        
        Int16 data(42);
        CPPUNIT_ASSERT_NO_THROW(m_kernel->clearOutputData(1));
        CPPUNIT_ASSERT_THROW(m_kernel->setParameter(1, data, false), WrongParameterId);
    }
    
    void testConnectorParameterWrongType()
    {
        m_kernel->initialize(0, 0);
        m_kernel->setConnectorType(0, Description::PARAMETER, Description::PERSISTENT);
        
        Int16 data(42);
        CPPUNIT_ASSERT_THROW(m_kernel->setParameter(0, data, false), WrongParameterType);
    }
    
    void testInputParameterPersistent()
    {
        m_kernel->initialize(0, 0);
        m_kernel->setConnectorType(0, Description::PARAMETER, Description::PERSISTENT);
        
        UInt16 data(42);
        CPPUNIT_ASSERT_THROW(m_kernel->getParameter(0, false), ParameterError);
        m_kernel->setParameter(0, data, false);
        
        CPPUNIT_ASSERT_NO_THROW(m_kernel->setParameter(0, data, false));
        CPPUNIT_ASSERT_NO_THROW(m_kernel->getParameter(0, false));
        CPPUNIT_ASSERT_NO_THROW(m_kernel->getParameter(0, false));
    }
    
    void testInputParameterDeactivate()
    {
        m_kernel->initialize(0, 0);
        m_kernel->setConnectorType(0, Description::PARAMETER, Description::PERSISTENT);
        m_kernel->activate();
        
        UInt16 data(42);
        m_kernel->setParameter(0, data, false);
        m_kernel->deactivate();
        
        CPPUNIT_ASSERT_NO_THROW(m_kernel->getParameter(0, false));
    }
    
    void testInputParameterPush()
    {
        m_kernel->initialize(0, 0);
        m_kernel->setConnectorType(0, Description::PARAMETER, Description::PUSH);
        
        UInt16 data(42);
        m_kernel->setParameter(0, data, false);
        
        CPPUNIT_ASSERT_THROW(m_kernel->setParameter(0, data, true, 0), Timeout);
        CPPUNIT_ASSERT_NO_THROW(m_kernel->getParameter(0, false));
        CPPUNIT_ASSERT_THROW(m_kernel->setParameter(0, data, true, 0), Timeout);
        
        m_kernel->activate();
        m_kernel->getOutputData(1);
        CPPUNIT_ASSERT_NO_THROW(m_kernel->setParameter(0, data, true, 0));
    }
    
    void testOutputParameterPersistent()
    {
        m_kernel->initialize(0, 0);
        m_kernel->setConnectorType(1, Description::PARAMETER, Description::PERSISTENT);
        m_kernel->activate();
        
        CPPUNIT_ASSERT_THROW(m_kernel->getParameter(1, true, 0), ParameterError);
        
        DataContainer data(new UInt16(42));
        m_kernel->setInputData(0, data);
        
        CPPUNIT_ASSERT_NO_THROW(m_kernel->getParameter(1, true, 0));
        CPPUNIT_ASSERT_NO_THROW(m_kernel->getParameter(1, true, 0));
    }
    
    void testOutputParameterPull()
    {
        m_kernel->initialize(0, 0);
        m_kernel->setConnectorType(1, Description::PARAMETER, Description::PULL);
        m_kernel->activate();
        
        CPPUNIT_ASSERT_THROW(m_kernel->getParameter(1, true, 0), ParameterError);
        
        DataContainer data(new UInt16(42));
        m_kernel->setInputData(0, data);
        
        CPPUNIT_ASSERT_NO_THROW(m_kernel->getParameter(1, true, 0));
        CPPUNIT_ASSERT_THROW(m_kernel->getParameter(1, true, 0), ParameterError);
    }
};
}
}

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::SynchronizedOperatorKernelTest);
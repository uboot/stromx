#include "stromx/test/RandomDataOperator.h"

#include <stromx/cvsupport/Matrix.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/String.h>
#include <cstdlib>

using namespace stromx::runtime;

namespace stromx
{
    namespace test
    {
        const std::string RandomDataOperator::TYPE("RandomDataOperator");
        const std::string RandomDataOperator::PACKAGE(STROMX_TEST_PACKAGE_NAME);
        const Version RandomDataOperator::VERSION(1, 2, 3);

        RandomDataOperator::RandomDataOperator()
        : OperatorKernel(TYPE, PACKAGE, VERSION,
                        setupInputs(), setupOutputs(),
                        setupParameters())
        {
        }

        void RandomDataOperator::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case DATA_TYPE:
                    m_dataType = data_cast<Enum>(value);
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(*parameters()[id], *this);
            }
        }

        const DataRef RandomDataOperator::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case DATA_TYPE:
                return m_dataType;
            default:
                throw WrongParameterId(id, *this);
            }
        }  

        void RandomDataOperator::execute(DataProvider& provider)
        {
            Data* data = 0;
            switch(m_dataType)
            {
            case LINE_SEGMENTS:
            {
                stromx::runtime::Matrix* segments = new stromx::cvsupport::Matrix(2, 4, ::Matrix::FLOAT_64);
                double* doubleData = reinterpret_cast<double*>(segments->data());
                doubleData[0] = 50 + uniform(20);
                doubleData[1] = 100 + uniform(20);
                doubleData[2] = 200 + uniform(20);
                doubleData[3] = 150 + uniform(20);
                
                doubleData[4] = 400 + uniform(20);
                doubleData[5] = 30 + uniform(20);
                doubleData[6] = 50 + uniform(20);
                doubleData[7] = 300 + uniform(20);
                
                data = segments;
                break;
            }
            case STRING:
                data = new stromx::runtime::String("Random string");
                break;
            default:
                ;
            }
            
            if(data)
            {
                Id2DataPair output(OUTPUT, DataContainer(data));
                provider.sendOutputData(output);
            }
        }
                
        const std::vector<const Description*> RandomDataOperator::setupInputs()
        {
            return std::vector<const Description*>();
        }

        const std::vector<const Description*> RandomDataOperator::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* description = new Description(OUTPUT, Variant::NONE);
            description->setTitle("Output");
            outputs.push_back(description);
            
            return outputs;
        }

        const std::vector<const Parameter*> RandomDataOperator::setupParameters()
        {
            std::vector<const Parameter*> parameters;
            
            EnumParameter* param = new EnumParameter(DATA_TYPE);
            param->setTitle("Data type");
            param->setAccessMode(Parameter::ACTIVATED_WRITE);
            param->add(EnumDescription(Enum(LINE_SEGMENTS), "Line segments"));
            param->add(EnumDescription(Enum(STRING), "String"));
            parameters.push_back(param);
            
            return parameters;
        }

        double RandomDataOperator::uniform(double limit)
        {
            return (double(rand()) / RAND_MAX - 0.5) * 2.0 * limit;
        }
    }
}


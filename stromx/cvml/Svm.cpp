/* 
*  Copyright 2015 Matthias Fuchs
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

#include "stromx/cvml/Svm.h"

#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataPair.h>

#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/Matrix.h>
#include <stromx/runtime/MatrixDescription.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/cvsupport/Utilities.h>

#include <opencv2/ml/ml.hpp>

using namespace stromx::runtime;
    
namespace stromx
{
    namespace cvml
    {        
        const std::string Svm::TYPE("Svm");
        
        const std::string Svm::PACKAGE(STROMX_CVML_PACKAGE_NAME);
        const runtime::Version Svm::VERSION(0, 1, 0);
        
            
        Svm::Svm()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
            m_svm(new cv::SVM())
        {
        }
        
        const runtime::DataRef Svm::getParameter(const unsigned int id)
        {
        }
        
        void Svm::setParameter(const unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case STATISTICAL_MODEL:
                    m_statisticalModel = stromx::runtime::data_cast<File>(value);
                    m_svm->load(m_statisticalModel.path().c_str());
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }
        
        void Svm::execute(runtime::DataProvider& provider)
        {
            Id2DataPair dataMapper(DATA);
            provider.receiveInputData(dataMapper);
            ReadAccess dataAccess(dataMapper.data());
            
            const Matrix & data = dataAccess.get<Matrix>();
            cv::Mat cvData = cvsupport::getOpenCvMat(data);
            
            float response = m_svm->predict(cvData);
            DataContainer result(new Float32(response));
            Id2DataPair responseMapper(PREDICTED_RESPONSE, result);
            provider.sendOutputData(responseMapper);
        }
        
        const std::vector<const runtime::Description*> Svm::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            MatrixDescription* input = new MatrixDescription(DATA, Variant::FLOAT_32_MATRIX);
            input->setTitle("Data vector");
            input->setRows(1);
            inputs.push_back(input);
            
            Description* response = new Description(TRAINING_RESPONSE, Variant::FLOAT_32);
            response->setTitle("Training response");
            inputs.push_back(response);
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> Svm::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            Description* response = new Description(PREDICTED_RESPONSE, Variant::FLOAT_32);
            response->setTitle("Predicted response");
            outputs.push_back(response);
            
            return outputs;
        }
        
        const std::vector<const runtime::Parameter*> Svm::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            Parameter* trainingIsActive = new Parameter(TRAINING_IS_ACTIVE, Variant::BOOL);
            trainingIsActive->setTitle("Training is active");
            trainingIsActive->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(trainingIsActive);
            
            Parameter* statisticalModel = new Parameter(STATISTICAL_MODEL, Variant::FILE);
            statisticalModel->setTitle("Statistical model");
            statisticalModel->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(statisticalModel);
                                        
            return parameters;
        }
    } 
}

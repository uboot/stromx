/* 
 *  Copyright 2014 Thomas Fidler
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

#include "stromx/raspi/Config.h"
#include "stromx/raspi/RaspiCam.h"

namespace stromx
{
    namespace raspi
    {
        const std::string RaspiCam::TYPE("RaspiCam");
        const std::string RaspiCam::PACKAGE(STROMX_RASPI_PACKAGE_NAME);
        const runtime::Version RaspiCam::VERSION(STROMX_RASPI_VERSION_MAJOR,STROMX_RASPI_VERSION_MINOR,STROMX_RASPI_VERSION_PATCH);
        
        const std::vector< const runtime::Description* > RaspiCam::setupInputs()
        {                       
            return std::vector<const runtime::Description*>();
        }
        
        const std::vector< const runtime::Description* > RaspiCam::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;

            runtime::Description* output = new runtime::Description(OUTPUT, runtime::DataVariant::RGB_IMAGE);
            output->setTitle("Output");
            outputs.push_back(output);

            return outputs;
        }

        const std::vector< const runtime::Parameter* > RaspiCam::setupInitParameters()
        {
            return std::vector<const runtime::Parameter*>();
        }
     
        RaspiCam::RaspiCam()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupInitParameters())
        {
        }

        RaspiCam::~RaspiCam()
        {
        }
        
        void RaspiCam::execute(runtime::DataProvider& provider)
        {

        }

    }
}
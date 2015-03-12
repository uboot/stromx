/* 
*  Copyright 2014 Matthias Fuchs
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

#include <python/stromx/runtime/ExportOperatorKernel.h>

#include <boost/python.hpp>

#include <stromx/runtime/Registry.h>
#include <stromx/raspi/Raspi.h>
#include <stromx/raspi/RaspiCam.h>
#include <stromx/raspi/RaspiStillCam.h>
#include <stromx/raspi/GpioTrigger.h>
#include <stromx/raspi/ReadGpio.h>
#include <stromx/raspi/WriteGpio.h>

using namespace boost::python;
using namespace stromx::raspi;
    
BOOST_PYTHON_MODULE(libraspi)
{
    def("register", stromxRaspiRegister);
    
    stromx::python::exportOperatorKernel<RaspiCam>("RaspCam");
    stromx::python::exportOperatorKernel<RaspiCam>("RaspStillCam");
    stromx::python::exportOperatorKernel<GpioTrigger>("GpioTrigger");
    stromx::python::exportOperatorKernel<ReadGpio>("ReadGpio");
    stromx::python::exportOperatorKernel<WriteGpio>("WriteGpio");
}

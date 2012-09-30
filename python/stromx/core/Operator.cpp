/* 
*  Copyright 2011 Matthias Fuchs
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

#include <stromx/core/Operator.h>
#include <stromx/core/Data.h>
#include <stromx/core/OperatorKernel.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

namespace
{
    std::auto_ptr<Operator> allocate(std::auto_ptr<OperatorKernel> kernel)
    {
        std::auto_ptr<Operator> op(new Operator(kernel.get()));
        kernel.release();
        return op;
    }
    
    const DataContainer getOutputDataWrap(Operator & op, const unsigned int id)
    {
        DataContainer data;
        
        Py_BEGIN_ALLOW_THREADS
        data = op.getOutputData(id);
        Py_END_ALLOW_THREADS
        
        return data;
    }
    
    void setInputDataWrap(Operator & op, const unsigned int id, const DataContainer data)
    {
        Py_BEGIN_ALLOW_THREADS
        op.setInputData(id, data);
        Py_END_ALLOW_THREADS
    }
    
    const Data& getParameterWrap(Operator & op, const unsigned int id)
    {
//         const Data* data = 0;
//         
//         Py_BEGIN_ALLOW_THREADS
//         data = &op.getParameter(id);
//         Py_END_ALLOW_THREADS
//         
//         return *data;
    }
    
    const Data& getParameterWithTimoutWrap(Operator & op, const unsigned int id, const unsigned int timeout)
    {
//         const Data* data = 0;
//         
//         Py_BEGIN_ALLOW_THREADS
//         return op.getParameter(id, timeout);
//         Py_END_ALLOW_THREADS
//         
//         return *data;
    }
    
    void setParameterWrap(Operator & op, const unsigned int id, const Data & data)
    {
        Py_BEGIN_ALLOW_THREADS
        op.setParameter(id, data);
        Py_END_ALLOW_THREADS
    }
    
    void setParameterWithTimoutWrap(Operator & op, const unsigned int id, const Data & data, const unsigned int timeout)
    {
        Py_BEGIN_ALLOW_THREADS
        op.setParameter(id, data, timeout);
        Py_END_ALLOW_THREADS
    }
}
      
void exportOperator()
{       
    scope in_Operator =
    class_<Operator, std::auto_ptr<Operator>, boost::noncopyable>("Operator", no_init)
        .def("__init__", make_constructor(&allocate))
        .def("status", &Operator::status)
        .def("info", &Operator::info, return_internal_reference<>())
        .def("name", &Operator::name, return_value_policy<copy_const_reference>())
        .def("setName", &Operator::setName)
        .def("initialize", &Operator::initialize)
        .def("deinitialize", &Operator::deinitialize)
        .def("getParameter", &getParameterWrap, return_internal_reference<>())
        .def("getParameter", &getParameterWithTimoutWrap, return_internal_reference<>())
        .def("setParameter", &setParameterWrap)
        .def("setParameter", &setParameterWrap)
        .def("getOutputData", &getOutputDataWrap)
        .def("setInputData", &setInputDataWrap)
        .def("clearOutputData", &Operator::clearOutputData)
        .def("addObserver", &Operator::addObserver)
        .def("removeObserver", &Operator::removeObserver)
    ;
    
    enum_<Operator::Status>("Status")
        .value("NONE", Operator::NONE)
        .value("INITIALIZED", Operator::INITIALIZED)
        .value("ACTIVE", Operator::ACTIVE)
        .value("EXECUTING", Operator::EXECUTING)
        ;
}

            


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

#include <boost/python.hpp>

#include <stromx/runtime/AbstractFactory.h>
#include <stromx/runtime/Operator.h>
#include <stromx/runtime/Data.h>
#include <stromx/runtime/OperatorKernel.h>

#include "Equality.h"

using namespace boost::python;
using namespace stromx::runtime;


namespace
{
    /*
    * TODO
    * 
    * This code is based on http://stackoverflow.com/q/6326757
    */
    template<typename T>
    void do_release(typename std::tr1::shared_ptr<T> const&, T*)
    {
    }

    template<typename T>
    typename boost::shared_ptr<T> toBoost(typename std::tr1::shared_ptr<T> const& p)
    {
        return
            boost::shared_ptr<T>(
            p.get(),
            boost::bind(&do_release<T>, p, _1));

    }

    Operator* allocate(std::auto_ptr<OperatorKernel> kernel)
    {
        Operator* op = new Operator(kernel.get());
        kernel.release();
        return op;
    }
    
    const DataContainer getOutputDataWrap(Operator & op, const unsigned int id)
    {
        DataContainer data;
        
        Py_BEGIN_ALLOW_THREADS
        try
        {
            data = op.getOutputData(id);
        }
        catch(stromx::runtime::Exception&)
        {
            Py_BLOCK_THREADS
            throw;
        }
        Py_END_ALLOW_THREADS
        
        return data;
    }
    
    void clearOutputDataWrap(Operator & op, const unsigned int id)
    {
        Py_BEGIN_ALLOW_THREADS
        try
        {
            op.clearOutputData(id);
        }
        catch(stromx::runtime::Exception&)
        {
            Py_BLOCK_THREADS
            throw;
        }
        Py_END_ALLOW_THREADS
    }
    
    void setInputDataWrap(Operator & op, const unsigned int id, const DataContainer & data)
    {
        Py_BEGIN_ALLOW_THREADS
        try
        {
            op.setInputData(id, data);
        }
        catch(stromx::runtime::Exception&)
        {
            Py_BLOCK_THREADS
            throw;
        }
        Py_END_ALLOW_THREADS
    }
    
    boost::shared_ptr<Data> getParameterWrap(Operator & op, const unsigned int id)
    {
        DataRef data;
        
        Py_BEGIN_ALLOW_THREADS
        try
        {
            data = op.getParameter(id);
        }
        catch(stromx::runtime::Exception&)
        {
            Py_BLOCK_THREADS
            throw;
        }
        Py_END_ALLOW_THREADS
        
        return toBoost(data.ptr());
    }
    
    boost::shared_ptr<Data> getParameterWithTimeoutWrap(Operator & op, const unsigned int id, const unsigned int timeout)
    {
        DataRef data;
        
        Py_BEGIN_ALLOW_THREADS
        try
        {
            data = op.getParameter(id, timeout);
        }
        catch(stromx::runtime::Exception&)
        {
            Py_BLOCK_THREADS
            throw;
        }
        Py_END_ALLOW_THREADS
        
        return toBoost(data.ptr());;
    }
    
    void setParameterWrap(Operator & op, const unsigned int id, const Data & data)
    {
        Py_BEGIN_ALLOW_THREADS
        try
        {
            op.setParameter(id, data);
        }
        catch(stromx::runtime::Exception&)
        {
            Py_BLOCK_THREADS
            throw;
        }
        Py_END_ALLOW_THREADS
    }
    
    void setParameterWithTimeoutWrap(Operator & op, const unsigned int id, const Data & data, const unsigned int timeout)
    {
        Py_BEGIN_ALLOW_THREADS
        try
        {
            op.setParameter(id, data, timeout);
        }
        catch(stromx::runtime::Exception&)
        {
            Py_BLOCK_THREADS
            throw;
        }
        Py_END_ALLOW_THREADS
    }
}

stromx::runtime::Data* get_pointer(const stromx::runtime::DataRef & p)
{
    return &(Data&)(p);
}
      
void exportOperator()
{       
    scope in_Operator =
    class_<Operator, boost::noncopyable>("Operator", no_init)
        .def("__init__", make_constructor(&allocate))
        .def("status", &Operator::status)
        .def("info", &Operator::info, return_internal_reference<>())
        .def("name", &Operator::name, return_value_policy<copy_const_reference>())
        .def("setName", &Operator::setName)
        .def("position", &Operator::position, return_internal_reference<>())
        .def("setPosition", &Operator::setPosition)
        .def("getParameter", &getParameterWrap)
        .def("getParameter", &getParameterWithTimeoutWrap)
        .def("setParameter", &setParameterWrap)
        .def("setParameter", &setParameterWithTimeoutWrap)
        .def("getOutputData", &getOutputDataWrap)
        .def("setInputData", &setInputDataWrap)
        .def("clearOutputData", &clearOutputDataWrap)
        .def("addObserver", &Operator::addObserver)
        .def("removeObserver", &Operator::removeObserver)
        .def("setFactory", &Operator::setFactory)
        .def("factory", &Operator::factory, return_internal_reference<>())
        .def("__eq__", &stromx::python::eq<Operator>)
        .def("__ne__", &stromx::python::ne<Operator>)
    ;
    
    enum_<Operator::Status>("Status")
        .value("NONE", Operator::NONE)
        .value("INITIALIZED", Operator::INITIALIZED)
        .value("ACTIVE", Operator::ACTIVE)
        .value("EXECUTING", Operator::EXECUTING)
        ;
        
    register_ptr_to_python< boost::shared_ptr<Data> >();
}

            


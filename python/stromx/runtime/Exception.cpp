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

#include <stromx/runtime/Exception.h>
#include <stromx/runtime/OperatorException.h>

#include <boost/python.hpp>

namespace
{
    using namespace boost::python;
    
    // cf. http://stackoverflow.com/questions/9620268/boost-python-custom-exception-class
    PyObject* createExceptionClass(const char* name, PyObject* baseTypeObj)
    {
        using std::string;

        string scopeName = extract<string>(scope().attr("__name__"));
        string qualifiedName0 = scopeName + "." + name;
        char* qualifiedName1 = const_cast<char*>(qualifiedName0.c_str());

        PyObject* typeObj = PyErr_NewException(qualifiedName1, baseTypeObj, 0);
        if (!typeObj) 
            throw_error_already_set();
        scope().attr(name) = handle<>(borrowed(typeObj));
        return typeObj;
    }
    
    template <class T>
    class Translator
    {
    public:
        Translator(const std::string & name, PyObject* const baseClass)
          : m_pyException(createExceptionClass(name.c_str(), baseClass))
        {}
        
        void operator()(const T &e) const
        {
            PyErr_SetString(m_pyException, e.what());
        }
        
        PyObject* classObject() { return m_pyException; }
        
    private:
        PyObject* m_pyException;
    };
    
    template <class T>
    PyObject* proxy(const std::string & name,
                    PyObject* const baseClass = PyExc_Exception)
    {
        Translator<T> translator(name, baseClass);
        register_exception_translator<T>(translator);
        return translator.classObject();
    }
    
}

void exportException()
{      
    using namespace stromx::runtime;

    PyObject* exception = proxy<Exception>("Exception");
    proxy<AccessEmpty>("AccessEmpty", exception);
    proxy<BadCast>("BadCast", exception);
    proxy<WrongArgument>("WrongArgument", exception);
    
    PyObject* dataException = proxy<DataException>("DataException", exception);
    proxy<DeserializationError>("DeserializationError", dataException);
    proxy<SerializationError>("SerializationError", dataException);
    
    PyObject* factoryException = proxy<FactoryException>("FactoryException", exception);
    proxy<DataAllocationFailed>("DataAllocationFailed", factoryException);
    proxy<OperatorAllocationFailed>("OperatorAllocationFailed", factoryException);
    
    PyObject* fileException = proxy<FileException>("FileException", exception);
    proxy<FileAccessFailed>("FileAccessFailed", fileException);
    proxy<InconsistentFileContent>("InconsistentFileContent", fileException);
    proxy<InvalidFileFormat>("InvalidFileFormat", fileException);
    
    proxy<Interrupt>("Interrupt", exception);
    proxy<NoInputFile>("NoInputFile", exception);
    proxy<NotImplemented>("NotImplemented", exception);
    proxy<OutOfMemory>("OutOfMemory", exception);
    proxy<Timeout>("Timeout", exception);
    proxy<WrongState>("WrongState", exception);
    proxy<WrongId>("WrongId", exception);
                    
    PyObject* operatorError = proxy<OperatorError>("OperatorError", exception);
    proxy<InputError>("InputError", operatorError);
    proxy<OutputError>("OutputError", operatorError);
    proxy<ParameterError>("ParameterError", operatorError);
                    
    PyObject* parameterError = proxy<ParameterError>("ParameterError", exception);
    proxy<ParameterAccessViolation>("ParameterAccessViolation", parameterError);
    proxy<WrongParameterType>("WrongParameterType", parameterError);
    proxy<WrongParameterValue>("WrongParameterValue", parameterError);
    proxy<WrongParameterId>("WrongParameterId", operatorError);
}

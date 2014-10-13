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

#include <stromx/runtime/Matrix.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{
    struct MatrixWrap : Matrix, wrapper<Matrix>
    {
        const Version & version() const
        {
            return this->get_override("version")();
        }
        
        const std::string & type() const
        {
            return this->get_override("type")();
        }
        
        const std::string & package() const
        {
            return this->get_override("package")();
        }
        
        const DataVariant & variant() const
        {
            return this->get_override("variant")();
        }
        
        Data* clone() const
        {
            return this->get_override("clone")();
        }
        
        uint8_t* buffer()
        {
            return this->get_override("buffer")();
        }
        
        unsigned int bufferSize() const
        {
            return this->get_override("bufferSize")();
        }
        
        unsigned int rows() const
        {
            return this->get_override("rows")();
        }
        
        unsigned int cols() const
        {
            return this->get_override("cols")();
        }
        
        unsigned int stride() const
        {
            return this->get_override("stride")();
        }
        
        ValueType valueType() const
        {
            return this->get_override("valueType")();
        }
        
        unsigned int valueSize(const ValueType valueType) const
        {
            return this->get_override("valueSize")(valueType);
        }
        
        uint8_t* data()
        {
            return this->get_override("data")();
        }
        
        const uint8_t* data() const
        {
            return this->get_override("data")();
        }
    };
    
    object data(Matrix & m)
    {               
        m.m_shape[0] = m.rows();
        m.m_shape[1] = m.cols();
        Py_ssize_t size = m.rows() * m.stride();

        m.m_strides[0] = m.stride();
        m.m_strides[1] = 1;
        
        switch(m.valueType())
        {
            case Matrix::UINT_8:
                m.m_format = "B";
                break;
            case Matrix::INT_8:
                m.m_format = "b";
                break;
            case Matrix::UINT_16:
                m.m_format = "H";
                break;
            case Matrix::INT_16:
                m.m_format = "h";
                break;
            case Matrix::FLOAT_32:
                m.m_format = "f";
                break;
            case Matrix::FLOAT_64:
                m.m_format = "d";
                break;
            default:
                BOOST_ASSERT(false);
        }
            
        char* format = const_cast<char*>(m.m_format.c_str());

        Py_buffer view =
        {
            m.data(),       // buf
            0,              // obj
            size,           // len
            m.valueSize(),  // itemsize
            1,              // readonly
            2,              // ndim
            format,         // format
            m.m_shape,      // shape
            m.m_strides,    // strides
            0,              // suboffsets
            {0, 0},         // smalltable
            0
        };
        PyObject* o = PyMemoryView_FromBuffer(&view);
        return object(handle<>(o));
    }
}


void exportMatrix()
{     
    scope in_Matrix = 
    class_<MatrixWrap, bases<Data>, boost::noncopyable>("Matrix", no_init)
        .def("rows", pure_virtual(&Matrix::rows))
        .def("cols", pure_virtual(&Matrix::cols))
        .def("stride", pure_virtual(&Matrix::stride))
        .def("valueType", pure_virtual(&Matrix::valueType))
        .def<unsigned int (stromx::runtime::Matrix::*)() const>("valueSize", &Matrix::valueSize)
        .def("data", &data)
    ;
    
    enum_<Matrix::ValueType>("ValueType")
        .value("INT_8", Matrix::INT_8)
        .value("INT_16", Matrix::INT_16)
        .value("INT_32", Matrix::INT_32)
        .value("UINT_8", Matrix::UINT_8)
        .value("UINT_16", Matrix::UINT_16)
        .value("UINT_32", Matrix::UINT_32)
        .value("FLOAT_32", Matrix::FLOAT_32)
        .value("FLOAT_64", Matrix::FLOAT_64)
    ;
}
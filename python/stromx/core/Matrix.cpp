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

#include <stromx/core/Matrix.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

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
        
        Data* const clone() const
        {
            return this->get_override("clone")();
        }
        
        uint8_t* const buffer()
        {
            return this->get_override("buffer")();
        }
        
        const unsigned int bufferSize() const
        {
            return this->get_override("bufferSize")();
        }
        
        const unsigned int rows() const
        {
            return this->get_override("rows")();
        }
        
        const unsigned int cols() const
        {
            return this->get_override("cols")();
        }
        
        const unsigned int stride() const
        {
            return this->get_override("stride")();
        }
        
        const ValueType valueType() const
        {
            return this->get_override("valueType")();
        }
        
        uint8_t* const data()
        {
            return this->get_override("data")();
        }
        
        const uint8_t* const data() const
        {
            return this->get_override("data")();
        }
        
        void initialize(const unsigned int rows, 
                        const unsigned int cols, 
                        const unsigned int stride, 
                        uint8_t* const data, 
                        const ValueType valueType)
        {
            this->get_override("initialize")();
        }
    };
}

void exportMatrix()
{     
    enum_<Matrix::ValueType>("MatrixValueType")
        .value("INT_8", Matrix::INT_8)
        .value("INT_16", Matrix::INT_16)
        .value("INT_32", Matrix::INT_32)
        .value("UINT_8", Matrix::UINT_8)
        .value("UINT_16", Matrix::UINT_16)
        .value("UINT_32", Matrix::UINT_32)
        .value("FLOAT", Matrix::FLOAT)
        .value("DOUBLE", Matrix::DOUBLE)
    ;
        
    class_<MatrixWrap, bases<Data>, boost::noncopyable>("Matrix", no_init)
        .def("bufferSize", pure_virtual(&Matrix::bufferSize))
        .def("rows", pure_virtual(&Matrix::rows))
        .def("cols", pure_virtual(&Matrix::cols))
        .def("stride", pure_virtual(&Matrix::stride))
        .def("valueType", pure_virtual(&Matrix::valueType))
        .def("_data", pure_virtual(reinterpret_cast<unsigned int (Matrix::*)()>(static_cast<uint8_t* const(Matrix::*)()>(&Matrix::data))))
    ;
}
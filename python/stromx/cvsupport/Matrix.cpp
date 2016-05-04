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

#include <stromx/runtime/Data.h>
#include <stromx/runtime/Image.h>

#include <stromx/cvsupport/Matrix.h>

#include <boost/lambda/lambda.hpp>
#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{  
    boost::shared_ptr<stromx::cvsupport::Matrix> allocateFromDimension(const unsigned int rows,
                                                                 const unsigned int cols,
                                                                 const stromx::runtime::Matrix::ValueType valueType)
    {
        return boost::shared_ptr<stromx::cvsupport::Matrix>(new stromx::cvsupport::Matrix(rows, cols, valueType), boost::lambda::_1);
    }
    
    boost::shared_ptr<stromx::cvsupport::Matrix> allocateFromFile(const std::string & filename)
    {
        return boost::shared_ptr<stromx::cvsupport::Matrix>(new stromx::cvsupport::Matrix(filename), boost::lambda::_1);
    } 
}

void exportMatrix()
{
    class_<stromx::cvsupport::Matrix, bases<stromx::runtime::Matrix>, boost::shared_ptr<stromx::cvsupport::Matrix> >("Matrix", no_init)
        .def("__init__", make_constructor(&allocateFromFile))
        .def("__init__", make_constructor(&allocateFromDimension))
        .def<void (stromx::cvsupport::Matrix::*)(const std::string &) const>("save", &stromx::cvsupport::Matrix::save)
        .def<void (stromx::cvsupport::Matrix::*)(const std::string &)>("open", &stromx::cvsupport::Matrix::open)
        .def<void (stromx::cvsupport::Matrix::*)(const unsigned int, const unsigned int, const Matrix::ValueType)>("resize", &stromx::cvsupport::Matrix::resize)
        .def("eye", &stromx::cvsupport::Matrix::eye)
        .staticmethod("eye") 
        .def("zeros", &stromx::cvsupport::Matrix::zeros)
        .staticmethod("zeros") 
    ;
}

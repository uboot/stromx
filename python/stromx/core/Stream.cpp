#include <stromx/core/Stream.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

void exportStream()
{    
    enum_<Stream::Status>("StreamStatus")
        .value("INACTIVE", Stream::INACTIVE)
        .value("ACTIVE", Stream::ACTIVE)
        .value("DEACTIVATING", Stream::DEACTIVATING)
        ;
        
    class_<Stream>("Stream")
        .def("name", &Stream::name, return_value_policy<boost::python::copy_const_reference>())
        .def("setName", &Stream::setName)
        .def("status", &Stream::status)
        .def("start", &Stream::start)
        .def("stop", &Stream::stop)
        .def("join", &Stream::join)
    ;
}
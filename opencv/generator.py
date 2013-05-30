# -*- coding: utf-8 -*-
"""
Created on Thu Jan 31 21:48:28 2013

@author: matz
"""

import os

import document
import package

from methodgenerator import *

class LibGenerator(object):
    p = None
    doc = None
    
    def save(self, package, printResult = False):
        self.p = package
        self.doc = document.Document()
        
        self.generate()
        
        if printResult:
            print self.doc.string()
        
class LibHeaderGenerator(LibGenerator):
    """
    >>> p = package.Package("imgproc", 0, 0, 1)
    >>> p.name = "OpenCV image processing"
    >>> m = package.Method("medianBlur")
    >>> p.methods.append(m)
    >>> g = LibHeaderGenerator()
    >>> g.save(p, True)    
    #ifndef STROMX_IMGPROC_IMGPROC_H
    #define STROMX_IMGPROC_IMGPROC_H
    <BLANKLINE>
    #include "Config.h"
    <BLANKLINE>
    namespace stromx
    {
        namespace runtime
        {
            class Registry;
        }
    }
    <BLANKLINE>
    extern "C"
    {
        STROMX_IMGPROC_API void stromxRegisterImgproc(stromx::runtime::Registry& registry);
    }
    <BLANKLINE>
    #endif // STROMX_IMGPROC_IMGPROC_H
    <BLANKLINE>
    """
    def generate(self):
        p = self.p.ident.constant()
        
        self.doc.line("#ifndef STROMX_{0}_{0}_H".format(p))
        self.doc.line("#define STROMX_{0}_{0}_H".format(p))
        self.doc.blank()
        
        self.doc.line('#include "Config.h"')
        self.doc.blank()
        
        self.doc.line("namespace stromx")
        self.doc.scopeEnter()
        self.doc.line("namespace runtime")
        self.doc.scopeEnter()
        self.doc.line("class Registry;")
        self.doc.scopeExit()
        self.doc.scopeExit()
        self.doc.blank()
        
        self.doc.line('extern "C"')
        self.doc.scopeEnter()
        self.doc.line("STROMX_{0}_API void stromxRegister{1}"
                  "(stromx::runtime::Registry& registry);"\
                  .format(p, self.p.ident.className()))
        self.doc.scopeExit()
        self.doc.blank()
        
        self.doc.line("#endif // STROMX_{0}_{0}_H".format(p))
        
        filename = "stromx/{0}/{1}.h".format(self.p.ident, 
                                             self.p.ident.className())
        with file(filename, "w") as f:
            f.write(self.doc.string())
           
class LibImplGenerator(LibGenerator):
    """
    >>> p = package.Package("imgproc", 0, 0, 1)
    >>> p.ident = package.Ident("imgproc")
    >>> p.name = "OpenCV image processing"
    >>> m = package.Method("medianBlur")
    >>> p.methods.append(m)
    >>> g = LibImplGenerator()
    >>> g.save(p, True)       
    #include "Imgproc.h"
    <BLANKLINE>
    #include "MedianBlur.h"
    #include <stromx/runtime/Registry.h>
    <BLANKLINE>
    void stromxRegisterImgproc(stromx::runtime::Registry& registry)
    {
        using namespace stromx::imgproc;
    <BLANKLINE>
        registry.registerOperator(new MedianBlur);
    }
    <BLANKLINE>
    """
    def generate(self):
        self.doc.line('#include "{0}.h"'.format(self.p.ident.className()))
        self.doc.blank()
        for m in self.p.methods:
            self.doc.line('#include "{0}.h"'.format(m.ident.className()))
        self.doc.line("#include <stromx/runtime/Registry.h>")
        self.doc.blank()
        
        self.doc.line("void stromxRegister{0}(stromx::runtime::Registry& registry)"\
            .format(self.p.ident.className()))
        self.doc.scopeEnter()
        self.doc.line("using namespace stromx::{0};".format(self.p.ident))
        self.doc.blank()
        
        for m in self.p.methods:
            self.doc.line("registry.registerOperator(new {0});"\
                .format(m.ident.className()))
        
        self.doc.scopeExit()
        
        filename = "stromx/{0}/{1}.cpp".format(self.p.ident, 
                                             self.p.ident.className())
        with file(filename, "w") as f:
            f.write(self.doc.string())
           
class LibTestGenerator(LibGenerator):
    """
    >>> p = package.Package("imgproc", 0, 0, 1)
    >>> g = LibTestGenerator()
    >>> g.save(p)      
    """
    def generate(self):
        text = """
/* Copied from http://www.evocomp.de/tutorials/tutorium_cppunit/howto_tutorial_cppunit.html */

#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

int main (int, char**)
{
    // Informiert Test-Listener ueber Testresultate
    CPPUNIT_NS :: TestResult testresult;

    // Listener zum Sammeln der Testergebnisse registrieren
    CPPUNIT_NS :: TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // Listener zur Ausgabe der Ergebnisse einzelner Tests
    CPPUNIT_NS :: BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // Test-Suite ueber die Registry im Test-Runner einfuegen
    CPPUNIT_NS :: TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS :: TestFactoryRegistry :: getRegistry ().makeTest ());
    testrunner.run (testresult);

    // Resultate im Compiler-Format ausgeben
    CPPUNIT_NS :: CompilerOutputter compileroutputter (&collectedresults, std::cerr);
    compileroutputter.write ();

    // Rueckmeldung, ob Tests erfolgreich waren
    return collectedresults.wasSuccessful () ? 0 : 1;
}
"""
        self.doc.text(text)
        
        filename = "stromx/{0}/test/main.cpp".format(self.p.ident)
        with file(filename, "w") as f:
            f.write(self.doc.string())

class CMakeGenerator(LibGenerator):
    """
    >>> p = package.Package("imgproc", 0, 0, 1)
    >>> p.name = "OpenCV image processing"
    >>> m = package.Method("medianBlur")
    >>> p.methods.append(m)
    >>> g = CMakeGenerator()
    >>> g.save(p, True)
    set (IMGPROC_VERSION_MAJOR 0)
    set (IMGPROC_VERSION_MINOR 0)
    set (IMGPROC_VERSION_PATCH 1)
    <BLANKLINE>
    configure_file (
        ${PROJECT_CURRENT_SOURCE_DIR}/Config.h.in
        ${PROJECT_CURRENT_SOURCE_DIR}/Config.h
    )
    <BLANKLINE>
    include_directories (
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
        ${Boost_INCLUDE_DIRS}
    )
    <BLANKLINE>
    set (SOURCES 
        Imgproc.cpp
        Utility.cpp
        MedianBlur.cpp
    )
    <BLANKLINE>
    add_library (stromx_imgproc SHARED ${SOURCES})
    <BLANKLINE>
    add_dependencies(stromx_imgproc stromx_runtime)
    <BLANKLINE>
    set(VERSION_STRING "${IMGPROC_VERSION_MAJOR}.${IMGPROC_VERSION_MINOR}.${IMGPROC_VERSION_PATCH}")
    <BLANKLINE>
    set_target_properties (stromx_imgproc PROPERTIES
        VERSION ${VERSION_STRING}
        SOVERSION ${VERSION_STRING}
    )
    <BLANKLINE>
    target_link_libraries (stromx_imgproc
        ${OpenCV_LIBS}
        stromx_runtime
        stromx_imgutil
    )
    <BLANKLINE>
    if(WIN32)
        install (TARGETS stromx_imgproc
            RUNTIME DESTINATION .
            LIBRARY DESTINATION ${LIB_DIR}
            ARCHIVE DESTINATION ${LIB_DIR}
        )
    else(WIN32)
        install (TARGETS stromx_imgproc
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION ${LIB_DIR}
            ARCHIVE DESTINATION ${LIB_DIR}
        )
    endif(WIN32)
    <BLANKLINE>
    if(BUILD_TESTS)
        add_subdirectory(test)
    endif(BUILD_TESTS)
    <BLANKLINE>
    """
    def generate(self):
        self.doc.line("set ({0}_VERSION_MAJOR {1})"\
            .format(self.p.ident.constant(), self.p.major))
        self.doc.line("set ({0}_VERSION_MINOR {1})"\
            .format(self.p.ident.constant(), self.p.major))
        self.doc.line("set ({0}_VERSION_PATCH {1})"\
            .format(self.p.ident.constant(), self.p.patch))
        self.doc.blank()
        
        self.doc.line("configure_file (")
        self.doc.increaseIndent()
        self.doc.line(r"${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in")
        self.doc.line(r"${CMAKE_CURRENT_BINARY_DIR}/Config.h")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("include_directories (")
        self.doc.increaseIndent()
        self.doc.line(r"${CMAKE_SOURCE_DIR}")
        self.doc.line(r"${CMAKE_BINARY_DIR}")
        self.doc.line(r"${Boost_INCLUDE_DIRS}")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("set (SOURCES ")
        self.doc.increaseIndent()
        self.doc.line("{0}.cpp".format(self.p.ident.className()))
        self.doc.line("Utility.cpp")
        for m in self.p.methods:
            self.doc.line("{0}.cpp".format(m.ident.className()))
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("add_library (stromx_{0} SHARED ${{SOURCES}})"\
            .format(self.p.ident))
        self.doc.blank()
        self.doc.line("add_dependencies(stromx_{0} stromx_runtime)"\
            .format(self.p.ident))
        self.doc.blank()
        
        self.doc.line('set(VERSION_STRING "${{{0}_VERSION_MAJOR}}.'
                  '${{{0}_VERSION_MINOR}}.${{{0}_VERSION_PATCH}}")'\
                  .format(self.p.ident.constant()))
        self.doc.blank()
        
        self.doc.line("set_target_properties (stromx_{0} PROPERTIES"\
            .format(self.p.ident))
        self.doc.increaseIndent()
        self.doc.line("VERSION ${VERSION_STRING}")
        self.doc.line("SOVERSION ${VERSION_STRING}")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("target_link_libraries (stromx_{0}"\
            .format(self.p.ident))
        self.doc.increaseIndent()
        self.doc.line("${OpenCV_LIBS}")
        self.doc.line("stromx_runtime")
        self.doc.line("stromx_imgutil")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("if(WIN32)")
        self.doc.increaseIndent()
        self.doc.line("install (TARGETS stromx_{0}".format(self.p.ident))
        self.doc.increaseIndent()
        self.doc.line("RUNTIME DESTINATION .")
        self.doc.line("LIBRARY DESTINATION ${LIB_DIR}")
        self.doc.line("ARCHIVE DESTINATION ${LIB_DIR}")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.decreaseIndent()
        self.doc.line("else(WIN32)")
        self.doc.increaseIndent()
        self.doc.line("install (TARGETS stromx_{0}".format(self.p.ident))
        self.doc.increaseIndent()
        self.doc.line("RUNTIME DESTINATION bin")
        self.doc.line("LIBRARY DESTINATION ${LIB_DIR}")
        self.doc.line("ARCHIVE DESTINATION ${LIB_DIR}")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.decreaseIndent()
        self.doc.line("endif(WIN32)")
        self.doc.blank()
        
        self.doc.line("install(DIRECTORY ${{CMAKE_SOURCE_DIR}}/stromx/{0}".format(self.p.ident))
        self.doc.increaseIndent()
        self.doc.line("DESTINATION include/stromx")
        self.doc.line('FILES_MATCHING PATTERN "*.h"')
        self.doc.line('PATTERN "test*" EXCLUDE')
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
    
        self.doc.line("install(DIRECTORY ${{CMAKE_BINARY_DIR}}/stromx/{0}".format(self.p.ident))
        self.doc.increaseIndent()
        self.doc.line("DESTINATION include/stromx")
        self.doc.line('FILES_MATCHING PATTERN "*.h"')
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("if(BUILD_TESTS)")
        self.doc.increaseIndent()
        self.doc.line("add_subdirectory(test)")
        self.doc.decreaseIndent()
        self.doc.line("endif(BUILD_TESTS)")
    
        filename = "stromx/{0}/CMakeLists.txt".format(self.p.ident)
        with file(filename, "w") as f:
            f.write(self.doc.string())
            

class PythonCMakeGenerator(LibGenerator):
    """
    >>> p = package.Package("imgproc", 0, 0, 1)
    >>> p.name = "OpenCV image processing"
    >>> m = package.Method("medianBlur")
    >>> p.methods.append(m)
    >>> g = PythonCMakeGenerator()
    >>> g.save(p, True)        
    """
    def generate(self):
        self.doc.line("project(python_stromx_{0})".format(self.p.ident))
        self.doc.blank()
        
        self.doc.line("set (SOURCES ")
        self.doc.increaseIndent()
        self.doc.line("{0}.cpp".format(self.p.ident.className()))
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("add_library ({0} SHARED ${{SOURCES}})"\
            .format(self.p.ident))
        self.doc.blank()
        self.doc.line("add_dependencies({0} stromx_{0})"\
            .format(self.p.ident))
        self.doc.blank()
        
        self.doc.line("include_directories (")
        self.doc.increaseIndent()
        self.doc.line(r"${CMAKE_SOURCE_DIR}")
        self.doc.line(r"${CMAKE_BINARY_DIR}")
        self.doc.line(r"${Boost_INCLUDE_DIRS}")
        self.doc.line(r"${OpenCV2_INCLUDE_DIR}")
        self.doc.line(r"${PYTHON_INCLUDE_DIRS}")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("target_link_libraries ({0}"\
            .format(self.p.ident))
        self.doc.increaseIndent()
        self.doc.line("${Boost_LIBRARIES}")
        self.doc.line("${PYTHON_LIBRARY}")
        self.doc.line("stromx_runtime")
        self.doc.line("stromx_imgutil")
        self.doc.line("stromx_{0}".format(self.p.ident))
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py "
                      "DESTINATION ${CMAKE_CURRENT_BINARY_DIR})")
        self.doc.blank()
        
        self.doc.line("if(PYTHON_INSTALL_DIR)")
        self.doc.increaseIndent()
        self.doc.line("install(DIRECTORY . ")
        self.doc.increaseIndent()
        self.doc.line("DESTINATION ${{PYTHON_INSTALL_DIR}}/stromx/{0}".format(self.p.ident))
        self.doc.line('FILES_MATCHING PATTERN "*.py"')
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.line("install(TARGETS imgutil")
        self.doc.increaseIndent()
        self.doc.line("DESTINATION ${{PYTHON_INSTALL_DIR}}/stromx/{0}".format(self.p.ident))
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.decreaseIndent()
        self.doc.line("endif()")
    
        filename = "python/stromx/{0}/CMakeLists.txt".format(self.p.ident)
        with file(filename, "w") as f:
            f.write(self.doc.string())
            
class PythonInitGenerator(LibGenerator):
    """
    >>> p = package.Package("imgproc", 0, 0, 1)
    >>> p.name = "OpenCV image processing"
    >>> m = package.Method("medianBlur")
    >>> p.methods.append(m)
    >>> g = PythonInitGenerator()
    >>> g.save(p, True)        
    """
    def generate(self):
        self.doc.line("import stromx.runtime".format(self.p.ident))
        self.doc.blank()
        self.doc.line("from lib{0} import *".format(self.p.ident))
        
        filename = "python/stromx/{0}/__init__.py".format(self.p.ident)
        with file(filename, "w") as f:
            f.write(self.doc.string())
            
class PythonExportGenerator(LibGenerator):
    """
    >>> p = package.Package("imgproc", 0, 0, 1)
    >>> p.name = "OpenCV image processing"
    >>> m = package.Method("medianBlur")
    >>> p.methods.append(m)
    >>> g = PythonExportGenerator()
    >>> g.save(p, True)        
    """
    def generate(self):
        self.doc.line("#include <boost/python.hpp>")
        self.doc.line("#include <stromx/runtime/Registry.h>")
        self.doc.line((
            "#include <stromx/{0}/{1}.h>"
        ).format(self.p.ident, self.p.ident.className()))
        
        for m in self.p.methods:
            self.doc.line("#include <stromx/{0}/{1}.h>".format(
                                            self.p.ident, m.ident.className()))
        
        self.doc.line("#include <python/stromx/runtime/ExportOperatorKernel.h>")
        self.doc.blank()
        
        self.doc.line("using namespace boost::python;")
        self.doc.line("using namespace stromx::{0};".format(self.p.ident))
        self.doc.blank()
    
        self.doc.line("BOOST_PYTHON_MODULE(lib{0})".format(self.p.ident))
        self.doc.scopeEnter()
        self.doc.line('def("register{0}", stromxRegister{0});'.format(self.p.ident.className()))
        self.doc.blank()
        
        for m in self.p.methods:
            l = 'stromx::python::exportOperatorKernel<{0}>("{0}");'.format(
                                                        m.ident.className())
            self.doc.line(l)
        
        self.doc.scopeExit()
        
        filename = "python/stromx/{0}/{1}.cpp".format(self.p.ident,
                                                      self.p.ident.className())
        with file(filename, "w") as f:
            f.write(self.doc.string())
            
class TestCMakeGenerator(LibGenerator):
    """
    >>> p = package.Package("imgproc", 0, 0, 1)
    >>> p.name = "OpenCV image processing"
    >>> m = package.Method("medianBlur")
    >>> p.methods.append(m)
    >>> p.testFiles.append("lenna.jpg")
    >>> g = TestCMakeGenerator()
    >>> g.save(p, True)
    add_test(NAME stromx_imgproc_test COMMAND stromx_imgproc_test)
    <BLANKLINE>
    if(MSVC)
        add_definitions(/DSTROMX_EXAMPLE_STATIC)
    endif(MSVC)
    <BLANKLINE>
    include_directories (
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
        ${CPPUNIT_INCLUDE_DIR}
    )
    <BLANKLINE>
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    <BLANKLINE>
    set (SOURCES 
        ../MedianBlur.cpp
        ../Utility.cpp
        MedianBlurTest.cpp
        main.cpp
    )
    <BLANKLINE>
    add_executable(stromx_imgproc_test ${SOURCES})
    <BLANKLINE>
    add_dependencies(stromx_imgproc_test stromx_imgproc)
    <BLANKLINE>
    target_link_libraries (stromx_imgproc_test
        ${CPPUNIT_LIBRARY}
        ${CMAKE_DL_LIBS}
        ${OpenCV_LIBS}
        stromx_runtime
        stromx_imgutil
    )
    <BLANKLINE>
    <BLANKLINE>
    """
    def generate(self):
        self.doc.line((
            "add_test(NAME stromx_{0}_test COMMAND stromx_{0}_test)"
        ).format(self.p.ident))
        self.doc.blank()
        self.doc.line("if(MSVC)")
        self.doc.increaseIndent()
        self.doc.line("add_definitions(/DSTROMX_EXAMPLE_STATIC)")
        self.doc.decreaseIndent()
        self.doc.line("endif(MSVC)")
        self.doc.blank()
        
        self.doc.line("include_directories (")
        self.doc.increaseIndent()
        self.doc.line(r"${CMAKE_SOURCE_DIR}")
        self.doc.line(r"${CMAKE_BINARY_DIR}")
        self.doc.line(r"${CPPUNIT_INCLUDE_DIR}")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        
        for f in self.p.testFiles:
            self.doc.line(("file(COPY ${{CMAKE_CURRENT_SOURCE_DIR}}/{0} "
                "DESTINATION ${{CMAKE_CURRENT_BINARY_DIR}})").format(f))
        self.doc.blank()
        
        self.doc.line("set (SOURCES ")
        self.doc.increaseIndent()
        for m in self.p.methods:
            self.doc.line("../{0}.cpp".format(m.ident.className()))
        self.doc.line("../Utility.cpp")
        for m in self.p.methods:
            self.doc.line("{0}Test.cpp".format(m.ident.className()))
        self.doc.line("main.cpp")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("add_executable(stromx_{0}_test ${{SOURCES}})"\
            .format(self.p.ident))
        self.doc.blank()
        self.doc.line("add_dependencies(stromx_{0}_test stromx_{0})"\
            .format(self.p.ident))
        self.doc.blank()
        
        self.doc.line("target_link_libraries (stromx_{0}_test"\
            .format(self.p.ident))
        self.doc.increaseIndent()
        self.doc.line("${CPPUNIT_LIBRARY}")
        self.doc.line("${CMAKE_DL_LIBS}")
        self.doc.line("${OpenCV_LIBS}")
        self.doc.line("stromx_runtime")
        self.doc.line("stromx_imgutil")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
    
    
        
        filename = "stromx/{0}/test/CMakeLists.txt".format(self.p.ident)
        with file(filename, "w") as f:
            f.write(self.doc.string())
            
class UtilityHeaderGenerator(LibGenerator):
    """
    >>> p = package.Package("imgproc", 0, 0, 1)
    >>> g = UtilityHeaderGenerator()
    >>> g.save(p, True)
    #ifndef STROMX_IMGPROC_UTILITY_H
    #define STROMX_IMGPROC_UTILITY_H
    <BLANKLINE>
    namespace stromx
    {
        namespace imgproc
        {
    <BLANKLINE>
        }
    }
    #endif // STROMX_IMGPROC_UTILITY_H
    <BLANKLINE>
    """
    def generate(self):
        p = self.p.ident.constant()
        guard = "STROMX_{0}_UTILITY_H".format(p)
        
        self.doc.line("#ifndef {0}".format(guard))
        self.doc.line("#define {0}".format(guard))
        self.doc.blank()
        
        
        includes = set()
        for f in self.p.functions:
            includes = includes.union(set(f.dclIncludes))
        
        for i in includes:
            self.doc.line("#include {0}".format(i))
        self.doc.blank()
        
        self.doc.line("namespace stromx")
        self.doc.scopeEnter()
        self.doc.line("namespace {0}".format(self.p.ident))
        self.doc.scopeEnter()
        
        for f in self.p.functions:
            self.doc.document(f.declaration)
            self.doc.blank()
        self.doc.blank()
        
        self.doc.scopeExit()
        self.doc.scopeExit()
        
        self.doc.line("#endif // {0}".format(guard))
        
        
        filename = "stromx/{0}/Utility.h".format(self.p.ident)
        with file(filename, "w") as f:
            f.write(self.doc.string())
       
class UtilityImplGenerator(LibGenerator):
    """
    >>> p = package.Package("imgproc", 0, 0, 1)
    >>> g = UtilityImplGenerator()
    >>> g.save(p, True)
    #include "Utility.h"
    <BLANKLINE>
    namespace stromx
    {
        namespace imgproc
        {
    <BLANKLINE>
        }
    }
    <BLANKLINE>
    """
    def generate(self):
        self.doc.line('#include "Utility.h"')
        self.doc.blank()
        
        includes = set()
        for f in self.p.functions:
            includes = includes.union(set(f.dfnIncludes))
        
        for i in includes:
            self.doc.line("#include {0}".format(i))
        self.doc.blank()
        
        self.doc.line("namespace stromx")
        self.doc.scopeEnter()
        self.doc.line("namespace {0}".format(self.p.ident))
        self.doc.scopeEnter()
        
        for f in self.p.functions:
            self.doc.document(f.definition)
            self.doc.blank()
        self.doc.blank()
        
        self.doc.scopeExit()
        self.doc.scopeExit()
        
        filename = "stromx/{0}/Utility.cpp".format(self.p.ident)
        with file(filename, "w") as f:
            f.write(self.doc.string()) 
            
class ConfigGenerator(LibGenerator):
    """
    >>> p = package.Package("imgproc", 0, 0, 1)
    >>> p.name = "OpenCV image processing"
    >>> g = ConfigGenerator()
    >>> g.save(p, True)
    #ifndef STROMX_IMGPROC_CONFIG_H
    #define STROMX_IMGPROC_CONFIG_H
    <BLANKLINE>
    #define STROMX_IMGPROC_VERSION_MAJOR @IMGPROC_VERSION_MAJOR@
    #define STROMX_IMGPROC_VERSION_MINOR @IMGPROC_VERSION_MINOR@
    #define STROMX_IMGPROC_VERSION_PATCH @IMGPROC_VERSION_PATCH@
    <BLANKLINE>
    #define STROMX_IMGPROC_PACKAGE_NAME "Imgproc"
    <BLANKLINE>
    #ifdef WIN32
        #ifdef STROMX_IMGPROC_STATIC
            #define STROMX_IMGPROC_STATIC
        #else // STROMX_IMGPROC_STATIC
            #ifdef stromx_imgproc_EXPORTS
                #define STROMX_IMGPROC_API __declspec(dllexport)
            #else // stromx_imgproc_EXPORTS
                #define STROMX_IMGPROC_API __declspec(dllimport)
            #endif // stromx_imgproc_EXPORTS
        #endif // STROMX_IMGPROC_STATIC
    #else // WIN32
        #define STROMX_IMGPROC_API
    #endif // WIN32
    <BLANKLINE>
    #endif // STROMX_IMGPROC_CONFIG_H
    <BLANKLINE>
    """
    def generate(self):
        p = self.p.ident.constant()
        guard = "STROMX_{0}_CONFIG_H".format(p)
        
        self.doc.line("#ifndef {0}".format(guard))
        self.doc.line("#define {0}".format(guard))
        self.doc.blank()
        
        version = "{0}_VERSION".format(p)
        self.doc.line("#define STROMX_{0}_MAJOR @{0}_MAJOR@".format(version))
        self.doc.line("#define STROMX_{0}_MINOR @{0}_MINOR@".format(version))
        self.doc.line("#define STROMX_{0}_PATCH @{0}_PATCH@".format(version))
        self.doc.blank()
        
        self.doc.line('#define STROMX_{0}_PACKAGE_NAME "{1}"'\
            .format(p, self.p.ident.className()))
        self.doc.blank()
        
        self.doc.line("#ifdef WIN32")
        self.doc.increaseIndent()
        self.doc.line("#ifdef STROMX_{0}_STATIC".format(p))
        self.doc.increaseIndent()
        self.doc.line("#define STROMX_{0}_STATIC".format(p))
        self.doc.decreaseIndent()
        self.doc.line("#else // STROMX_{0}_STATIC".format(p))
        self.doc.increaseIndent()
        self.doc.line("#ifdef stromx_{0}_EXPORTS".format(self.p.ident))
        self.doc.increaseIndent()
        self.doc.line("#define STROMX_{0}_API __declspec(dllexport)".format(p))
        self.doc.decreaseIndent()
        self.doc.line("#else // stromx_{0}_EXPORTS".format(self.p.ident))
        self.doc.increaseIndent()
        self.doc.line("#define STROMX_{0}_API __declspec(dllimport)".format(p))
        self.doc.decreaseIndent()
        self.doc.line("#endif // stromx_{0}_EXPORTS".format(self.p.ident))
        self.doc.decreaseIndent()
        self.doc.line("#endif // STROMX_{0}_STATIC".format(p))
        self.doc.decreaseIndent()
        self.doc.line("#else // WIN32")
        self.doc.increaseIndent()
        self.doc.line("#define STROMX_{0}_API".format(p))
        self.doc.decreaseIndent()
        self.doc.line("#endif // WIN32")
        self.doc.blank() 
        
        self.doc.line("#endif // {0}".format(guard))

        filename = "stromx/{0}/Config.h.in".format(self.p.ident)
        with file(filename, "w") as f:
            f.write(self.doc.string())
            
def generatePackageFiles(package):
    g = LibHeaderGenerator()
    g.save(package)
    
    g = LibImplGenerator()
    g.save(package)
    
    g = ConfigGenerator()
    g.save(package)
    
    g = UtilityHeaderGenerator()
    g.save(package)
    
    g = UtilityImplGenerator()
    g.save(package)
    
    g = CMakeGenerator()
    g.save(package)
        
    g = LibTestGenerator()
    g.save(package)
    
    g = TestCMakeGenerator()
    g.save(package)
    
    g = PythonCMakeGenerator()
    g.save(package)
        
    g = PythonInitGenerator()
    g.save(package)
    
    g = PythonExportGenerator()
    g.save(package)
    
    for m in package.methods:
        generateMethodFiles(package, m)
        
if __name__ == "__main__":
    import doctest
    doctest.testmod()
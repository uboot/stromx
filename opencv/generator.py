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
    """
    Base class of generator classes which output files depending on a package
    but not on a specific operator.
    """
    p = None
    doc = None
    
    def save(self, package, printResult = False):
        """
        Writes the output of the generator for the input package to the current
        document and optionally prints it to the standard output.
        """
        self.p = package
        self.doc = document.Document()
        
        self.generate()
        
        if printResult:
            print self.doc.string()
        
class LibHeaderGenerator(LibGenerator):
    """
    Generator of package header files.
    """
    def generate(self):
        p = self.p.ident.upper()
        
        self.doc.line("#ifndef STROMX_{0}_{0}_H".format(p))
        self.doc.line("#define STROMX_{0}_{0}_H".format(p))
        self.doc.blank()
        
        self.doc.line('#include "stromx/{0}/Config.h"'.format(self.p.ident))
        self.doc.blank()
        
        
        self.doc.namespaceEnter("stromx")
        self.doc.namespaceEnter("runtime")
        self.doc.line("class Registry;")
        self.doc.namespaceExit()
        self.doc.namespaceExit()
        self.doc.blank()
        
        self.doc.line('extern "C"')
        self.doc.scopeEnter()
        self.doc.line("STROMX_{0}_API void stromx{1}Register"
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
    Generator of package implementation files.
    """
    def generate(self):
        self.doc.line('#include "stromx/{0}/{1}.h"'.format(
            self.p.ident, self.p.ident.className()
        ))
        self.doc.blank()
        for m in self.p.methods:
            self.doc.line('#include "stromx/{0}/{1}.h"'.format(
            self.p.ident, m.ident.className()
        ))
        self.doc.line("#include <stromx/runtime/Locale.h>")
        self.doc.line("#include <stromx/runtime/Registry.h>")
        self.doc.blank()
        
        self.doc.namespaceEnter("stromx")
        self.doc.namespaceEnter(self.p.ident)
        self.doc.line("std::locale locale;")
        self.doc.namespaceExit()
        self.doc.namespaceExit()
        self.doc.blank()
        
        self.doc.line("void stromx{0}Register(stromx::runtime::Registry& registry)"\
            .format(self.p.ident.className()))
        self.doc.scopeEnter()
        self.doc.line("using namespace stromx::{0};".format(self.p.ident))
        self.doc.blank()
        
        p = self.p.ident.upper()
        self.doc.line(("locale = stromx::runtime::Locale::generate("
                       "STROMX_{0}_LOCALE_DIR, STROMX_{0}_LOCALE_DOMAIN);"
                      ).format(p))
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
    Generator of the package test main() function file.
    """
    def generate(self):
        text = """
// copied from cppunit/TestRunner.h

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include <stdexcept>

int main( int argc, char* argv[] )
{
    std::string testPath = (argc > 1) ? std::string(argv[1]) : "";

    // Create the event manager and test controller
    CppUnit::TestResult controller;

    // Add a listener that colllects test result
    CppUnit::TestResultCollector result;
    controller.addListener( &result );        

    // Add a listener that print dots as test run.
    CppUnit::BriefTestProgressListener progress;
    controller.addListener( &progress );      

    // Add the top suite to the test runner
    CppUnit::TestRunner runner;
    runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() );   
    try
    {
        std::cout << "Running "  <<  testPath;
        runner.run( controller, testPath );

        std::cerr << std::endl;

        // Print test in a compiler compatible format.
        CppUnit::CompilerOutputter outputter( &result, std::cerr );
        outputter.write();                      
    }
    catch ( std::invalid_argument &e )  // Test path not resolved
    {
        std::cerr  <<  std::endl  
                    <<  "ERROR: "  <<  e.what()
                    << std::endl;
        return 0;
    }

    return result.wasSuccessful() ? 0 : 1;
}
"""
        self.doc.text(text)
        
        filename = "stromx/{0}/test/main.cpp".format(self.p.ident)
        with file(filename, "w") as f:
            f.write(self.doc.string())

class CMakeGenerator(LibGenerator):
    """
    Generator of the CMake file for the package.
    """
    def generate(self):
        self.doc.line("set (STROMX_{0}_VERSION_MAJOR {1})"\
            .format(self.p.ident.upper(), self.p.major))
        self.doc.line("set (STROMX_{0}_VERSION_MINOR {1})"\
            .format(self.p.ident.upper(), self.p.major))
        self.doc.line("set (STROMX_{0}_VERSION_PATCH {1})"\
            .format(self.p.ident.upper(), self.p.patch))
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
        
        self.doc.line('set(VERSION_STRING "${{STROMX_{0}_VERSION_MAJOR}}.'
                  '${{STROMX_{0}_VERSION_MINOR}}.${{STROMX_{0}_VERSION_PATCH}}")'\
                  .format(self.p.ident.constant()))
        self.doc.blank()
        
        self.doc.line("set_target_properties (stromx_{0} PROPERTIES"\
            .format(self.p.ident))
        self.doc.increaseIndent()
        self.doc.line("VERSION ${VERSION_STRING}")
        self.doc.line("SOVERSION ${VERSION_STRING}")
        self.doc.line('FOLDER "library"')
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("target_link_libraries (stromx_{0}"\
            .format(self.p.ident))
        self.doc.increaseIndent()
        self.doc.line("${OpenCV_LIBS}")
        self.doc.line("stromx_runtime")
        self.doc.line("stromx_cvsupport")
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
    Generator of the CMake file for the Python bindings of the package.
    """
    def generate(self):
        self.doc.line("set (SOURCES ")
        self.doc.increaseIndent()
        self.doc.line("{0}.cpp".format(self.p.ident.className()))
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("add_library ({0} SHARED ${{SOURCES}})"\
            .format(self.p.ident))
        self.doc.blank()
        self.doc.line('set_target_properties ({0} PROPERTIES FOLDER "python")'\
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
        self.doc.line("stromx_cvsupport")
        self.doc.line("stromx_{0}".format(self.p.ident))
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py "
                      "${CMAKE_CURRENT_BINARY_DIR}/__init__.py COPYONLY)")
        self.doc.blank()
        
        self.doc.line("if(PYTHON_INSTALL_DIR)")
        self.doc.increaseIndent()
        self.doc.line("install(DIRECTORY . ")
        self.doc.increaseIndent()
        self.doc.line("DESTINATION ${{PYTHON_INSTALL_DIR}}/stromx/{0}".format(self.p.ident))
        self.doc.line('FILES_MATCHING PATTERN "*.py"')
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.line("install(TARGETS {0}".format(self.p.ident))
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
    Generator of the Python module file (__init__.py) of the package.
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
    Generator of the main export function file of the Python wrapper of the 
    package.
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
        self.doc.line('def("register", stromx{0}Register);'.format(self.p.ident.className()))
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
    Generator of the CMake file for the package tests.
    """
    def generate(self):
        self.doc.line((
            "add_test(NAME stromx_{0}_test COMMAND stromx_{0}_test)"
        ).format(self.p.ident))
        self.doc.blank()
        self.doc.line("if(MSVC)")
        self.doc.increaseIndent()
        self.doc.line((
            "add_definitions(/DSTROMX_{0}_STATIC)"
            ).format(self.p.ident.upper()))
        self.doc.decreaseIndent()
        self.doc.line("endif(MSVC)")
        self.doc.blank()
        
        self.doc.line("include_directories (")
        self.doc.increaseIndent()
        self.doc.line(r"${CMAKE_SOURCE_DIR}")
        self.doc.line(r"${CMAKE_BINARY_DIR}")
        self.doc.line(r"${CPPUNIT_INCLUDE_DIR}")
        self.doc.line(r"${Boost_INCLUDE_DIRS}")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        
        for f in self.p.testFiles:
            self.doc.line(("configure_file(${{CMAKE_CURRENT_SOURCE_DIR}}/{0} "
                "${{CMAKE_CURRENT_BINARY_DIR}}/{0} COPYONLY)").format(f))
        self.doc.blank()
        
        self.doc.line("set (SOURCES ")
        self.doc.increaseIndent()
        for m in self.p.methods:
            self.doc.line("../{0}.cpp".format(m.ident.className()))
        self.doc.line("../Utility.cpp")
        self.doc.line("../{0}.cpp".format(self.p.ident.className()))
        for m in self.p.methods:
            self.doc.line("{0}Test.cpp".format(m.ident.className()))
        self.doc.line("main.cpp")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("add_executable(stromx_{0}_test ${{SOURCES}})"\
            .format(self.p.ident))
        self.doc.blank()
        self.doc.line((
            'set_target_properties(stromx_{0}_test PROPERTIES FOLDER '
            '"test")').format(self.p.ident))
        self.doc.blank()
        
        self.doc.line("target_link_libraries (stromx_{0}_test"\
            .format(self.p.ident))
        self.doc.increaseIndent()
        self.doc.line("${CPPUNIT_LIBRARY}")
        self.doc.line("${CMAKE_DL_LIBS}")
        self.doc.line("${OpenCV_LIBS}")
        self.doc.line("stromx_runtime")
        self.doc.line("stromx_cvsupport")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
    
    
        
        filename = "stromx/{0}/test/CMakeLists.txt".format(self.p.ident)
        with file(filename, "w") as f:
            f.write(self.doc.string())
            
class UtilityHeaderGenerator(LibGenerator):
    """
    Generator of the package utility header.
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
    Generator of the package utility implementation.
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
    Generator of the package configure header template.
    """
    def generate(self):
        p = self.p.ident.upper()
        guard = "STROMX_{0}_CONFIG_H".format(p)
        
        self.doc.line("#ifndef {0}".format(guard))
        self.doc.line("#define {0}".format(guard))
        self.doc.blank()
        
        version = "{0}_VERSION".format(p)
        self.doc.line("#define STROMX_{0}_MAJOR @STROMX_{0}_MAJOR@".format(version))
        self.doc.line("#define STROMX_{0}_MINOR @STROMX_{0}_MINOR@".format(version))
        self.doc.line("#define STROMX_{0}_PATCH @STROMX_{0}_PATCH@".format(version))
        self.doc.blank()
        
        packageName = "cv::{0}".format(str(self.p.ident)[2:])
        self.doc.line('#define STROMX_{0}_PACKAGE_NAME "{1}"'\
            .format(p, packageName))
        self.doc.line('#define STROMX_{0}_LOCALE_DOMAIN "lib{1}"'\
            .format(p, self.p.ident))
        self.doc.line('#define STROMX_{0}_LOCALE_DIR "@LOCALE_DIR@"'.format(p))
        self.doc.blank()
        
        self.doc.line("#ifdef WIN32")
        self.doc.increaseIndent()
        self.doc.line("#ifdef STROMX_{0}_STATIC".format(p))
        self.doc.increaseIndent()
        self.doc.line("#define STROMX_{0}_API".format(p))
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
            
class LocaleGenerator(LibGenerator):
    """
    Generator of the locale header.
    """
    def generate(self):
        p = self.p.ident.upper()
        
        self.doc.line("#ifndef STROMX_{0}_LOCALE_H".format(p))
        self.doc.line("#define STROMX_{0}_LOCALE_H".format(p))
        self.doc.blank()
        
        self.doc.line('#include <stromx/runtime/Locale.h>')
        self.doc.blank()
        
        self.doc.namespaceEnter("stromx")
        self.doc.namespaceEnter(self.p.ident)
        self.doc.line("extern std::locale locale;")
        self.doc.namespaceExit()
        self.doc.namespaceExit()
        self.doc.blank()
        
        self.doc.line("#endif // STROMX_{0}_LOCALE_H".format(p))
        
        filename = "stromx/{0}/Locale.h".format(self.p.ident)
        with file(filename, "w") as f:
            f.write(self.doc.string())
            
def generatePackageFiles(package):
    """
    Generates all files for the given package (including all operator files).
    """
    g = LibHeaderGenerator()
    g.save(package)
    
    g = LibImplGenerator()
    g.save(package)
    
    g = ConfigGenerator()
    g.save(package)
    
    g = LocaleGenerator()
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
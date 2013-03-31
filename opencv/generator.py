# -*- coding: utf-8 -*-
"""
Created on Thu Jan 31 21:48:28 2013

@author: matz
"""

import document
import package

from methodgenerator import *

class LibGenerator(object):
    p = None
    doc = None
    
    def save(self, package, printResult):
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
        self.doc.line("namespace runtime".format(self.p.ident))
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
        
        with file("{0}.h".format(self.p.ident.className()), "w") as f:
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
        
        with file("{0}.cpp".format(self.p.ident.className()), "w") as f:
            f.write(self.doc.string())


class CMakeGenerator(LibGenerator):
    """
    >>> p = package.Package("imgproc", 0, 0, 1)
    >>> p.name = "OpenCV image processing"
    >>> m = package.Method("medianBlur")
    >>> p.methods.append(m)
    >>> g = CMakeGenerator()
    >>> g.save(p, True)        
    project(stromx_imgproc)
    <BLANKLINE>
    set (IMGPROC_VERSION_MAJOR 0)
    set (IMGPROC_VERSION_MINOR 0)
    set (IMGPROC_VERSION_PATCH 1)
    <BLANKLINE>
    configure_file (
        ${PROJECT_SOURCE_DIR}/Config.h.in
        ${PROJECT_SOURCE_DIR}/Config.h
    )
    <BLANKLINE>
    include_directories (
        ../..
        ${Boost_INCLUDE_DIRS}
    )
    <BLANKLINE>
    set (SOURCES 
        Imgproc.cpp
        MedianBlur.cpp
    )
    <BLANKLINE>
    add_library (stromx_imgproc SHARED ${SOURCES})
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
        stromx_example
    )
    <BLANKLINE>
    <BLANKLINE>
    """
    def generate(self):
        self.doc.line("project(stromx_{0})".format(self.p.ident))
        self.doc.blank()
        self.doc.line("set ({0}_VERSION_MAJOR {1})"\
            .format(self.p.ident.constant(), self.p.major))
        self.doc.line("set ({0}_VERSION_MINOR {1})"\
            .format(self.p.ident.constant(), self.p.major))
        self.doc.line("set ({0}_VERSION_PATCH {1})"\
            .format(self.p.ident.constant(), self.p.patch))
        self.doc.blank()
        
        self.doc.line("configure_file (")
        self.doc.increaseIndent()
        self.doc.line(r"${PROJECT_SOURCE_DIR}/Config.h.in")
        self.doc.line(r"${PROJECT_SOURCE_DIR}/Config.h")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("include_directories (")
        self.doc.increaseIndent()
        self.doc.line(r"../..")
        self.doc.line(r"${Boost_INCLUDE_DIRS}")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("set (SOURCES ")
        self.doc.increaseIndent()
        self.doc.line("{0}.cpp".format(self.p.ident.className()))
        for m in self.p.methods:
            self.doc.line("{0}.cpp".format(m.ident.className()))
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
        
        self.doc.line("add_library (stromx_{0} SHARED ${{SOURCES}})"\
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
        self.doc.line("stromx_example")
        self.doc.decreaseIndent()
        self.doc.line(")")
        self.doc.blank()
    
        with file("CMakeLists.txt", "w") as f:
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
    #define IMGPROC_VERSION_MAJOR @IMGPROC_VERSION_MAJOR@
    #define IMGPROC_VERSION_MINOR @IMGPROC_VERSION_MINOR@
    #define IMGPROC_VERSION_PATCH @IMGPROC_VERSION_PATCH@
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
        self.doc.line("#define {0}_MAJOR @{0}_MAJOR@".format(version))
        self.doc.line("#define {0}_MINOR @{0}_MINOR@".format(version))
        self.doc.line("#define {0}_PATCH @{0}_PATCH@".format(version))
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

        with file("Config.h.in", "w") as f:
            f.write(self.doc.string())
        
if __name__ == "__main__":
    import doctest
    doctest.testmod()
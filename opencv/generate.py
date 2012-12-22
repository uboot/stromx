# -*- coding: utf-8 -*-
"""
Created on Fri Dec 21 17:07:19 2012

@author: matz
"""

from package import *

class MethodGenerator(object):
    def __init__(self, package, method):
        self.p = package
        self.m = method
        self.lines = []
        self.indent = 0
        
    def generate(self):
        self.includeGuardEnter()
        self.blank()
        self.headerIncludes()
        self.blank()
        self.namespaceEnter()
        self.classEnter()
        self.access("public")
        self.classExit()
        self.namespaceExit()
        self.blank()
        self.includeGuardExit()
        
    def document(self):
        s = ""
        for line in self.lines:
            s += line + "\n"
        return s
        
    def increaseIndent(self):
        self.indent += 4
        
    def decreaseIndent(self):
        self.indent -= 4
    
    def line(self, line):
        self.lines.append("{0}{1}".format(" " * self.indent, line))
        
    def blank(self):
        self.line("\n")
    
    def headerIncludes(self):
        self.line('#include "Config.h"');
        self.line('#include <stromx/core/Enum.h>');
        self.line('#include <stromx/core/OperatorKernel.h>');
        self.line('#include <stromx/core/Primitive.h>');
        
    def includeGuardEnter(self):
        self.line("#ifndef {0}".format(self.includeGuard()))
        self.line("#define {0}".format(self.includeGuard()))
        
    def namespaceEnter(self):
        self.line("namespace stromx")
        self.line("{")
        self.increaseIndent()
        self.line("namespace {0}".format(self.p.ident.lower()))
        self.line("{")
        self.increaseIndent()
        
    def classEnter(self):
        self.line("class {0} {1} : public stromx.core.OperatorKernel".format(
                    self.apiDecl(), self.className()))
        self.line("{")
        self.increaseIndent()
        
    def access(self, key):
        self.decreaseIndent()
        self.line("{0}:".format(key))
        self.increaseIndent()
    
    def classExit(self):
        self.decreaseIndent()
        self.line("};")
    
    def namespaceExit(self):
        self.decreaseIndent()
        self.line("}")
        self.decreaseIndent()
        self.line("}")
        
    def includeGuardExit(self):
        self.line("#endif // {0}".format(self.includeGuard()))
        
    def includeGuard(self):
        return "STROMX_{0}_{1}_H".format(p.ident.upper(), m.ident.upper())
    
    def apiDecl(self):
        return "STROMX_{0}_API".format(p.ident.upper())
        
    def className(self):
        return "{0}{1}".format(self.m.ident[0].upper(), self.m.ident[1:])
    
if __name__ == "__main__":
    p = Package()
    p.ident = "imgproc"
    p.name = "OpenCV image processing"
    
    m = Method()
    m.ident = "medianBlur"
    m.name = "Median Blur"
    
    arg1 = Input()
    arg1.ident = "src"
    arg1.name = "source"
    arg1.cvType = CvType.MAT
    arg1.dataType = DataType.IMAGE
    
    arg2 = Output()
    arg2.ident = "dst"
    arg2.name = "destination"
    arg2.cvType = CvType.MAT
    arg2.dataType = DataType.IMAGE
    arg2.inPlace = arg1
    
    arg3 = NumericParameter()
    arg3.ident = "ksize"
    arg3.cvType = CvType.INT
    arg3.dataType = DataType.UINT_32
    arg3.default = 3
    arg3.minValue = 1
    arg3.rules.append(OddRule())
    
    m.args = [arg1, arg2, arg3]
    
    g = MethodGenerator(p, m)
    g.generate()
    print g.document()
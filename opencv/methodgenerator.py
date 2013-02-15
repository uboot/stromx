# -*- coding: utf-8 -*-

import datatype
import document
import package
import cvtype

class ArgumentVisitor(object):
    def visitInput(self, inputArg):
        pass
        
    def visitParameter(self, parameter):
        pass
        
    def visitNumericParameter(self, numericParameter):
        pass
        
    def visitConstant(self, const):
        pass
        
    def visitEnumParameter(self, enumParameter):
        pass
        
    def visitRefInput(self, refInput):
        pass
        
    def visitAllocation(self, allocation):
        pass
        
    def visitOutput(self, output):
        pass
        
    def visitSize(self, size):
        pass
        
        
class MethodGenerator(ArgumentVisitor):
    p = None
    m = None
    doc = None
    
    def save(self, package, method, printResult):
        self.p = package
        self.m = method
        self.doc = document.Document()
        
        self.generate()
        
        if printResult:
            print self.doc.string()
            
    def visitAll(self, visitor):
        for opt in self.m.options:
            for arg in opt.args:
                arg.accept(visitor)
            
    def visitOption(self, visitor, opt):
        for arg in opt.args:
            arg.accept(visitor)
            
class OpHeaderGenerator(MethodGenerator):
    """
    >>> p = package.Package("imgproc", 0, 0, 1)
    >>> p.name = "OpenCV image processing"
    >>> m = package.Method("medianBlur")
    >>> arg1 = package.Argument("src", "Source", cvtype.Mat(), datatype.Image())
    >>> arg2 = package.Argument("dst", "Destination", cvtype.Mat(), datatype.Image())
    >>> arg3 = package.NumericParameter("ksize", "Kernel size", cvtype.Int(), datatype.UInt32())
    >>> opt = package.Option("manual")
    >>> opt.args.extend([package.Input(arg1, True), package.Output(arg2), arg3])
    >>> m.options.append(opt)
    >>> opt = package.Option("allocate")
    >>> opt.args.extend([package.Input(arg1), package.Allocation(arg2), arg3])
    >>> m.options.append(opt)
    >>> g = OpHeaderGenerator()
    >>> g.save(p, m, True)
    <BLANKLINE>
    """
    class InputEnumVisitor(ArgumentVisitor):
        inputs = set()
    
        def visitInput(self, inputArg):
            self.inputs.add(inputArg)
            
        def export(self, doc):
            doc.enum("InputId", set([i.ident.constant() for i in self.inputs]))
            
    class OutputEnumVisitor(ArgumentVisitor):
        outputs = set()
    
        def visitOutput(self, output):
            self.outputs.add(output)
            
        def visitAllocation(self, allocation):
            self.outputs.add(allocation)
            
        def export(self, doc):
            if len(self.outputs) > 0:
                doc.enum("OutputId", ["RESULT"])
                
    class ParameterVisitor(ArgumentVisitor):
        params = set()
    
        def visitParameter(self, parameter):
            self.params.add(parameter)
            
        def visitNumericParameter(self, parameter):
            self.params.add(parameter)
            
    class ParameterEnumVisitor(ParameterVisitor):
        def export(self, doc, hasOptions):
            paramIds = [p.ident.constant() for p in self.params]
            if hasOptions:
                paramIds.append("DATA_FLOW")
                
            doc.enum("ParameterId", set(paramIds))
        
    class DataMemberVisitor(ParameterVisitor):
        def export(self, doc):
            for p in self.params:
                l = "{0} {1};".format(p.dataType.typeId(), p.ident.attribute())
                doc.line(l)
    
    def generate(self):
        self.__includeGuardEnter()
        self.__includes()
        self.__namespaceEnter()
        self.__classEnter()
        self.__public()
        
        v = OpHeaderGenerator.InputEnumVisitor()
        self.visitAll(v)
        v.export(self.doc)
        
        v = OpHeaderGenerator.OutputEnumVisitor()
        self.visitAll(v)
        v.export(self.doc)
        
        v = OpHeaderGenerator.ParameterEnumVisitor()
        self.visitAll(v)
        v.export(self.doc, len(self.m.options) > 1)
        
        self.__private()
        
        v = OpHeaderGenerator.DataMemberVisitor()
        self.visitAll(v)
        v.export(self.doc)
        
        self.__classExit()
        self.__namespaceExit()
        self.__includeGuardExit()
        
        
        with file("{0}.h".format(self.m.ident.className()), "w") as f:
            f.write(self.doc.string())
    
    def __includes(self):
        self.doc.line('#include "Config.h"')
        self.doc.line('#include <stromx/runtime/Enum.h>')
        self.doc.line('#include <stromx/runtime/OperatorKernel.h>')
        self.doc.line('#include <stromx/runtime/Primitive.h>')
        self.doc.blank()
        
    def __includeGuardEnter(self):
        self.doc.line("#ifndef {0}".format(self.__includeGuard()))
        self.doc.line("#define {0}".format(self.__includeGuard()))
        self.doc.blank()
        
    def __namespaceEnter(self):
        self.doc.namespaceEnter("stromx")
        self.doc.namespaceEnter(self.p.ident)
        
    def __classEnter(self):
        self.doc.line("class {0} {1} : public runtime::OperatorKernel".format(
                      self.__apiDecl(), self.m.ident.className()))
        self.doc.line("{")
        self.doc.increaseIndent()  
        
    def __public(self):
        self.doc.label("public")
        
    def __private(self):
        self.doc.label("private")
        
    def __classExit(self):
        self.doc.decreaseIndent()
        self.doc.line("};")
        
    def __namespaceExit(self):
        self.doc.namespaceExit()
        self.doc.namespaceExit()
        self.doc.blank()
        
    def __includeGuardExit(self):
        self.doc.line("#endif // {0}".format(self.__includeGuard()))
        
    def __includeGuard(self):
        return "STROMX_{0}_{1}_H".format(self.p.ident.upper(),
                                         self.m.ident.upper())
    
    def __apiDecl(self):
        return "STROMX_{0}_API".format(self.p.ident.upper())
            
class OpImplGenerator(MethodGenerator):
    
    def generate(self):
        
        with file("{0}.cpp".format(self.m.ident.className()), "w") as f:
            f.write(self.doc.string())
    
    def visitParameter(self, parameter):
        self.params.add(parameter.ident.attribute())
        
            
if __name__ == "__main__":
    import doctest
    doctest.testmod()     
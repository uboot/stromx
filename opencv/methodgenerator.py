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
    class ParameterVisitor(ArgumentVisitor):
        params = set()
    
        def visitParameter(self, parameter):
            self.params.add(parameter)
            
        def visitNumericParameter(self, parameter):
            self.params.add(parameter)
            
        def visitEnumParameter(self, parameter):
            self.params.add(parameter)
            
        def removeDuplicates(self):
            paramIdents = set()
            params = set()
            for p in self.params:
                if str(p.ident) not in paramIdents:
                    paramIdents.add(str(p.ident))
                    params.add(p)
            self.params = params
            
    p = None
    m = None
    doc = None
    
    def save(self, package, method, printResult):
        self.p = package
        self.m = method
        self.doc = document.Document()
        self.optionParam = self.createOptionParameter()
        
        self.generate()
        
        if printResult:
            print self.doc.string()
            
    def createOptionParameter(self):
        if len(self.m.options) <= 1:
            return None
            
        p = package.EnumParameter("dataFlow", "Data flow")
        p.isInit = True
        for opt in self.m.options:
            p.descriptions.append(package.EnumDescription(opt.ident,
                                                          str(opt.ident)))
        return p
            
    def visitAll(self, visitor):
        for opt in self.m.options:
            for arg in opt.args:
                arg.accept(visitor)
                
        if self.optionParam:
            self.optionParam.accept(visitor)
            
    def visitOption(self, visitor, opt):
        for arg in opt.args:
            arg.accept(visitor)
        
    def namespaceEnter(self):
        self.doc.namespaceEnter("stromx")
        self.doc.namespaceEnter(self.p.ident)
        
    def namespaceExit(self):
        self.doc.namespaceExit()
        self.doc.namespaceExit()
        self.doc.blank()
            
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
    #ifndef STROMX_IMGPROC_MEDIANBLUR_H
    #define STROMX_IMGPROC_MEDIANBLUR_H
    <BLANKLINE>
    #include "Config.h"
    #include <stromx/runtime/Enum.h>
    #include <stromx/runtime/OperatorKernel.h>
    #include <stromx/runtime/Primitive.h>
    <BLANKLINE>
    namespace stromx
    {
        namespace imgproc
        {
            class STROMX_IMGPROC_API MedianBlur : public runtime::OperatorKernel
            {
            public:
                enum InputId
                {
                    SRC
                }
                enum OutputId
                {
                    RESULT
                }
                enum ParameterId
                {
                    KSIZE,
                    DATA_FLOW
                }
                virtual OperatorKernel* clone() const { return new MedianBlur; }
                virtual void setParameter(const unsigned int id, const runtime::Data& value);
                virtual const runtime::DataRef getParameter(const unsigned int id) const;
                void initialize();
                virtual void execute(runtime::DataProvider& provider);
    <BLANKLINE>
            private:
                static const std::string PACKAGE;
                static const runtime::Version VERSION;
                static const std::string TYPE;
    <BLANKLINE>
                const std::vector<const runtime::Parameter*> setupInitParameters();
                const std::vector<const runtime::Parameter*> setupParameters();
                const std::vector<const runtime::Description*> setupInputs();
                const std::vector<const runtime::Description*> setupOutputs();
    <BLANKLINE>
                runtime::UInt32 m_ksize;
                runtime::Enum m_dataFlow;
            };
        }
    }
    <BLANKLINE>
    #endif // STROMX_IMGPROC_MEDIANBLUR_H
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
            
    class ParameterEnumVisitor(MethodGenerator.ParameterVisitor):
        def export(self, doc):
            paramIds = [p.ident.constant() for p in self.params]
            doc.enum("ParameterId", set(paramIds))
        
    class DataMemberVisitor(MethodGenerator.ParameterVisitor):
        def export(self, doc):
            for p in self.params:
                l = "{0} {1};".format(p.dataType.typeId(), p.ident.attribute())
                doc.line(l)
    
    def generate(self):
        self.__includeGuardEnter()
        self.__includes()
        self.namespaceEnter()
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
        v.export(self.doc)
        
        self.__kernelOverloads()
        
        self.__private()
        self.__statics()
        self.__setupFunctions()
        
        v = OpHeaderGenerator.DataMemberVisitor()
        self.visitAll(v)
        v.export(self.doc)
        
        self.__classExit()
        self.namespaceExit()
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
        
    def __classEnter(self):
        self.doc.line("class {0} {1} : public runtime::OperatorKernel".format(
                      self.__apiDecl(), self.m.ident.className()))
        self.doc.line("{")
        self.doc.increaseIndent()  
        
    def __public(self):
        self.doc.label("public")
        
    def __kernelOverloads(self):
        self.doc.line("virtual OperatorKernel* clone() const "
                      "{{ return new {0}; }}".format(self.m.ident.className()))
        self.doc.line("virtual void setParameter(const unsigned int id, "
                      "const runtime::Data& value);")
        self.doc.line("virtual const runtime::DataRef getParameter("
                      "const unsigned int id) const;")
        self.doc.line("void initialize();")
        self.doc.line("virtual void execute(runtime::DataProvider& provider);")
        self.doc.blank()
        
    def __private(self):
        self.doc.label("private")
        
    def __statics(self):
        self.doc.line("static const std::string PACKAGE;")
        self.doc.line("static const runtime::Version VERSION;")
        self.doc.line("static const std::string TYPE;")
        self.doc.blank()
        
    def __setupFunctions(self):
        self.doc.line("const std::vector<const runtime::Parameter*> "
                      "setupInitParameters();")
        self.doc.line("const std::vector<const runtime::Parameter*> "
                      "setupParameters();")
        self.doc.line("const std::vector<const runtime::Description*> "
                      "setupInputs();")
        self.doc.line("const std::vector<const runtime::Description*> "
                      "setupOutputs();")
        self.doc.blank()
        
    def __classExit(self):
        self.doc.decreaseIndent()
        self.doc.line("};")
        
    def __includeGuardExit(self):
        self.doc.line("#endif // {0}".format(self.__includeGuard()))
        
    def __includeGuard(self):
        return "STROMX_{0}_{1}_H".format(self.p.ident.upper(),
                                         self.m.ident.upper())
    
    def __apiDecl(self):
        return "STROMX_{0}_API".format(self.p.ident.upper())
            
class OpImplGenerator(MethodGenerator):
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
    >>> g = OpImplGenerator()
    >>> g.save(p, m, True) 
    """     
    class ParameterInitVisitor(MethodGenerator.ParameterVisitor):
        def export(self, doc):
            self.removeDuplicates()
            
            for i, p in enumerate(self.params):
                defaultValue = p.default if p.default != None else ""
                init = "{0}({1})".format(p.ident.attribute(), defaultValue)
                if i != len(self.params) - 1:
                    doc.line("{0},".format(init))
                else:
                    doc.line(init)
                    
    class GetParameterVisitor(MethodGenerator.ParameterVisitor):
        def export(self, doc):
            self.removeDuplicates()
            for p in self.params:
                doc.label("case {0}".format(p.ident.constant()))
                doc.line("return {0};".format(p.ident.attribute()))
        
    def generate(self):        
        self.__includes()
        self.namespaceEnter()
        self.__constructor()
        self.__getParameter()
        self.namespaceExit()
        
        with file("{0}.cpp".format(self.m.ident.className()), "w") as f:
            f.write(self.doc.string())
    
    def __includes(self):
        self.doc.line('#include "{0}.h"'.format(self.m.ident.className()))
        self.doc.blank()
        self.doc.line('#include <stromx/example/Image.h>')
        self.doc.line('#include <stromx/example/Matrix.h>')
        self.doc.line('#include <stromx/example/Utilities.h>')
        self.doc.line('#include <stromx/runtime/DataContainer.h>')
        self.doc.line('#include <stromx/runtime/DataProvider.h>')
        self.doc.line('#include <stromx/runtime/EnumParameter.h>')
        self.doc.line('#include <stromx/runtime/Id2DataComposite.h>')
        self.doc.line('#include <stromx/runtime/Id2DataPair.h>')
        self.doc.line('#include <stromx/runtime/NumericParameter.h>')
        self.doc.line('#include <stromx/runtime/OperatorException.h>')
        self.doc.line('#include <stromx/runtime/ReadAccess.h>')
        self.doc.line('#include <stromx/runtime/WriteAccess.h>')
        self.doc.line('#include <boost/assert.hpp>')
        self.doc.line('#include <opencv2/{0}/{0}.hpp>'.format(self.p.ident))
        self.doc.blank()    
        
    def __constructor(self):
        self.doc.line("{0}::{0}()".format(self.m.ident.className()))
        self.doc.line("  : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, "
                      "setupInitParameters()),")
        self.doc.increaseIndent()
        
        v = OpImplGenerator.ParameterInitVisitor()
        self.visitAll(v)
        v.export(self.doc)
        
        self.doc.decreaseIndent()
        self.doc.scopeEnter()
        self.doc.scopeExit()
        
    def __getParameter(self):
        self.doc.line("const runtime::DataRef {0}::getParameter"
                      "(unsigned int id) const"\
                      .format(self.m.ident.className()))
        self.doc.scopeEnter()
        self.doc.line("switch(id)")
        self.doc.scopeEnter()
        
        v = OpImplGenerator.GetParameterVisitor()
        self.visitAll(v)
        v.export(self.doc)
        
        self.doc.label("default")
        self.doc.line("throw runtime::WrongParameterId(id, *this);")
        self.doc.scopeExit()
        self.doc.scopeExit()
        
            
if __name__ == "__main__":
    import doctest
    doctest.testmod()     
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
        
    def string(self):
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
        self.line("")
        
    def className(self):
        return Names.className(self.m.ident)
        
    def namespaceEnter(self):
        self.line("namespace stromx")
        self.scopeEnter()
        self.line("namespace {0}".format(self.p.ident.lower()))
        self.scopeEnter()
    
    def namespaceExit(self):
        self.scopeExit()
        self.scopeExit()
        
    def scopeEnter(self):
        self.line("{")
        self.increaseIndent()
        
    def scopeExit(self):
        self.decreaseIndent()
        self.line("}")
        
    def label(self, key):
        self.decreaseIndent()
        self.line("{0}:".format(key))
        self.increaseIndent()
        
    def collect(self, method):
        l = []
        for arg in self.m.args:
            l.extend(arg.__getattribute__(method)())
        return l

class HeaderGenerator(MethodGenerator):
    def __init__(self, package, method):
        super(HeaderGenerator, self).__init__(package, method)
        
    def generate(self):
        self.includeGuardEnter()
        self.blank()
        self.includes()
        self.blank()
        self.namespaceEnter()
        self.classEnter()
        self.label("public:")
        self.inputIds()
        self.blank()
        self.outputIds()
        self.blank()
        self.parameterIds()
        self.blank()
        self.constructor()
        self.kernelOverloads()
        self.blank()
        self.label("private:")
        self.statics()
        self.blank()
        self.setupFunctions()
        self.blank()
        self.parameters()
        self.classExit()
        self.namespaceExit()
        self.blank()
        self.includeGuardExit()
        self.blank()
        
    def save(self):
        with file("{0}.h".format(self.className()), "w") as f:
            f.write(self.string())
    
    def includes(self):
        self.line('#include "Config.h"');
        self.line('#include <stromx/core/Enum.h>');
        self.line('#include <stromx/core/OperatorKernel.h>');
        self.line('#include <stromx/core/Primitive.h>');
        
    def includeGuardEnter(self):
        self.line("#ifndef {0}".format(self.includeGuard()))
        self.line("#define {0}".format(self.includeGuard()))
        
    def classEnter(self):
        self.line("class {0} {1} : public core::OperatorKernel".format(
                    self.apiDecl(), self.className()))
        self.line("{")
        self.increaseIndent()
        
    def inputIds(self):
        values = self.collect("inputId")
        self.enum("InputId", values)
        
    def outputIds(self):
        values = self.collect("outputId")
        self.enum("OutputId", values)
    
    def parameterIds(self):
        values = self.collect("paramId")
        self.enum("ParameterId", values)
        
    def constructor(self):
        self.line("{0}();".format(self.className()))
        
    def kernelOverloads(self):
        self.line("virtual OperatorKernel* clone() const {{ return new {0}; }}".format(self.className()))
        self.line("virtual void setParameter(const unsigned int id, const core::Data& value);")
        self.line("virtual const core::DataRef getParameter(const unsigned int id) const;")
        self.line("void initialize();")
        self.line("virtual void execute(core::DataProvider& provider);")
        
    def statics(self):
        self.line("static const std::string PACKAGE;")
        self.line("static const core::Version VERSION;")
        self.line("static const std::string TYPE;")
        
    def setupFunctions(self):
        self.line("const std::vector<const core::Parameter*> setupInitParameters();")
        self.line("const std::vector<const core::Parameter*> setupParameters();")
        self.line("const std::vector<const core::Description*> setupInputs();")
        self.line("const std::vector<const core::Description*> setupOutputs();")
        
    def parameters(self):
        values = self.collect("paramDecl")
        for v in values:
            self.line("{0};".format(v))
        
    def enum(self, name, values):
        self.line("enum {0}".format(name))
        self.scopeEnter()
        for isEnd, v in Names.listIterator(values):
            if not isEnd:
                self.line("{0},".format(v))
            else:
                self.line(v)
        self.scopeExit()
    
    def classExit(self):
        self.decreaseIndent()
        self.line("};")
        
    def includeGuardExit(self):
        self.line("#endif // {0}".format(self.includeGuard()))
        
    def includeGuard(self):
        return "STROMX_{0}_{1}_H".format(p.ident.upper(), m.ident.upper())
    
    def apiDecl(self):
        return "STROMX_{0}_API".format(p.ident.upper())
            
class ImplementationGenerator(MethodGenerator):
    def __init__(self, package, method):
        super(ImplementationGenerator, self).__init__(package, method)
        
    def generate(self):
        self.includes()
        self.blank()
        self.namespaceEnter()
        self.constants()
        self.blank()
        self.constructor()
        self.blank()
        self.setParameter()
        self.blank()
        self.getParameter()
        self.blank()
        self.setupInitParameters()
        self.blank()
        self.setupParameters()
        self.blank()
        self.setupInputs()
        self.blank()
        self.setupOutputs()
        self.blank()
        self.initialize()
        self.blank()
        self.execute()
        self.namespaceExit()
    
    def save(self):
        with file("{0}.cpp".format(self.className()), "w") as f:
            f.write(self.string())
            
    def includes(self):
        self.line('#include "{0}.h"'.format(self.className()))
        self.blank()
        self.line('#include "Image.h"')
        self.line('#include "Matrix.h"')
        self.line('#include "Utilities.h"')
        self.line('#include <stromx/core/DataContainer.h>')
        self.line('#include <stromx/core/DataProvider.h>')
        self.line('#include <stromx/core/EnumParameter.h>')
        self.line('#include <stromx/core/Id2DataPair.h>')
        self.line('#include <stromx/core/NumericParameter.h>')
        self.line('#include <stromx/core/OperatorException.h>')
        self.line('#include <stromx/core/ReadAccess.h>')
        self.line('#include <stromx/core/WriteAccess.h>')
        self.line('#include <opencv2/imgproc/imgproc.hpp>')
    
    def constructor(self):
        self.line("{0}()".format(self.method(self.className())))
        self.line(("  : core::OperatorKernel(TYPE, PACKAGE, VERSION, " +
                   "setupInitParameters()),"))
        self.increaseIndent()
        values = self.collect("paramInit")
        for isEnd, v in Names.listIterator(values):
            if not isEnd:
                self.line("{0},".format(v))
            else:
                self.line(v)
        self.decreaseIndent()
        self.scopeEnter()
        self.scopeExit()
        
    def constants(self):
        self.line("const std::string {0}::PACKAGE(STROMX_{1}_PACKAGE_NAME);"\
            .format(self.className(), Names.constantName(self.p.ident)))
        self.line("const core::Version {0}::VERSION({1}_VERSION_MAJOR, {1}_VERSION_MINOR, {1}_VERSION_PATCH);"\
            .format(self.className(), Names.constantName(self.p.ident)))
        
    def setParameter(self):
        self.line("void {0}(unsigned int id, const Data& value)"\
            .format(self.method("setParameter")))
        self.scopeEnter()
        self.line("try")
        self.scopeEnter()
        self.line("switch(id)")
        self.scopeEnter()
        self.label("default")
        self.line("throw core::WrongParameterId(id, *this);")
        self.scopeExit()
        self.scopeExit()
        self.line("catch(core::BadCast&)")
        self.scopeEnter()
        self.line("throw core::WrongParameterType(parameter(id), *this);")
        self.scopeExit()
        self.scopeExit()
        
    def getParameter(self):
        self.line("const DataRef {0}(unsigned int id)"\
            .format(self.method("getParameter")))
        self.scopeEnter()
        self.line("switch(id)")
        self.scopeEnter()
        self.label("default")
        self.line("throw core::WrongParameterId(id, *this);")
        self.scopeExit()
        self.scopeExit()
        
    def setupInitParameters(self):
        self.line("const std::vector<const core::Parameter*> {0}()"\
            .format(self.method("setupInitParameters")))
        self.scopeEnter()
        self.line("std::vector<const core::Parameter*> parameters;")
        self.blank()
        self.line("return parameters;")
        self.scopeExit()
    
    def setupParameters(self):
        self.line("const std::vector<const Parameter*> {0}()"\
            .format(self.method("setupParameters")))
        self.scopeEnter()
        self.line("std::vector<const Parameter*> parameters;")
        self.blank()
        self.line("return parameters;")
        self.scopeExit()
    
    def setupInputs(self):
        self.line("const std::vector<const core::Description*> {0}()"\
            .format(self.method("setupInputs")))
        self.scopeEnter()
        self.line("std::vector<const core::Description*> inputs;")
        self.blank()
        self.line("return inputs;")
        self.scopeExit()
    
    def setupOutputs(self):
        self.line("const std::vector<const core::Description*> {0}()"\
            .format(self.method("setupOutputs")))
        self.scopeEnter()
        self.line("std::vector<const core::Description*> outputs;")
        self.blank()
        self.line("return outputs;")
        self.scopeExit()
        
    def initialize(self):
        self.line("void {0}()"\
            .format(self.method("initialize")))
        self.scopeEnter()
        self.line(("core::OperatorKernel::initialize(setupInputs(), " +
                   "setupOutputs(), setupParameters());"))
        self.scopeExit()
    
    def execute(self):
        self.line("void {0}(core::DataProvider & provider)"\
            .format(self.method("execute")))
        self.scopeEnter()
        argStr = ""
        for isEnd, arg in Names.listIterator(self.m.args):
            argStr += arg.ident
            if not isEnd:
                argStr += ", "
        self.line("cv::{0}({1});".format(self.m.ident, argStr))
        self.scopeExit()
        
    def method(self, s):
        return "{0}::{1}".format(self.className(), s)
    
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
    
    g = HeaderGenerator(p, m)
    g.generate()
    print g.string()
    
    g = ImplementationGenerator(p, m)
    g.generate()
    print g.string()
# -*- coding: utf-8 -*-
"""
Created on Fri Dec 21 17:07:19 2012

@author: matz
"""

from package import *

def generate(package):
    g = LibHeaderGenerator(p)
    g.generate()
    g.save()
    
    g = LibImplGenerator(p)
    g.generate()
    g.save()
    
    g = CMakeGenerator(p)
    g.generate()
    g.save()
    
    g = ConfigGenerator(p)
    g.generate()
    g.save()
    
    g = LibHeaderGenerator(p)
    g.generate()
    g.save()
    
    g = LibImplGenerator(p)
    g.generate()
    g.save()
    
    for m in p.methods:
        g = OpHeaderGenerator(p, m)
        g.generate()
        g.save()
        
        g = OpImplGenerator(p, m)
        g.generate()
        g.save()

class Generator(object):
    def __init__(self):
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
        if isinstance(line, int):
            if line == Format.INCREASE_INDENT:
                self.increaseIndent()
            elif line == Format.DECREASE_INDENT:
                self.decreaseIndent()
            elif line == Format.SCOPE_ENTER:
                self.scopeEnter()
            elif line == Format.SCOPE_EXIT:
                self.scopeExit()
            else:
                assert(False)
            return
            
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
        
class LibGenerator(Generator):
    def __init__(self, package):
        super(LibGenerator, self).__init__()
        self.p = package
        
class LibHeaderGenerator(LibGenerator):
    def generate(self):
        p = Names.constantName(self.p.ident)
        self.line("#ifndef STROMX_{0}_{0}_H".format(p))
        self.line("#define STROMX_{0}_{0}_H".format(p))
        self.blank()
        
        self.line('#include "Config.h"')
        self.blank()
        
        self.line("namespace stromx")
        self.scopeEnter()
        self.line("namespace runtime".format(self.p.ident))
        self.scopeEnter()
        self.line("class Registry;")
        self.scopeExit()
        self.scopeExit()
        self.blank()
        
        self.line('extern "C"')
        self.scopeEnter()
        self.line("STROMX_{0}_API void stromxRegister{1}"
                  "(stromx::runtime::Registry& registry);"\
                  .format(p, Names.className(self.p.ident)))
        self.scopeExit()
        self.blank()
        
        self.line("#endif // STROMX_{0}_{0}_H".format(p))
        
    def save(self):
        with file("{0}.h".format(Names.className(self.p.ident)), "w") as f:
            f.write(self.string())
        
class LibImplGenerator(LibGenerator):
    def generate(self):
        self.line('#include "{0}.h"'.format(Names.className(self.p.ident)))
        self.blank()
        for m in self.p.methods:
            self.line('#include "{0}.h"'.format(Names.className(m.ident)))
        self.line("#include <stromx/runtime/Registry.h>")
        self.blank()
        
        self.line("void stromxRegister{0}(stromx::runtime::Registry& registry)"\
            .format(Names.className(self.p.ident)))
        self.scopeEnter()
        self.line("using namespace stromx::{0};".format(self.p.ident))
        self.blank()
        
        for m in self.p.methods:
            self.line("registry.registerOperator(new {0});"\
                .format(Names.className(m.ident)))
        
        self.scopeExit()
        
    def save(self):
        with file("{0}.cpp".format(Names.className(self.p.ident)), "w") as f:
            f.write(self.string())

class CMakeGenerator(LibGenerator):
    def generate(self):
        self.line("project(stromx_{0})".format(self.p.ident))
        self.blank()
        self.line("set ({0}_VERSION_MAJOR {1})"\
            .format(Names.constantName(self.p.ident), self.p.major))
        self.line("set ({0}_VERSION_MINOR {1})"\
            .format(Names.constantName(self.p.ident), self.p.major))
        self.line("set ({0}_VERSION_PATCH {1})"\
            .format(Names.constantName(self.p.ident), self.p.patch))
        self.blank()
        
        self.line("configure_file (")
        self.increaseIndent()
        self.line(r"${PROJECT_SOURCE_DIR}/Config.h.in")
        self.line(r"${PROJECT_SOURCE_DIR}/Config.h")
        self.decreaseIndent()
        self.line(")")
        self.blank()
        
        self.line("include_directories (")
        self.increaseIndent()
        self.line(r"../..")
        self.line(r"${Boost_INCLUDE_DIRS}")
        self.decreaseIndent()
        self.line(")")
        self.blank()
        
        self.line("set (SOURCES ")
        self.increaseIndent()
        for m in self.p.methods:
            self.line("{0}.cpp".format(Names.className(m.ident)))
            self.line("{0}.cpp".format(Names.className(self.p.ident)))
        self.decreaseIndent()
        self.line(")")
        self.blank()
        
        self.line("add_library (stromx_{0} SHARED ${{SOURCES}})"\
            .format(self.p.ident))
        self.blank()
        
        self.line('set(VERSION_STRING "${{{0}_VERSION_MAJOR}}.'
                  '${{{0}_VERSION_MINOR}}.${{{0}_VERSION_PATCH}}")'\
                  .format(Names.constantName(self.p.ident)))
        self.blank()
        
        self.line("set_target_properties (stromx_{0} PROPERTIES"\
            .format(self.p.ident))
        self.increaseIndent()
        self.line("VERSION ${VERSION_STRING}")
        self.line("SOVERSION ${VERSION_STRING}")
        self.decreaseIndent()
        self.line(")")
        self.blank()
        
        self.line("target_link_libraries (stromx_{0}"\
            .format(self.p.ident))
        self.increaseIndent()
        self.line("${OpenCV_LIBS}")
        self.line("stromx_runtime")
        self.line("stromx_example")
        self.decreaseIndent()
        self.line(")")
        self.blank()
    
    def save(self):
        with file("CMakeLists.txt", "w") as f:
            f.write(self.string())
            
class ConfigGenerator(LibGenerator):
    def generate(self):
        p = Names.constantName(self.p.ident)
        guard = "STROMX_{0}_CONFIG_H".format(p)
        
        self.line("#ifndef {0}".format(guard))
        self.line("#define {0}".format(guard))
        self.blank()
        
        version = "{0}_VERSION".format(p)
        self.line("#define {0}_MAJOR @{0}_MAJOR@".format(version))
        self.line("#define {0}_MINOR @{0}_MINOR@".format(version))
        self.line("#define {0}_PATCH @{0}_PATCH@".format(version))
        self.blank()
        
        self.line('#define STROMX_{0}_PACKAGE_NAME "{1}"'\
            .format(p, Names.className(self.p.ident)))
        self.blank()
        
        self.line("#ifdef WIN32")
        self.increaseIndent()
        self.line("#ifdef STROMX_{0}_STATIC".format(p))
        self.increaseIndent()
        self.line("#define STROMX_{0}_STATIC".format(p))
        self.decreaseIndent()
        self.line("#else // STROMX_{0}_STATIC".format(p))
        self.increaseIndent()
        self.line("#ifdef stromx_{0}_EXPORTS".format(self.p.ident))
        self.increaseIndent()
        self.line("#define STROMX_{0}_API __declspec(dllexport)".format(p))
        self.decreaseIndent()
        self.line("#else // stromx_{0}_EXPORTS".format(self.p.ident))
        self.increaseIndent()
        self.line("#define STROMX_{0}_API __declspec(dllimport)".format(p))
        self.decreaseIndent()
        self.line("#endif // stromx_{0}_EXPORTS".format(self.p.ident))
        self.decreaseIndent()
        self.line("#endif // STROMX_{0}_STATIC".format(p))
        self.decreaseIndent()
        self.line("#else // WIN32")
        self.increaseIndent()
        self.line("#define STROMX_{0}_API".format(p))
        self.decreaseIndent()
        self.line("#endif // WIN32")
        self.blank() 
        
        self.line("#endif // {0}".format(guard))

    def save(self):
        with file("Config.h.in", "w") as f:
            f.write(self.string())
        
class MethodGenerator(Generator):
    option = Options.MANUAL
    
    def __init__(self, package, method):
        super(MethodGenerator, self).__init__()
        self.p = package
        self.m = method
        
    def collect(self, method):
        l = []
        for arg in self.m.options[self.option]:
            l.extend(arg.__getattribute__(method)())
        l.extend(self.m.optionParameter.__getattribute__(method)())
        return l
        
class OpHeaderGenerator(MethodGenerator):
    def __init__(self, package, method):
        super(OpHeaderGenerator, self).__init__(package, method)
        
    def generate(self):
        self.includeGuardEnter()
        self.blank()
        self.includes()
        self.blank()
        self.namespaceEnter()
        self.classEnter()
        self.label("public")
        self.inputIds()
        self.blank()
        self.outputIds()
        self.blank()
        self.parameterIds()
        self.blank()
        self.enumIds()
        self.blank()
        self.constructor()
        self.kernelOverloads()
        self.blank()
        self.label("private")
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
        self.line('#include <stromx/runtime/Enum.h>');
        self.line('#include <stromx/runtime/OperatorKernel.h>');
        self.line('#include <stromx/runtime/Primitive.h>');
        
    def includeGuardEnter(self):
        self.line("#ifndef {0}".format(self.includeGuard()))
        self.line("#define {0}".format(self.includeGuard()))
        
    def classEnter(self):
        self.line("class {0} {1} : public runtime::OperatorKernel".format(
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
    
    def enumIds(self):
        values = self.collect("enumIds")
        for v in values:
            self.enum(v[0], v[1])
        
    def constructor(self):
        self.line("{0}();".format(self.className()))
        
    def kernelOverloads(self):
        self.line("virtual OperatorKernel* clone() const {{ return new {0}; }}".format(self.className()))
        self.line("virtual void setParameter(const unsigned int id, const runtime::Data& value);")
        self.line("virtual const runtime::DataRef getParameter(const unsigned int id) const;")
        self.line("void initialize();")
        self.line("virtual void execute(runtime::DataProvider& provider);")
        
    def statics(self):
        self.line("static const std::string PACKAGE;")
        self.line("static const runtime::Version VERSION;")
        self.line("static const std::string TYPE;")
        
    def setupFunctions(self):
        self.line("const std::vector<const runtime::Parameter*> setupInitParameters();")
        self.line("const std::vector<const runtime::Parameter*> setupParameters();")
        self.line("const std::vector<const runtime::Description*> setupInputs();")
        self.line("const std::vector<const runtime::Description*> setupOutputs();")
        
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
        self.decreaseIndent()
        self.line("};")
    
    def classExit(self):
        self.decreaseIndent()
        self.line("};")
        
    def includeGuardExit(self):
        self.line("#endif // {0}".format(self.includeGuard()))
        
    def includeGuard(self):
        return "STROMX_{0}_{1}_H".format(self.p.ident.upper(),
                                         self.m.ident.upper())
    
    def apiDecl(self):
        return "STROMX_{0}_API".format(self.p.ident.upper())
            
class OpImplGenerator(MethodGenerator):
    def __init__(self, package, method):
        super(OpImplGenerator, self).__init__(package, method)
        
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
        self.line('#include <stromx/example/Image.h>')
        self.line('#include <stromx/example/Matrix.h>')
        self.line('#include <stromx/example/Utilities.h>')
        self.line('#include <stromx/runtime/DataContainer.h>')
        self.line('#include <stromx/runtime/DataProvider.h>')
        self.line('#include <stromx/runtime/EnumParameter.h>')
        self.line('#include <stromx/runtime/Id2DataComposite.h>')
        self.line('#include <stromx/runtime/Id2DataPair.h>')
        self.line('#include <stromx/runtime/NumericParameter.h>')
        self.line('#include <stromx/runtime/OperatorException.h>')
        self.line('#include <stromx/runtime/ReadAccess.h>')
        self.line('#include <stromx/runtime/WriteAccess.h>')
        self.line('#include <boost/assert.hpp>')
        self.line('#include <opencv2/imgproc/imgproc.hpp>')
    
    def constructor(self):
        self.line("{0}()".format(self.method(self.className())))
        self.line(("  : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, " +
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
        self.line("const runtime::Version {0}::VERSION({1}_VERSION_MAJOR, {1}_VERSION_MINOR, {1}_VERSION_PATCH);"\
            .format(self.className(), Names.constantName(self.p.ident)))
        self.line('const std::string {0}::TYPE("{1}");'\
            .format(self.className(), Names.className(self.m.ident)))
        
    def setParameter(self):
        self.line("void {0}(unsigned int id, const runtime::Data& value)"\
            .format(self.method("setParameter")))
        self.scopeEnter()
        self.line("try")
        self.scopeEnter()
        self.line("switch(id)")
        self.scopeEnter()
        values = self.collect("paramSet")
        for v in values:
            self.label("case {0}".format(v.label))
            for l in v.lines:
                self.line(l)
        self.label("default")
        self.line("throw runtime::WrongParameterId(id, *this);")
        self.scopeExit()
        self.scopeExit()
        self.line("catch(runtime::BadCast&)")
        self.scopeEnter()
        self.line("throw runtime::WrongParameterType(parameter(id), *this);")
        self.scopeExit()
        self.scopeExit()
        
    def getParameter(self):
        self.line("const runtime::DataRef {0}(unsigned int id) const"\
            .format(self.method("getParameter")))
        self.scopeEnter()
        self.line("switch(id)")
        self.scopeEnter()
        values = self.collect("paramGet")
        for v in values:
            self.label("case {0}".format(v.label))
            for l in v.lines:
                self.line(l)
        self.label("default")
        self.line("throw runtime::WrongParameterId(id, *this);")
        self.scopeExit()
        self.scopeExit()
        
    def setupInitParameters(self):
        self.line("const std::vector<const runtime::Parameter*> {0}()"\
            .format(self.method("setupInitParameters")))
        self.scopeEnter()
        self.line("std::vector<const runtime::Parameter*> parameters;")
        self.blank()
        values = self.collect("initParamCreate")
        for v in values:
            for l in v:
                self.line(l)
            self.blank()
        self.line("return parameters;")
        self.scopeExit()
    
    def setupParameters(self):
        self.line("const std::vector<const runtime::Parameter*> {0}()"\
            .format(self.method("setupParameters")))
        self.scopeEnter()
        self.line("std::vector<const runtime::Parameter*> parameters;")
        self.blank()
        values = self.collect("paramCreate")
        for v in values:
            for l in v:
                self.line(l)
            self.blank()
        self.line("return parameters;")
        self.scopeExit()
    
    def setupInputs(self):
        self.line("const std::vector<const runtime::Description*> {0}()"\
            .format(self.method("setupInputs")))
        self.scopeEnter()
        
        optParam = self.m.optionParameter
        if not optParam.trivial:
            self.line("switch(int({0}))"\
                .format(Names.attributeName(optParam.ident)))
            self.scopeEnter()
            
        for opt in optParam.options:   
            self.option = opt
            if not optParam.trivial:
                self.label("case {0}".format(optParam.enumConstant(opt)))
                self.scopeEnter()
                
            self.__setupInputs()
                
            if not optParam.trivial:
                self.scopeExit()
            
        self.option = Options.MANUAL
            
        if not optParam.trivial:
            self.label("default")
            self.line("BOOST_ASSERT(false);")
            self.scopeExit()
            
        self.scopeExit()
        
    def __setupInputs(self):
        self.line("std::vector<const runtime::Description*> inputs;")
        self.blank()
        values = self.collect("inputCreate")
        for v in values:
            for l in v:
                self.line(l)
            self.blank()
        self.line("return inputs;")
        
    
    def setupOutputs(self):
        self.line("const std::vector<const runtime::Description*> {0}()"\
            .format(self.method("setupOutputs")))
        self.scopeEnter()
                
        self.line("std::vector<const runtime::Description*> outputs;")
        self.blank()
        values = self.collect("outputCreate")
        for v in values:
            for l in v:
                self.line(l)
            self.blank()
        self.line("return outputs;") 
        
        self.scopeExit()
        
    def initialize(self):
        self.line("void {0}()"\
            .format(self.method("initialize")))
        self.scopeEnter()
        self.line(("runtime::OperatorKernel::initialize(setupInputs(), " +
                   "setupOutputs(), setupParameters());"))
        self.scopeExit()
    
    def execute(self):
        self.line("void {0}(runtime::DataProvider & provider)"\
            .format(self.method("execute")))
        self.scopeEnter()
        
        optParam = self.m.optionParameter
        if not optParam.trivial:
            self.line("switch(int({0}))"\
                .format(Names.attributeName(optParam.ident)))
            self.scopeEnter()
            
        for opt in optParam.options:   
            self.option = opt
            if not optParam.trivial:
                self.label("case {0}".format(optParam.enumConstant(opt)))
                self.scopeEnter()
                
            self.__execute()
                
            if not optParam.trivial:
                self.scopeExit()
                self.line("break;")
            
        self.option = Options.MANUAL
            
        if not optParam.trivial:
            self.label("default")
            self.line("BOOST_ASSERT(false);")
            self.scopeExit()
        
        self.scopeExit()
        
    def __execute(self):
        writeAccess = self.collect("writeAccess")
        assert(len(writeAccess) <= 1)
        readAccess = self.collect("readAccess")
        access = readAccess + writeAccess
        
        for a in access:
            self.line("runtime::Id2DataPair {0}InMapper({1});"\
                .format(a, Names.constantName(a)))
        self.blank()
          
        receiveInputStr = ""              
        for isEnd, a in Names.listIterator(access):
            receiveInputStr += "{0}InMapper".format(a)
            
            if not isEnd:
                receiveInputStr += " && "
        self.line("provider.receiveInputData({0});".format(receiveInputStr))
        self.blank()
        
        for a in writeAccess:
            self.line("runtime::Data* {0}Data = 0;".format(a))
            
        for a in readAccess:
            self.line("const runtime::Data* {0}Data = 0;".format(a))
        if len(readAccess):
            self.blank()
        
        for a in readAccess:
            self.line("runtime::ReadAccess<> {0}ReadAccess;"\
                .format(a))
        self.blank()
        
        if len(writeAccess):
            self.line("runtime::DataContainer inContainer = {0}InMapper.data();"\
                .format(writeAccess[0]))
            self.line("runtime::WriteAccess<> writeAccess(inContainer);")
            self.line("{0}Data = &writeAccess();".format(writeAccess[0]))
            self.blank()
            
            for a in readAccess:
                self.line("if({0}InMapper.data() == inContainer)".format(a))
                self.scopeEnter()
                self.line("{0}Data = &writeAccess();".format(a))
                self.scopeExit()
                self.line("else")
                self.scopeEnter()
                self.line(("{0}ReadAccess = runtime::ReadAccess<>({0}InMapper" +
                           ".data());").format(a))
                self.line("{0}Data = &{0}ReadAccess();".format(a))
                self.scopeExit()
                self.blank()
        else:
            for a in readAccess:
                self.line(("{0}ReadAccess = runtime::ReadAccess<>({0}InMapper" +
                           ".data());").format(a))
                self.line("{0}Data = &{0}ReadAccess();".format(a))
                self.blank();
                
        castedData = self.collect("castedData")
        for v in castedData:
            self.line(v)
        self.blank()
        
        outputInit = self.collect("outputInit")
        for v in outputInit:
            self.line(v)
        if len(outputInit):
            self.blank()
        
        cvData = self.collect("cvData")
        for v in cvData:
            self.line(v)
        self.blank()
        
        allocate = self.collect("allocate")
        for v in allocate:
            self.line(v)
        if len(allocate):
            self.blank()
        
        arg = self.collect("arg")
        argStr = ""
        for isEnd, a in Names.listIterator(arg):
            argStr += a
            if not isEnd:
                argStr += ", "
        self.line("cv::{0}({1});".format(self.m.ident, argStr))
        self.blank()
        
        outContainer = self.collect("outContainer")
        for v in outContainer:
            self.line(v)
        if len(outContainer):
            self.blank()
        
        outputId = self.collect("outputId")
        assert(len(outputId) == 1)
        self.line("runtime::Id2DataPair outputMapper({0}, outContainer);"\
            .format(outputId[0]))
        
        self.line("provider.sendOutputData(outputMapper);")
        
    def method(self, s):
        return "{0}::{1}".format(self.className(), s)
    
if __name__ == "__main__":
    p = Package(0, 0, 1)
    p.ident = "imgproc"
    p.name = "OpenCV image processing"
    
    m = Method()
    m.ident = "medianBlur"
    m.name = "Median Blur"
    p.methods.append(m)
    
    arg1 = Argument()
    arg1.ident = "src"
    arg1.name = "Source"
    arg1.cvType = CvType.MAT
    arg1.dataType = DataType.IMAGE
    
    arg2 = Argument()
    arg2.ident = "dst"
    arg2.name = "Destination"
    arg2.cvType = CvType.MAT
    arg2.dataType = DataType.IMAGE
    
    arg3 = NumericParameter()
    arg3.ident = "ksize"
    arg3.name = "Kernel size"
    arg3.cvType = CvType.INT
    arg3.dataType = DataType.UINT_32
    arg3.default = 3
    arg3.minValue = 1
    arg3.rules.append(OddRule())
    
    options = dict()
    options[Options.MANUAL] = [Input(arg1), Output(arg2, arg1), arg3]
    options[Options.IN_PLACE] = [Output(arg1), RefInput(arg1), arg3]
    options[Options.ALLOCATE] = [Input(arg1), Allocation(arg2, arg1), arg3]

    m.options = options
    
    generate(p)

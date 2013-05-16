# -*- coding: utf-8 -*-

import datatype
import document
import package
import cvtype

from rulegenerator import *

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
        
    def visitCompound(self, compound):
        pass
    
class SingleArgumentVisitor(ArgumentVisitor):
    def visitCompound(self, compound):
        for arg in compound.args:
            arg.accept(self)
            
    
class CollectVisitor(SingleArgumentVisitor):
    def __init__(self):
        self.args = set()
        
    def visitInput(self, inputArg):
        self.args.add(inputArg)
        
    def visitParameter(self, parameter):
        self.args.add(parameter)
        
    def visitNumericParameter(self, numericParameter):
        self.args.add(numericParameter)
        
    def visitConstant(self, const):
        self.args.add(const)
        
    def visitEnumParameter(self, enumParameter):
        self.args.add(enumParameter)
        
    def visitRefInput(self, refInput):
        self.args.add(refInput)
        
    def visitAllocation(self, allocation):
        self.args.add(allocation)
        
    def visitOutput(self, output):
        self.args.add(output)
        
class MethodGenerator(object):
    
    class CollectParametersVisitor(SingleArgumentVisitor):
        def __init__(self):
            self.params = []
    
        def visitParameter(self, parameter):
            self.params.append(parameter)
            
        def visitNumericParameter(self, parameter):
            self.visitParameter(parameter)
            
        def visitEnumParameter(self, parameter):
            self.visitParameter(parameter)
            
    class DocVisitor(SingleArgumentVisitor):
        def __init__(self, doc):
            self.doc = doc
            
    class ParameterVisitor(DocVisitor):            
        def visitNumericParameter(self, parameter):
            self.visitParameter(parameter)
            
        def visitEnumParameter(self, parameter):
            self.visitParameter(parameter)
            
    def __init__(self):
        self.p = None
        self.m = None
        self.doc = None
    
    def save(self, package, method, printResult = False):
        self.p = package
        self.m = method
        self.doc = document.Document()
        self.optionParam = self.createOptionParameter()
        
        self.generate()
        
        if printResult:
            print self.doc.string()
            
    def createOptionParameter(self):
        p = package.EnumParameter("dataFlow", "Data flow")
        p.isInit = True
        for opt in self.m.options:
            desc = package.EnumDescription(opt.ident.constant(), str(opt.name))
            desc.name = opt.name
            p.descriptions.append(desc)
        return p
            
    def visitAll(self, visitor, visitOptionParam = True):
        v = CollectVisitor()
        for opt in self.m.options:
            for arg in opt.args:
                arg.accept(v)
                
        args = v.args
        argIdents = set()
        filteredArgs = set()
        for arg in args:
            if str(arg.ident) not in argIdents:
                argIdents.add(str(arg.ident))
                filteredArgs.add(arg)
            
        for arg in filteredArgs:
            arg.accept(visitor)
                
        if visitOptionParam and self.optionParam:
            self.optionParam.accept(visitor)
            
    def visitOption(self, opt, visitor):
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
    class ConnectorEnumVisitor(SingleArgumentVisitor):
        def __init__(self):
            self.connectors = set()
            
        def visitRefInput(self, refInputArg):
            self.connectors.add(refInputArg)
    
        def visitInput(self, inputArg):
            self.connectors.add(inputArg)
    
        def visitOutput(self, output):
            self.connectors.add(output)
            
        def visitAllocation(self, allocation):
            self.connectors.add(allocation)
            
        def export(self, doc):
            connectorIds = [i.ident.constant() for i in self.connectors]
            doc.enum("ConnectorId", set(connectorIds))
            
    class ParameterEnumVisitor(MethodGenerator.CollectParametersVisitor):
        def export(self, doc):
            paramIds = [p.ident.constant() for p in self.params]
            doc.enum("ParameterId", set(paramIds))
        
    class DataMemberVisitor(MethodGenerator.ParameterVisitor):
        def visitParameter(self, parameter):
            l = "{0} {1};".format(parameter.dataType.typeId(),
                                  parameter.ident.attribute())
            self.doc.line(l)
        
    class ParameterDescriptionsVisitor(MethodGenerator.DocVisitor):
        def visitEnumParameter(self, parameter):
            self.doc.line(("runtime::EnumParameter* m_{0}Parameter;"
                          ).format(parameter.ident))
                          
        def visitNumericParameter(self, parameter):
            self.doc.line(("runtime::NumericParameter<{1}>* m_{0}Parameter;"
                          ).format(parameter.ident,
                                   parameter.dataType.typeId()))
            
    class EnumParameterIdVisitor(MethodGenerator.DocVisitor):
        def visitEnumParameter(self, parameter):
            keys = []
            for desc in parameter.descriptions:
                keys.append(desc.ident)
            enumName = "{0}Id".format(parameter.ident.className())
            self.doc.enum(enumName, keys)
            
    class EnumConversionDeclVisitor(MethodGenerator.DocVisitor):
        def visitEnumParameter(self, parameter):
            name = parameter.ident.className()
            l = "int convert{0}(const runtime::Enum & value);".format(name)
            self.doc.line(l)
    
    def generate(self):
        self.__includeGuardEnter()
        self.__includes()
        self.namespaceEnter()
        self.__classEnter()
        self.__public()
        
        v = OpHeaderGenerator.EnumParameterIdVisitor(self.doc)
        self.visitAll(v)
        
        v = OpHeaderGenerator.ConnectorEnumVisitor()
        self.visitAll(v)
        v.export(self.doc)
        
        v = OpHeaderGenerator.ParameterEnumVisitor()
        self.visitAll(v)
        v.export(self.doc)
        
        self.__constructor()
        self.__kernelOverloads()
        
        self.__private()
        self.__statics()
        self.__setupFunctions()
        
        v = OpHeaderGenerator.EnumConversionDeclVisitor(self.doc)
        self.visitAll(v, False)
        self.doc.blank()
        
        v = OpHeaderGenerator.DataMemberVisitor(self.doc)
        self.visitAll(v)
        
        v = OpHeaderGenerator.ParameterDescriptionsVisitor(self.doc)
        self.visitAll(v)
        
        self.__classExit()
        self.namespaceExit()
        self.__includeGuardExit()
        
        with file("{0}.h".format(self.m.ident.className()), "w") as f:
            f.write(self.doc.string())
    
    def __includes(self):
        self.doc.line('#include "stromx/{0}/Config.h"'.format(self.p.ident))
        self.doc.line('#include <stromx/runtime/Enum.h>')
        self.doc.line('#include <stromx/runtime/EnumParameter.h>')
        self.doc.line('#include <stromx/runtime/NumericParameter.h>')
        self.doc.line('#include <stromx/runtime/OperatorException.h>')
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
        
    def __constructor(self):
        self.doc.line("{0}();".format(self.m.ident.className()))
        
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
    class ParameterInitVisitor(MethodGenerator.CollectParametersVisitor):
        def export(self, doc):
            for i, p in enumerate(self.params):
                defaultValue = p.default if p.default != None else ""
                init = "{0}({1})".format(p.ident.attribute(), defaultValue)
                if i != len(self.params) - 1:
                    doc.line("{0},".format(init))
                else:
                    doc.line(init)
            
    class GetParametersVisitor(MethodGenerator.ParameterVisitor):
        def visitParameter(self, parameter):
            self.doc.label("case {0}".format(parameter.ident.constant()))
            self.doc.line("return {0};".format(parameter.ident.attribute()))
                    
    class SetParametersVisitor(MethodGenerator.DocVisitor):
        def visitParameter(self, parameter):
            self.__setParameterWithCheck(parameter)
            
        def visitEnumParameter(self, parameter):
            l = ("checkEnumValue(castedValue, {0}Parameter, *this);"
                ).format(parameter.ident.attribute())
            self.__setParameterWithCheck(parameter, l)
            
        def visitNumericParameter(self, parameter):
            l = ("checkNumericValue(castedValue, {0}Parameter, *this);"
                ).format(parameter.ident.attribute())
            self.__setParameterWithCheck(parameter, l)
            
        def __setParameterWithCheck(self, parameter, check = ""):
            self.doc.label("case {0}".format(parameter.ident.constant()))
            self.doc.scopeEnter()
            self.doc.line(("{0} castedValue = runtime::data_cast<{1}>(value);"
                          ).format(parameter.dataType.typeId(),
                                   parameter.dataType.typeId()))
            if check != "":
                self.doc.line(check)
            
            checkParams = CheckParameterVisitor(self.doc, parameter)
            for rule in parameter.rules:
                rule.accept(checkParams)
                
            self.doc.line(("{0} = castedValue;"
                          ).format(parameter.ident.attribute()))
            self.doc.scopeExit()
            self.doc.line("break;")
                
    class SetupParametersVisitor(MethodGenerator.DocVisitor):
        def __init__(self, doc, isInit = False):
            super(OpImplGenerator.SetupParametersVisitor, self).__init__(doc)
            self.isInit = isInit
            
        def visitParameter(self, parameter):
            l = "runtime::Parameter* {0} = new runtime::Parameter({1}, {2});"\
                .format(parameter.ident, parameter.ident.constant(),
                        parameter.dataType.variant())
            self.doc.line(l)
            self.__accessMode(parameter.ident)
            l = '{0}->setTitle("{1}");'\
                .format(parameter.ident, parameter.name)
            self.doc.line(l)
            l = "parameters.push_back({0});".format(parameter.ident)
            self.doc.line(l)
            self.doc.blank()
            
        def visitEnumParameter(self, parameter):
            ident = "m_{0}Parameter".format(parameter.ident)
            l = ("{0} = new runtime::EnumParameter({1});"
                ).format(ident, parameter.ident.constant())
            self.doc.line(l)
            self.__accessMode(ident)
            l = '{0}->setTitle("{1}");'.format(ident, parameter.name)
            self.doc.line(l)

            for desc in parameter.descriptions:
                d = 'runtime::Enum({0})'.format(desc.ident)
                l = '{0}->add(runtime::EnumDescription({1}, "{2}"));'\
                    .format(ident, d, desc.name)
                self.doc.line(l)
            l = "parameters.push_back({0});".format(ident)
            self.doc.line(l)
            self.doc.blank()
            
        def visitNumericParameter(self, parameter):
            ident = "m_{0}Parameter".format(parameter.ident)
            l = ("{0} = new runtime::NumericParameter<{2}>({1});"
                ).format(ident, parameter.ident.constant(),
                         parameter.dataType.typeId())
            self.doc.line(l)
            self.__accessMode(ident)
            l = '{0}->setTitle("{1}");'\
                .format(ident, parameter.name)
            self.doc.line(l)
            if parameter.maxValue != None:
                l = "{0}->setMax({1});".format(ident,
                                 parameter.dataType.cast(parameter.maxValue))
                self.doc.line(l)
            if parameter.minValue != None:
                l = "{0}->setMin({1});".format(ident,
                                 parameter.dataType.cast(parameter.minValue))
                self.doc.line(l)
            if parameter.step != None:
                l = "{0}->setStep({1});".format(ident,
                                 parameter.dataType.cast(parameter.step))
                self.doc.line(l)
            l = "parameters.push_back({0});".format(ident)
            self.doc.line(l)
            self.doc.blank()
            
        def __accessMode(self, ident):
            if self.isInit:
                accessMode = "NONE_WRITE"
            else:
                accessMode = "ACTIVATED_WRITE"
                
            l = "{0}->setAccessMode(runtime::Parameter::{1});"\
                .format(ident, accessMode)
            self.doc.line(l)
            
            
    class SetupOutputsVistor(MethodGenerator.DocVisitor):
        def visitOutput(self, output):
            l = "runtime::Description* {0} = new runtime::Description({1}, {2});"\
                .format(output.ident, output.ident.constant(),
                        output.dataType.variant())
            self.doc.line(l)
            l = '{0}->setTitle("{1}");'\
                .format(output.ident, output.name)
            self.doc.line(l)
            l = "outputs.push_back({0});".format(output.ident)
            self.doc.line(l)
            self.doc.blank()
        
        def visitAllocation(self, allocation):
            self.visitOutput(allocation)
            
    class SetupInputsVisitor(MethodGenerator.DocVisitor):
        def visitOutput(self, output):
            l = "runtime::Description* {0} = new runtime::Description({1}, {2});"\
                .format(output.ident, output.ident.constant(),
                        output.dataType.variant())
            self.doc.line(l)
            l = '{0}->setTitle("{1}");'\
                .format(output.ident, output.name)
            self.doc.line(l)
            l = "inputs.push_back({0});".format(output.ident)
            self.doc.line(l)
            self.doc.blank()
        
        def visitInput(self, allocation):
            self.visitOutput(allocation)
            
    class InputMapperVisitor(MethodGenerator.DocVisitor):
        def visitInput(self, inputArg):
            self.__visit(inputArg)
            
        def visitOutput(self, output):
            self.__visit(output)
            
        def __visit(self, arg):
            ident = arg.ident
            constant = arg.ident.constant()
            l = "runtime::Id2DataPair {0}InMapper({1});".format(ident, constant)
            self.doc.line(l)
    
    class ReceiveInputDataVisitor(SingleArgumentVisitor):
        def __init__(self):
            self.line = ""
            
        def visitInput(self, inputArg):
            self.__visit(inputArg)
            
        def visitOutput(self, output):
            self.__visit(output)
            
        def export(self, doc):
            if self.line != "":
                doc.line("provider.receiveInputData({0});".format(self.line))
            
        def __visit(self, arg):
            if self.line == "":
                self.line = "{0}InMapper".format(arg.ident)
            else:
                self.line += " && {0}InMapper".format(arg.ident)
           
    class InDataVisitor(MethodGenerator.DocVisitor):
        def visitInput(self, inputArg):
            self.doc.line(("const runtime::Data* "
                           "{0}Data = 0;").format(inputArg.ident))
            
        def visitOutput(self, output):
            self.doc.line("runtime::Data* {0}Data = 0;".format(output.ident))
            
    class AccessVisitor(MethodGenerator.DocVisitor):
        def visitInput(self, inputArg):
            self.doc.line(("runtime::ReadAccess<> "
                           "{0}ReadAccess;").format(inputArg.ident))
                        
        def visitOutput(self, output):
            mapper = "{0}InMapper".format(output.ident)
            data = "{0}Data".format(output.ident)
            self.doc.line(("runtime::DataContainer inContainer = "
                           "{0}.data();").format(mapper))
            self.doc.line("runtime::WriteAccess<> writeAccess(inContainer);")
            self.doc.line("{0} = &writeAccess();".format(data))
            
    class CopyWriteAccessVisitor(SingleArgumentVisitor):
        def __init__(self):
            self.output = None
            self.inputs = []
            
        def visitInput(self, inputArg):
            self.inputs.append(inputArg)
            
        def visitOutput(self, output):
            assert(self.output == None)
            self.output = output
            
        def export(self, doc):
            # no danger of reading a write access if there is no output (i.e.
            # no write access)
            if self.output == None:
                for i in self.inputs:
                    l = ("{0}ReadAccess = runtime::ReadAccess<>("
                         "{0}InMapper.data());").format(i.ident)
                    doc.line(l)
                    l = "{0}Data = &{0}ReadAccess();".format(i.ident)
                    doc.line(l)
                doc.blank()
                return
                
            # check if a read access refers to the same data as the write
            # acess and handle this situation accordingly
            for i in self.inputs:
                l = "if({0}InMapper.data() == inContainer)".format(i.ident)
                doc.line(l)
                doc.scopeEnter()
                doc.line("srcData = &writeAccess();")
                doc.scopeExit()
                doc.line("else")
                doc.scopeEnter()
                l = ("{0}ReadAccess = runtime::ReadAccess<>("
                     "{0}InMapper.data());").format(i.ident)
                doc.line(l)
                l = "{0}Data = &{0}ReadAccess();".format(i.ident)
                doc.line(l)
                doc.scopeExit()
                doc.blank()
                
    class CastedDataVisitor(MethodGenerator.DocVisitor):
        def visitInput(self, inputArg):
            l = ("const {1}* {0}CastedData = "
                 "runtime::data_cast<{1}>({0}Data);").format(inputArg.ident,
                inputArg.dataType.typeId())
            self.doc.line(l)
            
        def visitOutput(self, output):
            l = ("{1} * {0}CastedData = "
                 "runtime::data_cast<{1}>({0}Data);").format(output.ident,
                output.dataType.typeId())
            self.doc.line(l)
    
    class InitInVisitor(MethodGenerator.DocVisitor):
        def visitOutput(self, output):
            self.doc.document(output.initIn)
            
    class CvDataVisitor(MethodGenerator.DocVisitor):
        def visitInput(self, inputArg):
            cvData = "{0} {1}CvData".format(inputArg.cvType.typeId(), 
                                            inputArg.ident)
            castedData = "*{0}CastedData".format(inputArg.ident)
            cast = inputArg.cvType.cast(castedData)
            l = "{0} = {1};".format(cvData, cast)
            self.doc.line(l)
            
        def visitOutput(self, inputArg):
            cvData = "{0} {1}CvData".format(inputArg.cvType.typeId(), 
                                            inputArg.ident)
            castedData = "*{0}CastedData".format(inputArg.ident)
            cast = inputArg.cvType.cast(castedData)
            l = "{0} = {1};".format(cvData, cast)
            self.doc.line(l)
            
        def visitAllocation(self, allocation):
            cvData = "{0} {1}CvData;".format(allocation.cvType.typeId(), 
                                             allocation.ident)
            self.doc.line(cvData)
            
        def visitParameter(self, parameter):
            cvData = "{0} {1}CvData".format(parameter.cvType.typeId(), 
                                            parameter.ident)
            castedData = parameter.cvType.cast(parameter.ident.attribute())
            self.doc.line("{0} = {1};".format(cvData, castedData))
            
        def visitNumericParameter(self, numericParameter):
            self.visitParameter(numericParameter)
            
        def visitEnumParameter(self, parameter):
            ident = parameter.ident
            cvData = "{0} {1}CvData".format(parameter.cvType.typeId(), 
                                            ident)
            castedData = "convert{0}({1})".format(ident.className(),
                                                   ident.attribute())
            self.doc.line("{0} = {1};".format(cvData, castedData))
            
        def visitRefInput(self, refInput):
            cvData = "{0} {1}CvData".format(refInput.cvType.typeId(), 
                                             refInput.ident)
            rhs = "{0}CvData".format(refInput.refArg.ident)
            self.doc.line("{0} = {1};".format(cvData, rhs))
            
    class MethodArgumentVisitor(ArgumentVisitor):
        def __init__(self):
            self.args = []
            
        def visitInput(self, inputArg):
            self.visit(inputArg)
            
        def visitOutput(self, output):
            self.visit(output)
            
        def visitAllocation(self, allocation):
            self.visit(allocation)
            
        def visitParameter(self, parameter):
            self.visit(parameter)
            
        def visitNumericParameter(self, numericParameter):
            self.visit(numericParameter)
            
        def visitEnumParameter(self, refInput):
            self.visit(refInput)
            
        def visitConstant(self, constant):
            self.args.append(constant.value)
            
        def visitRefInput(self, refInput):
            self.visit(refInput)
            
        def visit(self, arg):
            self.args.append("{0}CvData".format(arg.ident))
            
        def visitCompound(self, compound):
            self.args.append(compound.create())
            
        def export(self):
            argStr = ""
            for i, arg in enumerate(self.args):
                argStr += arg
                if i < len(self.args) - 1:
                    argStr += ", "
            return argStr
            
    class OutDataVisitor(MethodGenerator.DocVisitor):
        def visitOutput(self, output):
            l = "runtime::DataContainer outContainer = inContainer;";
            self.doc.line(l)
            l = ("runtime::Id2DataPair outputMapper({0}, "
                 "outContainer);").format(output.ident.constant());
            self.doc.line(l)
            
        def visitAllocation(self, allocation):
            dataType = allocation.dataType.typeId()
            ident = allocation.ident
            cvData = "{0}CvData".format(ident)
            cast = allocation.dataType.cast(cvData)
            l = "{0}* {1}CastedData = new {2};".format(dataType, ident, cast)
            self.doc.line(l)
            l = ("runtime::DataContainer outContainer = "
                 "runtime::DataContainer({0}CastedData);").format(ident)
            self.doc.line(l)
            l = ("runtime::Id2DataPair outputMapper({0}, "
                 "outContainer);").format(allocation.ident.constant());
            self.doc.line(l)
        
    class InitOutVisitor(MethodGenerator.DocVisitor):
        def visitAllocation(self, allocation):
            self.doc.document(allocation.initOut)
                
    class EnumConversionDefVisitor(MethodGenerator.DocVisitor):
        def __init__(self, doc, m):
            super(OpImplGenerator.EnumConversionDefVisitor, self).__init__(doc)
            self.m = m
            
        def visitEnumParameter(self, parameter):
            name = parameter.ident.className()
            l = ("int {1}::convert{0}(const runtime::Enum & value)"
                ).format(name, self.m.ident.className())
            self.doc.line(l)
            self.doc.scopeEnter()
            self.doc.line("switch(int(value))")
            self.doc.scopeEnter()
            for desc in parameter.descriptions:
                self.doc.label("case {0}".format(desc.ident))
                self.doc.line("return {0};".format(desc.cvIdent))
            self.doc.label("default")
            self.doc.line(("throw runtime::WrongParameterValue(parameter({0}),"
                           " *this);").format(parameter.ident.constant()))
            self.doc.scopeExit()
            self.doc.scopeExit()
            self.doc.blank()
        
    def generate(self):        
        self.__includes()
        self.namespaceEnter()
        self.__statics()
        self.__constructor()
        self.__getParameter()
        self.__setParameter()
        self.__setupInitParameters()
        self.__setupParameters()
        self.__setupInputs()
        self.__setupOutputs()
        self.__initialize()
        self.__execute()
        self.__convertEnumValues()
        self.namespaceExit()
        
        with file("{0}.cpp".format(self.m.ident.className()), "w") as f:
            f.write(self.doc.string())
    
    def __includes(self):
        self.doc.line('#include "stromx/{0}/{1}.h"'\
            .format(self.p.ident, self.m.ident.className()))
        self.doc.blank()
        self.doc.line('#include "stromx/{0}/Utility.h"'.format(self.p.ident))
        self.doc.line('#include <stromx/example/Image.h>')
        self.doc.line('#include <stromx/example/Matrix.h>')
        self.doc.line('#include <stromx/example/Utilities.h>')
        self.doc.line('#include <stromx/runtime/DataContainer.h>')
        self.doc.line('#include <stromx/runtime/DataProvider.h>')
        self.doc.line('#include <stromx/runtime/Id2DataComposite.h>')
        self.doc.line('#include <stromx/runtime/Id2DataPair.h>')
        self.doc.line('#include <stromx/runtime/ReadAccess.h>')
        self.doc.line('#include <stromx/runtime/WriteAccess.h>')
        self.doc.line('#include <opencv2/{0}/{0}.hpp>'.format(self.p.ident))
        self.doc.blank()    
        
    def __statics(self):
        method = self.m.ident.className()
        package = self.p.ident.constant()
        self.doc.line(("const std::string {0}::PACKAGE(STROMX_{1}_PACKAGE_"
                       "NAME);").format(method, package))
        self.doc.line(("const runtime::Version {0}::VERSION("
                       "STROMX_{1}_VERSION_MAJOR, STROMX_{1}_VERSION_MINOR, "
                       "STROMX_{1}_VERSION_PATCH);".format(method, package)))
        self.doc.line('const std::string {0}::TYPE("{0}");'.format(method))
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
        self.doc.blank()
        
    def __getParameter(self):
        self.doc.line("const runtime::DataRef {0}::getParameter"
                      "(unsigned int id) const"\
                      .format(self.m.ident.className()))
        self.doc.scopeEnter()
        self.doc.line("switch(id)")
        self.doc.scopeEnter()
        
        v = OpImplGenerator.GetParametersVisitor(self.doc)
        self.visitAll(v)
        
        self.doc.label("default")
        self.doc.line("throw runtime::WrongParameterId(id, *this);")
        self.doc.scopeExit()
        self.doc.scopeExit()
        self.doc.blank()
        
    def __setParameter(self):
        self.doc.line("void {0}::setParameter"
                      "(unsigned int id, const runtime::Data& value)"\
                      .format(self.m.ident.className()))
        self.doc.scopeEnter()
        self.doc.line("try")
        self.doc.scopeEnter()
        self.doc.line("switch(id)")
        self.doc.scopeEnter()
        
        v = OpImplGenerator.SetParametersVisitor(self.doc)
        self.visitAll(v)
        
        self.doc.label("default")
        self.doc.line("throw runtime::WrongParameterId(id, *this);")
        self.doc.scopeExit()
        self.doc.scopeExit()
        self.doc.line("catch(runtime::BadCast&)")
        self.doc.scopeEnter()
        self.doc.line("throw runtime::WrongParameterType(parameter(id), *this);")
        self.doc.scopeExit()
        self.doc.scopeExit()
        self.doc.blank()
        
    def __setupInitParameters(self):
        self.doc.line("const std::vector<const runtime::Parameter*> "
                      "{0}::setupInitParameters()"\
                      .format(self.m.ident.className()))
        self.doc.scopeEnter()
        self.doc.line("std::vector<const runtime::Parameter*> parameters;")
        self.doc.blank()
        
        v = OpImplGenerator.SetupParametersVisitor(self.doc, True)
        self.optionParam.accept(v)
        
        self.doc.line("return parameters;")
        self.doc.scopeExit()
        self.doc.blank()
        
    def __setupParameters(self):
        self.doc.line("const std::vector<const runtime::Parameter*> "
                      "{0}::setupParameters()"\
                      .format(self.m.ident.className()))
        self.doc.scopeEnter()
        self.doc.line("std::vector<const runtime::Parameter*> parameters;")
        self.doc.blank()
        
        self.doc.line("switch(int({0}))".format(
                                        self.optionParam.ident.attribute()))
        self.doc.scopeEnter()
        for o in self.m.options:
            self.doc.label("case({0})".format(o.ident.constant()))
            self.doc.scopeEnter()
            
            v = OpImplGenerator.SetupParametersVisitor(self.doc)
            for arg in o.args:
                arg.accept(v)
                
            self.doc.scopeExit()
            self.doc.line("break;")
        self.doc.scopeExit()
        self.doc.blank()
        
        self.doc.line("return parameters;")
        self.doc.scopeExit()
        self.doc.blank()
        
    def __setupInputs(self):
        self.doc.line("const std::vector<const runtime::Description*> "
                      "{0}::setupInputs()"\
                      .format(self.m.ident.className()))
        self.doc.scopeEnter()
        self.doc.line("std::vector<const runtime::Description*> inputs;")
        self.doc.blank()
        
        self.doc.line("switch(int({0}))".format(
                                        self.optionParam.ident.attribute()))
        self.doc.scopeEnter()
        for o in self.m.options:
            self.doc.label("case({0})".format(o.ident.constant()))
            self.doc.scopeEnter()
            
            v = OpImplGenerator.SetupInputsVisitor(self.doc)
            for arg in o.args:
                arg.accept(v)
                
            self.doc.scopeExit()
            self.doc.line("break;")
        self.doc.scopeExit()
        self.doc.blank()
        
        self.doc.line("return inputs;")
        self.doc.scopeExit()
        self.doc.blank()
        
    def __setupOutputs(self):
        self.doc.line("const std::vector<const runtime::Description*> "
                      "{0}::setupOutputs()"\
                      .format(self.m.ident.className()))
        self.doc.scopeEnter()
        self.doc.line("std::vector<const runtime::Description*> outputs;")
        self.doc.blank()
        
        self.doc.line("switch(int({0}))".format(
                                        self.optionParam.ident.attribute()))
        self.doc.scopeEnter()
        for o in self.m.options:
            self.doc.label("case({0})".format(o.ident.constant()))
            self.doc.scopeEnter()
            
            v = OpImplGenerator.SetupOutputsVistor(self.doc)
            self.visitOption(o, v)
                
            self.doc.scopeExit()
            self.doc.line("break;")
        self.doc.scopeExit()
        self.doc.blank()
        
        self.doc.line("return outputs;")
        self.doc.scopeExit()
        self.doc.blank()
        
    def __initialize(self):
        self.doc.line("void {0}::initialize()"\
                      .format(self.m.ident.className()))
        self.doc.scopeEnter()
        self.doc.line("runtime::OperatorKernel::initialize(setupInputs(), "
                      "setupOutputs(), setupParameters());")
        self.doc.scopeExit()
        self.doc.blank()
        
    def __execute(self):
        self.doc.line("void {0}::execute(runtime::DataProvider & provider)"\
                      .format(self.m.ident.className()))
        self.doc.scopeEnter()
        
        self.doc.line("switch(int({0}))".format(
                                        self.optionParam.ident.attribute()))
        self.doc.scopeEnter()
        for o in self.m.options:
            self.doc.label("case({0})".format(o.ident.constant()))
            self.doc.scopeEnter()
            
            v = OpImplGenerator.InputMapperVisitor(self.doc)
            self.visitOption(o, v)
            
            self.doc.blank()
            
            v = OpImplGenerator.ReceiveInputDataVisitor()
            self.visitOption(o, v)
            v.export(self.doc)   
            
            self.doc.blank()
            
            v = OpImplGenerator.InDataVisitor(self.doc)
            self.visitOption(o, v)    
            
            self.doc.blank()
            
            v = OpImplGenerator.AccessVisitor(self.doc)
            self.visitOption(o, v)  
            
            self.doc.blank()
            
            v = OpImplGenerator.CopyWriteAccessVisitor()
            self.visitOption(o, v)
            v.export(self.doc)    
            
            v = OpImplGenerator.CastedDataVisitor(self.doc)
            self.visitOption(o, v) 
            
            self.doc.blank()
            
            v = OpImplGenerator.InitInVisitor(self.doc)
            self.visitOption(o, v) 
            
            self.doc.blank()
            
            v = OpImplGenerator.CvDataVisitor(self.doc)
            self.visitOption(o, v)
            
            self.doc.blank()
            
            v = OpImplGenerator.MethodArgumentVisitor()   
            self.visitOption(o, v)
            
            argStr = v.export()
            self.doc.line("cv::{0}({1});".format(self.m.ident, argStr))
            
            self.doc.blank()
            
            v = OpImplGenerator.OutDataVisitor(self.doc)
            self.visitOption(o, v)
            
            self.doc.blank()
            
            v = OpImplGenerator.InitOutVisitor(self.doc)
            self.visitOption(o, v)
            
            l = "provider.sendOutputData(outputMapper);";
            self.doc.line(l)
                
            self.doc.scopeExit()
            self.doc.line("break;")
        self.doc.scopeExit()
        self.doc.scopeExit()
        self.doc.blank()
        
    def __convertEnumValues(self):
        v = OpImplGenerator.EnumConversionDefVisitor(self.doc, self.m)
        self.visitAll(v, False)


class OpTestHeaderGenerator(MethodGenerator):
    def generate(self):  
        self.__includeGuardEnter()
        self.__includes()
        self.namespaceEnter()
        self.__classEnter()
        self.__testSuiteEnter()
        self.__testSuiteExit()
        self.doc.blank()
        
        self.doc.label("public")
        self.__constructor()
        self.doc.blank()
        
        self.doc.label("protected")
        self.doc.line("void setUp();")
        self.doc.line("void tearDown();")
        self.doc.blank()
        
        self.doc.label("private")
        self.doc.line("runtime::OperatorTester* m_operator;")
        self.__classExit()
        self.namespaceExit()
        self.__includeGuardExit()
        
        with file("test/{0}Test.h".format(self.m.ident.className()), "w") as f:
            f.write(self.doc.string())
        
    def __includeGuardEnter(self):
        self.doc.line("#ifndef {0}".format(self.__includeGuard()))
        self.doc.line("#define {0}".format(self.__includeGuard()))
        self.doc.blank()
    
    def __includes(self):
        self.doc.line('#include "stromx/{0}/Config.h"'.format(self.p.ident))
        self.doc.blank()
        self.doc.line('#include <cppunit/extensions/HelperMacros.h>')
        self.doc.line('#include <cppunit/TestFixture.h>')
        self.doc.blank()
        self.doc.line('#include "stromx/runtime/OperatorTester.h"')
        self.doc.blank()
            
    def __includeGuardExit(self):
        self.doc.line("#endif // {0}".format(self.__includeGuard()))
        
    def __includeGuard(self):
        return "STROMX_{0}_{1}TEST_H".format(self.p.ident.upper(),
                                             self.m.ident.upper())
    def __classEnter(self):
        self.doc.line((
            "class {0}Test : public CPPUNIT_NS::TestFixture"
        ).format(self.m.ident.className()))
        self.doc.line("{")
        self.doc.increaseIndent()  
    
    def __testSuiteEnter(self):
        self.doc.line((
        "CPPUNIT_TEST_SUITE({0}Test);"
    ).format(self.m.ident.className()))
    
    def __testSuiteExit(self):
        self.doc.line("CPPUNIT_TEST_SUITE_END();")
        
    def __constructor(self):
        self.doc.line((
            "{0}Test() : m_operator(0) {{}}"
        ).format(self.m.ident.className()))
        
    def __classExit(self):
        self.doc.decreaseIndent()
        self.doc.line("};")       
            
class OpTestImplGenerator(MethodGenerator):    
    def __includes(self):
        self.doc.line((
            '#include "stromx/{0}/test/{1}Test.h"'
            ).format(self.p.ident, self.m.ident.className()))
        self.doc.blank()
        
        self.doc.line('#include <stromx/runtime/OperatorException.h>')
        self.doc.line('#include <stromx/runtime/ReadAccess.h>')
        self.doc.line('#include "stromx/example/Image.h"')
        self.doc.line((
            '#include "stromx/{0}/{1}.h"'
            ).format(self.p.ident, self.m.ident.className()))
        self.doc.blank()
        
    def __testSuite(self):
        self.doc.line((
            "CPPUNIT_TEST_SUITE_REGISTRATION (stromx::{0}::{1}Test);"
            ).format(self.p.ident, self.m.ident.className()))
        self.doc.blank()
        
    def __setUp(self):
        className = self.m.ident.className()
        self.doc.line("void {0}Test::setUp()".format(className))
        self.doc.scopeEnter()
        self.doc.line((
            "m_operator = new stromx::runtime::OperatorTester(new {0});"
            ).format(self.m.ident.className()))
        self.doc.scopeExit()
        self.doc.blank()
        
    def __tearDown(self):
        className = self.m.ident.className()
        self.doc.line("void {0}Test::tearDown()".format(className))
        self.doc.scopeEnter()
        self.doc.line("delete m_operator;")
        self.doc.scopeExit()
        self.doc.blank()

    def generate(self):  
        self.__includes()
        self.__testSuite()
        self.namespaceEnter()
        self.__setUp()
        self.__tearDown()
        self.namespaceExit()
        
        with file("test/{0}Test.cpp".format(self.m.ident.className()), "w") as f:
            f.write(self.doc.string())
        
def generateMethodFiles(package, method):
    g = OpHeaderGenerator()
    g.save(package, method)
    
    g = OpImplGenerator()
    g.save(package, method)
    
    g = OpTestHeaderGenerator()
    g.save(package, method)
    
    g = OpTestImplGenerator()
    g.save(package, method)
        
if __name__ == "__main__":
    import doctest
    doctest.testmod()     
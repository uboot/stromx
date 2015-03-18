# -*- coding: utf-8 -*-

import document
import interface
import package

import rulegenerator
import testgenerator

class ArgumentVisitorBase(interface.ArgumentVisitor):
    def visitReturnValue(self, retValue):
        self.visitAllocation(retValue)
    
class SingleArgumentVisitor(ArgumentVisitorBase):
    """
    Visitor which handles compound arguments and calls the accept methods
    of each component of the compound arguments. I.e. derived visitors do not
    have to care about compound arguments they only have to consider "single"
    arguments.
    """
    def visitCompound(self, compound):
        for arg in compound.args:
            if not arg is None:
                arg.accept(self)
            
    
class CollectVisitor(SingleArgumentVisitor):
    """
    Visitor stores all arguments it visits the common set self.args.
    """
    def __init__(self):
        self.args = set()
        
    def visitInput(self, inputArg):
        self.args.add(inputArg)
        
    def visitParameter(self, parameter):
        self.args.add(parameter)
        
    def visitConstant(self, const):
        self.args.add(const)
        
    def visitRefInput(self, refInput):
        self.args.add(refInput)
        
    def visitAllocation(self, allocation):
        self.args.add(allocation)
                           
    def visitInputOutput(self, inputOutput):
        self.args.add(inputOutput)
        
    def visitOutput(self, output):
        self.args.add(output)
        
class MethodGenerator(object):
    """
    Abstract base class of all generators which output files depending on 
    operators. It provides utility functions used by the derived classes.
    """
    class CollectParametersVisitor(SingleArgumentVisitor):
        def __init__(self):
            self.params = []
    
        def visitParameter(self, parameter):
            self.params.append(parameter)
            
    class DocVisitor(SingleArgumentVisitor):
        """ 
        Visitor which holds a document.
        """
        def __init__(self, doc):
            self.doc = doc
            
    def __init__(self):
        self.p = None
        self.m = None
        self.doc = None
    
    def save(self, package, method, printResult = False):
        """
        Writes the output of the generator for the input method to the current
        document and optionally prints it to the standard output.
        """
        self.p = package
        self.m = method
        self.doc = document.Document()
        self.optionParam = self.createOptionParameter()
        
        self.generate()
        
        if printResult:
            print self.doc.string()
            
    def createOptionParameter(self):
        """
        Creates and returns an enum parameters which provides one value for
        each option of the method.
        """
        p = package.EnumParameter("dataFlow", "Data flow")
        p.isInit = True
        for opt in self.m.options:
            desc = package.EnumDescription(opt.ident.constant(), str(opt.name))
            desc.name = opt.name
            p.descriptions.append(desc)
        return p
            
    def visitAll(self, visitor, visitOptionParam = True):
        """
        Collects all arguments of all options and removes duplicates (i.e. 
        arguments with common identifier). Then the visitor visits all 
        remaining arguments and the option parameter if the according flag is
        set to true.
        """
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
            
        for arg in sorted(filteredArgs, key=lambda arg: str(arg.ident)):
            arg.accept(visitor)
                
        if visitOptionParam and self.optionParam:
            self.optionParam.accept(visitor)
            
    def visitOption(self, opt, visitor):
        """
        The visitor visits all arguments of the given option.
        """
        for arg in opt.args:
            arg.accept(visitor)
        
    def namespaceEnter(self):
        """
        Enters the namespace of the package the method belongs to.
        """
        self.doc.namespaceEnter("stromx")
        self.doc.namespaceEnter(self.p.ident)
        
    def namespaceExit(self):
        """
        Exits the package namespace.
        """
        self.doc.namespaceExit(self.p.ident)
        self.doc.namespaceExit("stromx")
        self.doc.blank()
            
class OpHeaderGenerator(MethodGenerator):
    """
    Generates the header of a method operator.
    """
    class ConnectorEnumVisitor(SingleArgumentVisitor):
        """
        Exports the enumeration of the IDs of all visited input and output
        connectors.
        """
        def __init__(self):
            self.connectors = set()
            
        def visitRefInput(self, refInputArg):
            self.connectors.add(refInputArg)
    
        def visitInput(self, inputArg):
            self.connectors.add(inputArg)
    
        def visitInputOutput(self, arg):
            self.connectors.add(arg)
    
        def visitOutput(self, output):
            self.connectors.add(output)
            
        def visitAllocation(self, allocation):
            self.connectors.add(allocation)
            
        def export(self, doc):
            connectorIds = [i.ident.constant() for i in self.connectors]
            doc.enum("ConnectorId", set(connectorIds))
            
    class ParameterEnumVisitor(MethodGenerator.CollectParametersVisitor):
        """
        Exports the enumeration of the parameter IDs of all visited parameters.
        """
        def export(self, doc):
            paramIds = [p.ident.constant() for p in self.params]
            doc.enum("ParameterId", set(paramIds))
        
    class DataMemberVisitor(MethodGenerator.DocVisitor):
        """
        Exports class members for the values of all visited parameters.
        """
        def visitParameter(self, parameter):
            l = "{0} {1};".format(parameter.dataType.concreteTypeId(),
                                  parameter.ident.attribute())
            self.doc.line(l)
        
    class DescriptionsVisitor(MethodGenerator.DocVisitor):
        """
        Exports class members for the parameter description of all visited
        parameters.
        """
        def visitParameter(self, parameter):
            if parameter.argType == package.ArgType.PLAIN:
                self.doc.line(("runtime::Parameter* m_{0}Parameter;"
                              ).format(parameter.ident))
            elif parameter.argType == package.ArgType.ENUM:
                self.doc.line(("runtime::EnumParameter* m_{0}Parameter;"
                              ).format(parameter.ident))
            elif parameter.argType == package.ArgType.NUMERIC:
                self.doc.line(("runtime::NumericParameter<{1}>* m_{0}Parameter;"
                              ).format(parameter.ident,
                                       parameter.dataType.typeId()))
            elif parameter.argType == package.ArgType.MATRIX:
                self.doc.line(("runtime::MatrixParameter* m_{0}Parameter;"
                              ).format(parameter.ident))
            else:
                assert(False)
        
        def visitOutput(self, arg):
            self.visitInput(arg)
            
        def visitInputOutput(self, arg):
            self.visitInput(arg)
            
        def visitAllocation(self, arg):
            self.visitInput(arg)
            
        def visitRefInput(self, arg):
            self.visitInput(arg)
            
        def visitInput(self, arg):
            if arg.argType == package.ArgType.MATRIX:
                self.doc.line((
                    "runtime::MatrixDescription* m_{0}Description;"
                ).format(arg.ident))
            else:
                self.doc.line((
                    "runtime::Description* m_{0}Description;"
                ).format(arg.ident))
            
    class EnumParameterIdVisitor(MethodGenerator.DocVisitor):
        """
        Exports enumerations for the IDs of all visited enumeration parameters.
        """
        def visitParameter(self, parameter):
            if parameter.argType != package.ArgType.ENUM:
                return
                
            keys = []
            for desc in parameter.descriptions:
                keys.append(desc.ident)
            enumName = "{0}Id".format(parameter.ident.className())
            self.doc.enum(enumName, keys)
            
    class EnumConversionDeclVisitor(MethodGenerator.DocVisitor):
        """
        Exports declarations of conversion functions for each visited 
        enumeration parameter.
        """
        def visitParameter(self, parameter):
            if parameter.argType != package.ArgType.ENUM:
                return
                
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
        
        v = OpHeaderGenerator.DescriptionsVisitor(self.doc)
        self.visitAll(v)
        
        self.__classExit()
        self.namespaceExit()
        self.__includeGuardExit()
        
        filename = "stromx/{0}/{1}.h".format(self.p.ident,
                                             self.m.ident.className())
        with file(filename, "w") as f:
            f.write(self.doc.string())
    
    def __includes(self):
        self.doc.line('#include "stromx/{0}/Config.h"'.format(self.p.ident))
        self.doc.line('#include <stromx/cvsupport/Matrix.h>')
        self.doc.line('#include <stromx/runtime/Enum.h>')
        self.doc.line('#include <stromx/runtime/EnumParameter.h>')
        self.doc.line('#include <stromx/runtime/List.h>')
        self.doc.line('#include <stromx/runtime/MatrixDescription.h>')
        self.doc.line('#include <stromx/runtime/MatrixParameter.h>')
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
    """
    Generates the header of a method operator.
    """
    class ParameterInitVisitor(MethodGenerator.CollectParametersVisitor):
        """
        Exports the constructor initialization for all visited parameter data 
        members .
        """
        def export(self, doc):
            for i, p in enumerate(self.params):
                defaultValue = p.default if p.default != None else ""
                defaultValue = document.pythonToCpp(defaultValue)
                init = "{0}({1})".format(p.ident.attribute(), defaultValue)
                if i != len(self.params) - 1:
                    doc.line("{0},".format(init))
                else:
                    doc.line(init)
            
    class GetParametersVisitor(MethodGenerator.DocVisitor):
        """
        Exports case sections which return the values of all visited 
        parameters.
        """
        def visitParameter(self, parameter):
            self.doc.label("case {0}".format(parameter.ident.constant()))
            self.doc.line("return {0};".format(parameter.ident.attribute()))
                    
    class SetParametersVisitor(MethodGenerator.DocVisitor):
        """
        Exports case sections which set the values of all visited parameters.
        """
        def visitParameter(self, parameter):
            l = ""
            if parameter.argType == package.ArgType.PLAIN:
                pass
            elif parameter.argType == package.ArgType.ENUM:
                l = ("cvsupport::checkEnumValue(castedValue, {0}Parameter, *this);"
                    ).format(parameter.ident.attribute())
            elif parameter.argType == package.ArgType.NUMERIC:
                l = ("cvsupport::checkNumericValue(castedValue, {0}Parameter, *this);"
                    ).format(parameter.ident.attribute())
            elif parameter.argType == package.ArgType.MATRIX:
                l = ("cvsupport::checkMatrixValue(castedValue, {0}Parameter, *this);"
                    ).format(parameter.ident.attribute())
            else:
                assert(False)
                
            self.__setParameterWithCheck(parameter, l)
            
        def __setParameterWithCheck(self, parameter, check):
            self.doc.label("case {0}".format(parameter.ident.constant()))
            self.doc.scopeEnter()
            self.doc.line(("const {0} & castedValue = runtime::data_cast<{1}>(value);"
                          ).format(parameter.dataType.typeId(),
                                   parameter.dataType.typeId()))
            
            l = ("if(! castedValue.variant().isVariant({0}))".format(
                                                parameter.dataType.variant()))
            self.doc.line(l)
            self.doc.scopeEnter()
            l = 'throw runtime::WrongParameterType(parameter(id), *this);'
            self.doc.line(l)
            self.doc.scopeExit()
            
            if check != "":
                self.doc.line(check)
            
            checkParams = rulegenerator.CheckParameterVisitor(self.doc,
                                                              parameter)
            for rule in parameter.rules:
                rule.accept(checkParams)
                
            self.doc.line(("{0} = castedValue;"
                          ).format(parameter.ident.attribute()))
            self.doc.scopeExit()
            self.doc.line("break;")
                
    class SetupParametersVisitor(MethodGenerator.DocVisitor):
        """
        Exports the allocation of the descriptions of all visited parameters.
        """
        def __init__(self, doc, isInit = False):
            super(OpImplGenerator.SetupParametersVisitor, self).__init__(doc)
            self.isInit = isInit
            
        def visitParameter(self, parameter):
            if parameter.argType == package.ArgType.PLAIN:
                self.__visitPlainParameter(parameter)
            elif parameter.argType == package.ArgType.ENUM:
                self.__visitEnumParameter(parameter)
            elif parameter.argType == package.ArgType.MATRIX:
                self.__visitMatrixParameter(parameter)
            elif parameter.argType == package.ArgType.NUMERIC:
                self.__visitNumericParameter(parameter)
            else:
                assert(False)
            
        def __visitPlainParameter(self, parameter):
            ident = "m_{0}Parameter".format(parameter.ident)
            l = "{0} = new runtime::Parameter({1}, {2});"\
                .format(ident, parameter.ident.constant(),
                        parameter.dataType.variant())
            self.doc.line(l)
            self.__accessMode(ident)
            l = '{0}->setTitle(L_("{1}"));'.format(ident, parameter.name)
            self.doc.line(l)
            l = "parameters.push_back({0});".format(ident)
            self.doc.line(l)
            self.doc.blank()
            
        def __visitEnumParameter(self, parameter):
            ident = "m_{0}Parameter".format(parameter.ident)
            l = ("{0} = new runtime::EnumParameter({1});"
                ).format(ident, parameter.ident.constant())
            self.doc.line(l)
            self.__accessMode(ident)
            l = '{0}->setTitle(L_("{1}"));'.format(ident, parameter.name)
            self.doc.line(l)

            for desc in parameter.descriptions:
                d = 'runtime::Enum({0})'.format(desc.ident)
                l = '{0}->add(runtime::EnumDescription({1}, L_("{2}")));'\
                    .format(ident, d, desc.name)
                self.doc.line(l)
            l = "parameters.push_back({0});".format(ident)
            self.doc.line(l)
            self.doc.blank()
            
        def __visitMatrixParameter(self, parameter):
            ident = "m_{0}Parameter".format(parameter.ident)
            l = "{0} = new runtime::MatrixParameter({1}, {2});"\
                .format(ident, parameter.ident.constant(),
                        parameter.dataType.variant())
            self.doc.line(l)
            self.__accessMode(ident)
            l = '{0}->setTitle(L_("{1}"));'.format(ident, parameter.name)
            self.doc.line(l)
            self.doc.line("{0}->setRows({1});".format(ident, parameter.rows))
            self.doc.line("{0}->setCols({1});".format(ident, parameter.cols))
            l = "parameters.push_back({0});".format(ident)
            self.doc.line(l)
            self.doc.blank()
            
        def __visitNumericParameter(self, parameter):
            ident = "m_{0}Parameter".format(parameter.ident)
            l = ("{0} = new runtime::NumericParameter<{2}>({1});"
                ).format(ident, parameter.ident.constant(),
                         parameter.dataType.typeId())
            self.doc.line(l)
            self.__accessMode(ident)
            l = '{0}->setTitle(L_("{1}"));'\
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
        """
        Exports the allocation of the descriptions of all visited outputs.
        """
        def visitInputOutput(self, arg):
            self.visitOutput(arg)
            
        def visitOutput(self, output):
            if output.argType == package.ArgType.PLAIN:
                self.__setupDescription(output)
            elif output.argType == package.ArgType.MATRIX:
                self.__setupMatrixDescription(output)
            else:
                assert(False)
        
        def visitAllocation(self, allocation):
            self.visitOutput(allocation)
            
        def __setupDescription(self, arg):
            l = "runtime::Description* {0} = new runtime::Description({1}, {2});"\
                .format(arg.ident, arg.ident.constant(),
                        arg.dataType.variant())
            self.doc.line(l)
            l = '{0}->setTitle(L_("{1}"));'.format(arg.ident, arg.name)
            self.doc.line(l)
            l = "outputs.push_back({0});".format(arg.ident)
            self.doc.line(l)
            self.doc.blank()
            
        def __setupMatrixDescription(self, arg):
            l = "runtime::MatrixDescription* {0} = new runtime::MatrixDescription({1}, {2});"\
                .format(arg.ident, arg.ident.constant(),
                        arg.dataType.variant())
            self.doc.line(l)
            l = '{0}->setTitle(L_("{1}"));'.format(arg.ident, arg.name)
            self.doc.line(l)
            l = '{0}->setRows({1});'.format(arg.ident, arg.rows)
            self.doc.line(l)
            l = '{0}->setCols({1});'.format(arg.ident, arg.cols)
            self.doc.line(l)
            l = "outputs.push_back({0});".format(arg.ident)
            self.doc.line(l)
            self.doc.blank()
            
    class SetupInputsVisitor(MethodGenerator.DocVisitor):
        """
        Exports the allocation of the descriptions of all visited inputs.
        """
        def visitOutput(self, arg):
            if arg.argType == package.ArgType.PLAIN:
                self.__setupDescription(arg, True)
            elif arg.argType == package.ArgType.MATRIX:
                self.__setupMatrixDescription(arg, True)
            else:
                assert(False)
        
        def visitInput(self, arg):
            if arg.argType == package.ArgType.PLAIN:
                self.__setupDescription(arg, False)
            elif arg.argType == package.ArgType.MATRIX:
                self.__setupMatrixDescription(arg, False)
            else:
                assert(False)
        
        def visitInputOutput(self, arg):
            self.visitInput(arg)
            
        def __setupDescription(self, arg, isOutput):
            description = "{0}Description".format(arg.ident.attribute())
            l = "{0} = new runtime::Description({1}, {2});"\
                .format(description, arg.ident.constant(),
                        self.__getVariant(arg, isOutput))
            self.doc.line(l)
            l = '{0}->setTitle(L_("{1}"));'\
                .format(description, arg.name)
            self.doc.line(l)
            l = "inputs.push_back({0});".format(description)
            self.doc.line(l)
            self.doc.blank()
            
        def __setupMatrixDescription(self, arg, isOutput):
            description = "{0}Description".format(arg.ident.attribute())
            l = (
                "{0} = new "
                "runtime::MatrixDescription({1}, {2});"
            ).format(description, arg.ident.constant(), 
                     self.__getVariant(arg, isOutput))
            self.doc.line(l)
            l = '{0}->setTitle("{1}");'.format(description, arg.name)
            self.doc.line(l)
            l = '{0}->setRows({1});'.format(description, arg.rows)
            self.doc.line(l)
            l = '{0}->setCols({1});'.format(description, arg.cols)
            self.doc.line(l)
            l = "inputs.push_back({0});".format(description)
            self.doc.line(l)
            self.doc.blank()
            
        def __getVariant(self, arg, isOutput):
            if isOutput:
                return arg.dataType.canBeCreatedFromVariant()
            else:
                return arg.dataType.variant()
            
    class InputMapperVisitor(MethodGenerator.DocVisitor):
        """
        Exports input mappers for all visited inputs and outputs.
        """
        def visitInput(self, arg):
            self.__visit(arg)
            
        def visitOutput(self, arg):
            self.__visit(arg)
            
        def visitInputOutput(self, arg):
            self.__visit(arg)
            
        def __visit(self, arg):
            ident = arg.ident
            constant = arg.ident.constant()
            l = "runtime::Id2DataPair {0}InMapper({1});".format(ident, constant)
            self.doc.line(l)
    
    class ReceiveInputDataVisitor(SingleArgumentVisitor):
        """
        Exports the receive input command for all visited inputs and outputs.
        """
        def __init__(self):
            self.line = ""
            
        def visitInput(self, inputArg):
            self.__visit(inputArg)
            
        def visitOutput(self, output):
            self.__visit(output)
            
        def visitInputOutput(self, arg):
            self.__visit(arg)
            
        def export(self, doc):
            if self.line != "":
                doc.line("provider.receiveInputData({0});".format(self.line))
            
        def __visit(self, arg):
            if self.line == "":
                self.line = "{0}InMapper".format(arg.ident)
            else:
                self.line += " && {0}InMapper".format(arg.ident)
           
    class InDataVisitor(MethodGenerator.DocVisitor):
        """
        Exports stromx::Data* variables for all visited inputs and outputs.
        """
        def visitInput(self, inputArg):
            self.doc.line(("const runtime::Data* "
                           "{0}Data = 0;").format(inputArg.ident))
                           
        def visitInputOutput(self, arg):
            self.visitOutput(arg)
            
        def visitOutput(self, output):
            self.doc.line("runtime::Data* {0}Data = 0;".format(output.ident))
            
    class AccessVisitor(MethodGenerator.DocVisitor):
        """
        Exports data accessors for all visited inputs and outputs.
        """
        def visitInput(self, inputArg):
            self.doc.line(("runtime::ReadAccess<> "
                           "{0}ReadAccess;").format(inputArg.ident))
                           
        def visitInputOutput(self, arg):
            self.visitOutput(arg)
                        
        def visitOutput(self, output):
            mapper = "{0}InMapper".format(output.ident)
            data = "{0}Data".format(output.ident)
            self.doc.line(("runtime::DataContainer inContainer = "
                           "{0}.data();").format(mapper))
            self.doc.line("runtime::WriteAccess<> writeAccess(inContainer);")
            self.doc.line("{0} = &writeAccess();".format(data))
            
    class CopyWriteAccessVisitor(SingleArgumentVisitor):
        """
        Exports the if-conditions which either create a read access or 
        reference an existing write access to read each visited input.
        """
        def __init__(self):
            self.output = None
            self.inputs = []
            
        def visitInput(self, inputArg):
            self.inputs.append(inputArg)
                           
        def visitInputOutput(self, arg):
            self.visitOutput(arg)
            
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
                if i.inPlace:
                    doc.line("srcData = &writeAccess();")
                else:
                    message = '"Can not operate in place."'
                    ex = (
                        "throw runtime::InputError({0}, *this, {1});"
                    ).format(i.ident.constant(), message)
                    doc.line(ex)
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
                
    class CheckVariantVisitor(MethodGenerator.DocVisitor):
        """
        Exports the variant check for each visited input.
        """
        def visitInput(self, inputArg):
            self.__visit(inputArg)
                           
        def visitInputOutput(self, arg):
            self.__visit(arg)
            
        def visitOutput(self, output):
            self.__visit(output)
            
        def __visit(self, arg):
            l = (
                "if(! {0}Data->variant().isVariant({1}Description->variant()))"
            ).format(arg.ident, arg.ident.attribute())
            self.doc.line(l)
            self.doc.scopeEnter()
            l = (
                'throw runtime::InputError({0}, *this, "Wrong input data '
                'variant.");'
            ).format(arg.ident.constant())
            self.doc.line(l)
            self.doc.scopeExit()
                
    class CastedDataVisitor(MethodGenerator.DocVisitor):
        """
        Exports the cast to a concrete stromx data type for each visited
        input and output.
        """
        def visitInput(self, inputArg):
            l = ("const {1}* {0}CastedData = "
                 "runtime::data_cast<{1}>({0}Data);").format(inputArg.ident,
                inputArg.dataType.typeId())
            self.doc.line(l)
                           
        def visitInputOutput(self, arg):
            self.visitOutput(arg)
            
        def visitOutput(self, output):
            l = ("{1} * {0}CastedData = "
                 "runtime::data_cast<{1}>({0}Data);").format(output.ident,
                output.dataType.typeId())
            self.doc.line(l)
                
    class CheckCastedDataVisitor(MethodGenerator.DocVisitor):
        """
        Exports the data check for the data check of each visited input.
        """
        def visitInput(self, inputArg):
            self.__visit(inputArg)
                           
        def visitInputOutput(self, arg):
            self.__visit(arg)
            
        def visitOutput(self, output):
            self.__visit(output)
            
        def __visit(self, arg):
            if arg.argType == package.ArgType.MATRIX:
                l = (
                    "cvsupport::checkMatrixValue(*{0}CastedData, {1}Description, *this);"
                ).format(arg.ident, arg.ident.attribute())
                self.doc.line(l)
            else:
                pass
    
    class InitInVisitor(MethodGenerator.DocVisitor):
        """
        Exports the initialization of the argument before the OpenCV
        function is called.
        """
        def visitConstant(self, arg):
            self.__visit(arg)
            
        def visitInputOutput(self, arg):
            self.__visit(arg)
            
        def visitOutput(self, output):
            self.__visit(output)
            
        def __visit(self, arg):
            self.doc.document(arg.initIn)
        
            
    class CvDataVisitor(MethodGenerator.DocVisitor):
        """
        Exports the conversion to a native or OpenCV data type for each visited
        argument.
        """
        def visitInput(self, inputArg):
            cvData = "{0} {1}CvData".format(inputArg.cvType.typeId(), 
                                            inputArg.ident)
            castedData = "*{0}CastedData".format(inputArg.ident)
            cast = inputArg.cvType.cast(castedData)
            l = "{0} = {1};".format(cvData, cast)
            self.doc.line(l)
                           
        def visitInputOutput(self, arg):
            self.visitOutput(arg)
            
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
            if parameter.argType == package.ArgType.ENUM:
                self.__visitEnumParameter(parameter)
            else:
                cvData = "{0} {1}CvData".format(parameter.cvType.typeId(), 
                                                parameter.ident)
                castedData = parameter.cvType.cast(parameter.ident.attribute())
                self.doc.line("{0} = {1};".format(cvData, castedData))
            
        def __visitEnumParameter(self, parameter):
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
            
    class MethodArgumentVisitor(ArgumentVisitorBase):
        """
        Exports the argument of the OpenCV function for each visited argument.
        """
        def __init__(self):
            self.args = []
            
        def visitInput(self, inputArg):
            self.visit(inputArg)
                           
        def visitInputOutput(self, arg):
            self.visitOutput(arg)
            
        def visitOutput(self, output):
            self.visit(output)
            
        def visitAllocation(self, allocation):
            self.visit(allocation)
            
        def visitParameter(self, parameter):
            self.visit(parameter)
            
        def visitConstant(self, constant):
            value = constant.value
            value = document.pythonToCpp(value)
            self.args.append(str(value))
            
        def visitRefInput(self, refInput):
            self.visit(refInput)
            
        def visitReturnValue(self, retValue):
            pass
            
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
            
    class MethodReturnValueVisitor(ArgumentVisitorBase):
        """
        Exports the return value of the OpenCV function out of each visited argument.
        """
        def __init__(self):
            self.returnValue = ""
            
        def visitReturnValue(self, retVal):
            self.returnValue = "{0}CvData = ".format(retVal.ident)
            
        def export(self):
            return self.returnValue
            
    class OutDataVisitor(MethodGenerator.DocVisitor):
        """
        Exports the wrapping of the result data into a data container for
        each visited output or allocation.
        """
        def visitInputOutput(self, arg):
            self.visitOutput(arg)
            
        def visitOutput(self, output):
            l = "runtime::DataContainer {0}OutContainer = inContainer;".format(output.ident)
            self.doc.line(l)
            l = ("runtime::Id2DataPair {0}OutMapper({1}, "
                 "{0}OutContainer);").format(output.ident, output.ident.constant());
            self.doc.line(l)
            
        def visitAllocation(self, allocation):
            dataType = allocation.dataType.typeId()
            ident = allocation.ident
            cvData = "{0}CvData".format(ident)
            newObject = allocation.dataType.allocate(cvData)
            l = "{0}* {1}CastedData = {2};".format(dataType, ident, newObject)
            self.doc.line(l)
            l = ("runtime::DataContainer {0}OutContainer = "
                 "runtime::DataContainer({0}CastedData);").format(ident)
            self.doc.line(l)
            l = ("runtime::Id2DataPair {0}OutMapper({1}, "
                 "{0}OutContainer);").format(ident, allocation.ident.constant())
            self.doc.line(l)
        
    class InitOutVisitor(MethodGenerator.DocVisitor):
        """
        Exports the initialization of the output argument after the OpenCV
        function is called.
        """
        def visitAllocation(self, allocation):
            self.doc.document(allocation.initOut)
            
    class SendOutputDataVisitor(SingleArgumentVisitor):
        """
        Exports the send output command for all visited outputs.
        """
        def __init__(self):
            self.line = ""
            
        def visitAllocation(self, output):
            self.__visit(output)
            
        def visitOutput(self, output):
            self.__visit(output)
            
        def visitInputOutput(self, arg):
            self.__visit(arg)
            
        def export(self, doc):
            if self.line != "":
                doc.line("provider.sendOutputData({0});".format(self.line))
            
        def __visit(self, arg):
            if self.line == "":
                self.line = "{0}OutMapper".format(arg.ident)
            else:
                self.line += " && {0}OutMapper".format(arg.ident)
                
    class EnumConversionDefVisitor(MethodGenerator.DocVisitor):
        """
        Exports the function which converts an enumeration value to its 
        OpenCV value for each visited enumeration parameter.
        """
        def __init__(self, doc, m):
            super(OpImplGenerator.EnumConversionDefVisitor, self).__init__(doc)
            self.m = m
            
        def visitParameter(self, parameter):
            if parameter.argType != package.ArgType.ENUM:
                return
                
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
        
        
        filename = "stromx/{0}/{1}.cpp".format(self.p.ident,
                                               self.m.ident.className())
        with file(filename, "w") as f:
            f.write(self.doc.string())
    
    def __includes(self):
        cvModule = str(self.p.ident)[2:]
        self.doc.line('#include "stromx/{0}/{1}.h"'\
            .format(self.p.ident, self.m.ident.className()))
        self.doc.blank()
        self.doc.line('#include "stromx/{0}/Locale.h"'.format(self.p.ident))
        self.doc.line('#include "stromx/{0}/Utility.h"'.format(self.p.ident))
        self.doc.line('#include <stromx/cvsupport/Image.h>')
        self.doc.line('#include <stromx/cvsupport/Matrix.h>')
        self.doc.line('#include <stromx/cvsupport/Utilities.h>')
        self.doc.line('#include <stromx/runtime/DataContainer.h>')
        self.doc.line('#include <stromx/runtime/DataProvider.h>')
        self.doc.line('#include <stromx/runtime/Id2DataComposite.h>')
        self.doc.line('#include <stromx/runtime/Id2DataPair.h>')
        self.doc.line('#include <stromx/runtime/ReadAccess.h>')
        self.doc.line('#include <stromx/runtime/VariantComposite.h>')
        self.doc.line('#include <stromx/runtime/WriteAccess.h>')
        self.doc.line('#include <opencv2/{0}/{0}.hpp>'.format(cvModule))
        self.doc.blank()    
        
    def __statics(self):
        method = self.m.ident.className()
        package = self.p.ident.upper()
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
        
        if len(self.m.options) > 1:
            v = OpImplGenerator.SetupParametersVisitor(self.doc, isInit = True)
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
            
            v = OpImplGenerator.CheckVariantVisitor(self.doc)
            self.visitOption(o, v) 
            
            self.doc.blank()
            
            v = OpImplGenerator.CastedDataVisitor(self.doc)
            self.visitOption(o, v)  
            
            v = OpImplGenerator.CheckCastedDataVisitor(self.doc)
            self.visitOption(o, v) 
            
            self.doc.blank()
            
            if o.inputCheck != None:
                self.doc.document(o.inputCheck)
                self.doc.blank()
            
            v = OpImplGenerator.InitInVisitor(self.doc)
            self.visitOption(o, v) 
            
            self.doc.blank()
            
            v = OpImplGenerator.CvDataVisitor(self.doc)
            self.visitOption(o, v)
            
            self.doc.blank()
            
            v = OpImplGenerator.MethodReturnValueVisitor()   
            self.visitOption(o, v)
            retVal = v.export()
            
            v = OpImplGenerator.MethodArgumentVisitor()   
            self.visitOption(o, v)
            argStr = v.export()
            
            namespace = ""
            if self.m.namespace != "":
                namespace = "{0}::".format(self.m.namespace)
                
            self.doc.line("{3}{2}{0}({1});".format(self.m.ident, argStr,
                                                   namespace, retVal))
            if o.postCall != None:
                self.doc.document(o.postCall)
                
            self.doc.blank()
            
            v = OpImplGenerator.OutDataVisitor(self.doc)
            self.visitOption(o, v)
            
            self.doc.blank()
            
            v = OpImplGenerator.InitOutVisitor(self.doc)
            self.visitOption(o, v)
            
            v = OpImplGenerator.SendOutputDataVisitor()
            self.visitOption(o, v)
            v.export(self.doc)
                
            self.doc.scopeExit()
            self.doc.line("break;")
        self.doc.scopeExit()
        self.doc.scopeExit()
        self.doc.blank()
        
    def __convertEnumValues(self):
        v = OpImplGenerator.EnumConversionDefVisitor(self.doc, self.m)
        self.visitAll(v, False)

class OpTestGenerator(object):
    """
    Abstract base class of all generators which output operator tests.
    """
    def testNames(self):
        l = []
        for o in self.m.options:
            for i in range(len(o.tests)):
                l.append("test{0}{1}".format(o.ident.className(), i))
        return l
    
class OpTestHeaderGenerator(MethodGenerator, OpTestGenerator):
    """
    Generates the header of an operator test.
    """
    def generate(self):  
        self.__includeGuardEnter()
        self.__includes()
        self.namespaceEnter()
        self.__classEnter()
        self.__testSuite()
        self.doc.blank()
        
        self.doc.label("public")
        self.__constructor()
        self.doc.line("void setUp();")
        self.doc.line("void tearDown();")
        self.doc.blank()
        
        self.doc.label("protected")
        self.__testMethods()
        self.doc.blank()
        
        self.doc.label("private")
        self.doc.line("runtime::OperatorTester* m_operator;")
        self.__classExit()
        self.namespaceExit()
        self.__includeGuardExit()
        
        filename = "stromx/{0}/test/{1}Test.h".format(self.p.ident,
                                                      self.m.ident.className())
        with file(filename, "w") as f:
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
    
    def __testSuite(self):
        self.doc.line((
            "CPPUNIT_TEST_SUITE({0}Test);"
        ).format(self.m.ident.className()))
        for test in self.testNames():
            self.doc.line("CPPUNIT_TEST({0});".format(test))
        self.doc.line("CPPUNIT_TEST_SUITE_END();")
        
    def __constructor(self):
        self.doc.line((
            "{0}Test() : m_operator(0) {{}}"
        ).format(self.m.ident.className()))
        
    def __testMethods(self):
        for test in self.testNames():
            self.doc.line("void {0}();".format(test))
        
    def __classExit(self):
        self.doc.decreaseIndent()
        self.doc.line("};")       
            
class OpTestImplGenerator(MethodGenerator, OpTestGenerator):  
    """
    Generates the implementation of an operator test.
    """  
    def __includes(self):
        self.doc.line((
            '#include "stromx/{0}/test/{1}Test.h"'
            ).format(self.p.ident, self.m.ident.className()))
        self.doc.blank()
        
        self.doc.line('#include <boost/lexical_cast.hpp>')
        self.doc.line('#include <stromx/runtime/OperatorException.h>')
        self.doc.line('#include <stromx/runtime/ReadAccess.h>')
        self.doc.line('#include "stromx/cvsupport/Image.h"')
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
        
    def __testMethods(self):
        className = self.m.ident.className()
        for o in self.m.options:
            for i, test in enumerate(o.tests):
                testName = "test{0}{1}".format(o.ident.className(), i)
                
                self.doc.line(
                    "void {0}Test::{1}()".format(className, testName)
                )
                self.doc.scopeEnter()
                
                if len(self.m.options) > 1:
                    index = "{0}::DATA_FLOW".format(self.m.ident.className())
                    value = (
                        "runtime::Enum({0}::{1})"
                        ).format(self.m.ident.className(), o.ident.constant())
                    l = "m_operator->setParameter({0}, {1});".format(index, value)
                    self.doc.line(l)
                self.doc.line("m_operator->initialize();")
                self.doc.line("m_operator->activate();")
                self.doc.blank();
                
                testgenerator.generate(self.doc, self.m, o.args,
                                       test, testName)
                self.doc.scopeExit()
                self.doc.blank()

    def generate(self):  
        self.__includes()
        self.__testSuite()
        self.namespaceEnter()
        self.__setUp()
        self.__tearDown()
        self.__testMethods()
        self.namespaceExit()
        
        filename = "stromx/{0}/test/{1}Test.cpp".format(self.p.ident,
                                                        self.m.ident.className())
        with file(filename, "w") as f:
            f.write(self.doc.string())
            
def generateMethodFiles(package, method):
    """
    Generates the operator and the operator tests for the given method.
    """
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
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
            self.params = [] #set()
    
        def visitParameter(self, parameter):
            self.params.append(parameter) #add(parameter)
            
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
            
    p = None
    m = None
    doc = None
    
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
            p.descriptions.append(package.EnumDescription(opt.ident,
                                                          str(opt.ident)))
        return p
            
    def visitAll(self, visitor):
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
                
        if self.optionParam:
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
    >>> opt = package.Option("inPlace")
    >>> opt.args.extend([package.Output(arg1), package.RefInput(arg2, arg1), arg3])
    >>> m.options.append(opt)
    >>> g = OpHeaderGenerator()
    >>> g.save(p, m, True)   
    #ifndef STROMX_IMGPROC_MEDIANBLUR_H
    #define STROMX_IMGPROC_MEDIANBLUR_H
    <BLANKLINE>
    #include "stromx/imgproc/Config.h"
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
                enum OptionId
                {
                    MANUAL,
                    ALLOCATE,
                    IN_PLACE
                };
                enum ConnectorId
                {
                    SRC,
                    DST
                };
                enum ParameterId
                {
                    KSIZE,
                    DATA_FLOW
                };
                MedianBlur();
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
    class ConnectorEnumVisitor(SingleArgumentVisitor):
        def __init__(self):
            self.connectors = set()
    
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
    
    def generate(self):
        self.__includeGuardEnter()
        self.__includes()
        self.namespaceEnter()
        self.__classEnter()
        self.__public()
        
        self.__optionEnum()
        
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
        
        v = OpHeaderGenerator.DataMemberVisitor(self.doc)
        self.visitAll(v)
        
        self.__classExit()
        self.namespaceExit()
        self.__includeGuardExit()
        
        with file("{0}.h".format(self.m.ident.className()), "w") as f:
            f.write(self.doc.string())
    
    def __includes(self):
        self.doc.line('#include "stromx/{0}/Config.h"'.format(self.p.ident))
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
        
    def __optionEnum(self):
        optionIds = [o.ident.constant() for o in self.m.options]
        self.doc.enum("OptionId", optionIds)
        
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
    >>> p = package.Package("imgproc", 0, 0, 1)
    >>> p.name = "OpenCV image processing"
    >>> m = package.Method("medianBlur")
    >>> arg1 = package.Argument("src", "Source", cvtype.Mat(), datatype.Image())
    >>> arg2 = package.Argument("dst", "Destination", cvtype.Mat(), datatype.Image())
    >>> arg3 = package.NumericParameter("ksize", "Kernel size", cvtype.Int(), datatype.UInt32())
    >>> initIn = ("{1}->initializeImage({0}->width(), {0}->height(), "\
                  "{0}->stride(), {1}->data(), {0}->pixelType());")\
                 .format("srcCastedData", "dstCastedData")
    >>> arg2.initIn.append(initIn)    
    >>> initOut = ("{1}->initializeImage({1}->width(), {1}->height(), "\
                  "{1}->stride(), {1}->data(), {0}->pixelType());")\
                 .format("srcCastedData", "dstCastedData")
    >>> arg2.initOut.append(initOut)
    >>> opt = package.Option("manual")
    >>> opt.args.extend([package.Input(arg1, True), package.Output(arg2), arg3])
    >>> m.options.append(opt)
    >>> opt = package.Option("allocate")
    >>> opt.args.extend([package.Input(arg1), package.Allocation(arg2), arg3])
    >>> m.options.append(opt)
    >>> opt = package.Option("inPlace")
    >>> opt.args.extend([package.Output(arg1), package.RefInput(arg2, arg1), arg3])
    >>> m.options.append(opt)
    >>> g = OpImplGenerator()
    >>> g.save(p, m, True)
    #include "stromx/imgproc/MedianBlur.h"
    <BLANKLINE>
    #include <stromx/example/Image.h>
    #include <stromx/example/Matrix.h>
    #include <stromx/example/Utilities.h>
    #include <stromx/runtime/DataContainer.h>
    #include <stromx/runtime/DataProvider.h>
    #include <stromx/runtime/EnumParameter.h>
    #include <stromx/runtime/Id2DataComposite.h>
    #include <stromx/runtime/Id2DataPair.h>
    #include <stromx/runtime/NumericParameter.h>
    #include <stromx/runtime/OperatorException.h>
    #include <stromx/runtime/ReadAccess.h>
    #include <stromx/runtime/WriteAccess.h>
    #include <boost/assert.hpp>
    #include <opencv2/imgproc/imgproc.hpp>
    <BLANKLINE>
    namespace stromx
    {
        namespace imgproc
        {
            const std::string MedianBlur::PACKAGE(STROMX_IMGPROC_PACKAGE_NAME);
            const runtime::Version MedianBlur::VERSION(STROMX_IMGPROC_VERSION_MAJOR, STROMX_IMGPROC_VERSION_MINOR, STROMX_IMGPROC_VERSION_PATCH);
            const std::string MedianBlur::TYPE("MedianBlur");
    <BLANKLINE>
            MedianBlur::MedianBlur()
              : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
                m_ksize(),
                m_dataFlow()
            {
            }
    <BLANKLINE>
            const runtime::DataRef MedianBlur::getParameter(unsigned int id) const
            {
                switch(id)
                {
                case KSIZE:
                    return m_ksize;
                case DATA_FLOW:
                    return m_dataFlow;
                default:
                    throw runtime::WrongParameterId(id, *this);
                }
            }
    <BLANKLINE>
            void MedianBlur::setParameter(unsigned int id, const runtime::Data& value)
            {
                try
                {
                    switch(id)
                    {
                    case KSIZE:
                        m_ksize = runtime::data_cast<runtime::UInt32>(value);
                        break;
                    case DATA_FLOW:
                        m_dataFlow = runtime::data_cast<runtime::Enum>(value);
                        break;
                    default:
                        throw runtime::WrongParameterId(id, *this);
                    }
                }
                catch(runtime::BadCast&)
                {
                    throw runtime::WrongParameterType(parameter(id), *this);
                }
            }
    <BLANKLINE>
            const std::vector<const runtime::Parameter*> MedianBlur::setupInitParameters()
            {
                std::vector<const runtime::Parameter*> parameters;
    <BLANKLINE>
                runtime::Parameter* dataFlow = new runtime::Parameter(DATA_FLOW, runtime::DataVariant::ENUM);
                dataFlow->setAccessMode(runtime::Parameter::NONE_WRITE);
                dataFlow->setTitle("Data flow");
                parameters.push_back(dataFlow);
    <BLANKLINE>
                return parameters;
            }
    <BLANKLINE>
            const std::vector<const runtime::Parameter*> MedianBlur::setupParameters()
            {
                std::vector<const runtime::Parameter*> parameters;
    <BLANKLINE>
                switch(int(m_dataFlow))
                {
                case(MANUAL):
                    {
                        runtime::Parameter* ksize = new runtime::Parameter(KSIZE, runtime::DataVariant::UINT_32);
                        ksize->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                        ksize->setTitle("Kernel size");
                        parameters.push_back(ksize);
    <BLANKLINE>
                    }
                    break;
                case(ALLOCATE):
                    {
                        runtime::Parameter* ksize = new runtime::Parameter(KSIZE, runtime::DataVariant::UINT_32);
                        ksize->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                        ksize->setTitle("Kernel size");
                        parameters.push_back(ksize);
    <BLANKLINE>
                    }
                    break;
                case(IN_PLACE):
                    {
                        runtime::Parameter* ksize = new runtime::Parameter(KSIZE, runtime::DataVariant::UINT_32);
                        ksize->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                        ksize->setTitle("Kernel size");
                        parameters.push_back(ksize);
    <BLANKLINE>
                    }
                    break;
                }
    <BLANKLINE>
                return parameters;
            }
    <BLANKLINE>
            const std::vector<const runtime::Description*> MedianBlur::setupInputs()
            {
                std::vector<const runtime::Description*> inputs;
    <BLANKLINE>
                switch(int(m_dataFlow))
                {
                case(MANUAL):
                    {
                        runtime::Description* dst = new runtime::Description(DST, runtime::DataVariant::IMAGE);
                        dst->setTitle("Destination");
                        inputs.push_back(dst);
    <BLANKLINE>
                    }
                    break;
                case(ALLOCATE):
                    {
                        runtime::Description* dst = new runtime::Description(DST, runtime::DataVariant::IMAGE);
                        dst->setTitle("Destination");
                        inputs.push_back(dst);
    <BLANKLINE>
                    }
                    break;
                case(IN_PLACE):
                    {
                        runtime::Description* src = new runtime::Description(SRC, runtime::DataVariant::IMAGE);
                        src->setTitle("Source");
                        inputs.push_back(src);
    <BLANKLINE>
                    }
                    break;
                }
    <BLANKLINE>
                return inputs;
            }
    <BLANKLINE>
            const std::vector<const runtime::Description*> MedianBlur::setupOutputs()
            {
                std::vector<const runtime::Description*> outputs;
    <BLANKLINE>
                switch(int(m_dataFlow))
                {
                case(MANUAL):
                    {
                        runtime::Description* dst = new runtime::Description(DST, runtime::DataVariant::IMAGE);
                        dst->setTitle("Destination");
                        outputs.push_back(dst);
    <BLANKLINE>
                    }
                    break;
                case(ALLOCATE):
                    {
                        runtime::Description* dst = new runtime::Description(DST, runtime::DataVariant::IMAGE);
                        dst->setTitle("Destination");
                        outputs.push_back(dst);
    <BLANKLINE>
                    }
                    break;
                case(IN_PLACE):
                    {
                        runtime::Description* src = new runtime::Description(SRC, runtime::DataVariant::IMAGE);
                        src->setTitle("Source");
                        outputs.push_back(src);
    <BLANKLINE>
                    }
                    break;
                }
    <BLANKLINE>
                return outputs;
            }
    <BLANKLINE>
            void MedianBlur::initialize()
            {
                runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
            }
    <BLANKLINE>
            void MedianBlur::execute(runtime::DataProvider & provider)
            {
                switch(int(m_dataFlow))
                {
                case(MANUAL):
                    {
                        runtime::Id2DataPair srcInMapper(SRC);
                        runtime::Id2DataPair dstInMapper(DST);
    <BLANKLINE>
                        provider.receiveInputData(srcInMapper && dstInMapper);
    <BLANKLINE>
                        const runtime::Data* srcData = 0;
                        runtime::Data* dstData = 0;
    <BLANKLINE>
                        runtime::ReadAccess<> srcReadAccess;
                        runtime::DataContainer inContainer = dstInMapper.data();
                        runtime::WriteAccess<> writeAccess(inContainer);
                        dstData = &writeAccess();
    <BLANKLINE>
                        if(srcInMapper.data() == inContainer)
                        {
                            srcData = &writeAccess();
                        }
                        else
                        {
                            srcReadAccess = runtime::ReadAccess<>(srcInMapper.data());
                            srcData = &srcReadAccess();
                        }
    <BLANKLINE>
                        const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                        runtime::Image * dstCastedData = runtime::data_cast<runtime::Image>(dstData);
    <BLANKLINE>
                        dstCastedData->initializeImage(srcCastedData->width(), srcCastedData->height(), srcCastedData->stride(), dstCastedData->data(), srcCastedData->pixelType());
    <BLANKLINE>
                        cv::Mat srcCvData = example::getOpenCvMat(*srcCastedData);
                        cv::Mat dstCvData = example::getOpenCvMat(*dstCastedData);
                        int ksizeCvData = int(m_ksize);
    <BLANKLINE>
                        cv::medianBlur(srcCvData, dstCvData, ksizeCvData);
    <BLANKLINE>
                        runtime::DataContainer outContainer = inContainer;
                        runtime::Id2DataPair outputMapper(DST, outContainer);
    <BLANKLINE>
                        provider.sendOutputData(outputMapper);
                    }
                    break;
                case(ALLOCATE):
                    {
                        runtime::Id2DataPair srcInMapper(SRC);
    <BLANKLINE>
                        provider.receiveInputData(srcInMapper);
    <BLANKLINE>
                        const runtime::Data* srcData = 0;
    <BLANKLINE>
                        runtime::ReadAccess<> srcReadAccess;
    <BLANKLINE>
                        const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
    <BLANKLINE>
                        cv::Mat srcCvData = example::getOpenCvMat(*srcCastedData);
                        cv::Mat dstCvData;
                        int ksizeCvData = int(m_ksize);
    <BLANKLINE>
                        cv::medianBlur(srcCvData, dstCvData, ksizeCvData);
    <BLANKLINE>
                        runtime::Image* dstCastedData = new example::Image(dstCvData);
                        runtime::DataContainer outContainer = runtime::DataContainer(dstCastedData);
                        runtime::Id2DataPair outputMapper(DST, outContainer);
    <BLANKLINE>
                        dstCastedData->initializeImage(dstCastedData->width(), dstCastedData->height(), dstCastedData->stride(), dstCastedData->data(), srcCastedData->pixelType());
                        provider.sendOutputData(outputMapper);
                    }
                    break;
                case(IN_PLACE):
                    {
                        runtime::Id2DataPair srcInMapper(SRC);
    <BLANKLINE>
                        provider.receiveInputData(srcInMapper);
    <BLANKLINE>
                        runtime::Data* srcData = 0;
    <BLANKLINE>
                        runtime::DataContainer inContainer = srcInMapper.data();
                        runtime::WriteAccess<> writeAccess(inContainer);
                        srcData = &writeAccess();
    <BLANKLINE>
                        runtime::Image * srcCastedData = runtime::data_cast<runtime::Image>(srcData);
    <BLANKLINE>
                        cv::Mat srcCvData = example::getOpenCvMat(*srcCastedData);
                        cv::Mat dstCvData = srcCvData;
                        int ksizeCvData = int(m_ksize);
    <BLANKLINE>
                        cv::medianBlur(srcCvData, dstCvData, ksizeCvData);
    <BLANKLINE>
                        runtime::DataContainer outContainer = inContainer;
                        runtime::Id2DataPair outputMapper(SRC, outContainer);
    <BLANKLINE>
                        provider.sendOutputData(outputMapper);
                    }
                    break;
                }
            }
    <BLANKLINE>
        }
    }
    <BLANKLINE>
    <BLANKLINE>
    """     
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
                    
    class SetParametersVisitor(MethodGenerator.ParameterVisitor):
            
        def visitParameter(self, parameter):
            self.doc.label("case {0}".format(parameter.ident.constant()))
            self.doc.line("{0} = runtime::data_cast<{1}>(value);"\
                .format(parameter.ident.attribute(),
                        parameter.dataType.typeId()))
            self.doc.line("break;")
                
    class SetupParametersVisitor(MethodGenerator.ParameterVisitor):
        def __init__(self, doc, isInit = False):
            super(OpImplGenerator.SetupParametersVisitor, self).__init__(doc)
            self.isInit = isInit
            
        def visitParameter(self, parameter):
            l = "runtime::Parameter* {0} = new runtime::Parameter({1}, {2});"\
                .format(parameter.ident, parameter.ident.constant(),
                        parameter.dataType.variant())
            self.doc.line(l)
            self.__accessMode(parameter)
            l = '{0}->setTitle("{1}");'\
                .format(parameter.ident, parameter.name)
            self.doc.line(l)
            l = "parameters.push_back({0});".format(parameter.ident)
            self.doc.line(l)
            self.doc.blank()
            
        def __accessMode(self, parameter):
            if self.isInit:
                accessMode = "NONE_WRITE"
            else:
                accessMode = "ACTIVATED_WRITE"
                
            l = "{0}->setAccessMode(runtime::Parameter::{1});"\
                .format(parameter.ident, accessMode)
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
        
        def visitAllocation(self, allocation):
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
            if self.output == None:
                return
                
            if len(self.inputs) == 0:
                return
                
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
            for l in output.initIn:
                self.doc.line(l)
            
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
            
        def visitEnumParameter(self, numericParameter):
            self.visitParameter(numericParameter)
            
        def visitConstant(self, constant):
            cvData = "{0} {1}CvData".format(constant.cvType.typeId(), 
                                            constant.ident)
            castedData = constant.cvType.cast(constant.value)
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
            
        def visitConstant(self, refInput):
            self.visit(refInput)
            
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
            for l in allocation.initOut:
                self.doc.line(l)
        
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
        self.namespaceExit()
        
        with file("{0}.cpp".format(self.m.ident.className()), "w") as f:
            f.write(self.doc.string())
    
    def __includes(self):
        self.doc.line('#include "stromx/{0}/{1}.h"'\
            .format(self.p.ident, self.m.ident.className()))
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
        
def generateMethodFiles(package, method):
    g = OpHeaderGenerator()
    g.save(package, method)
    
    g = OpImplGenerator()
    g.save(package, method)
        
if __name__ == "__main__":
    import doctest
    doctest.testmod()     
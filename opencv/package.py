# -*- coding: utf-8 -*-

import datatype
import cvtype

def listIterator(l):
    isEnd = [i == len(l) - 1 for i in range(len(l))]
    return zip(isEnd, l)

class Ident(object):
    def __init__(self, ident = ""):
        self.ident = ident
        
    def className(self):
        return "{0}{1}".format(self.ident[0].upper(), self.ident[1:])
        
    def constant(self):
        """
        >>> Ident("srcPos").constant()
        'SRC_POS'
        >>> Ident("SrcPosF").constant()
        'SRC_POS_F'
        >>> Ident("SrcPos1").constant()
        'SRC_POS_1'
        """
        result = ""
        for c in self.ident:
            if c.isupper() or c.isdigit():
                result += '_'
            result += c.upper()
        return result.strip("_")
        
    def method(self):
        return self.ident
        
    def attribute(self):
        return "m_{0}".format(self.method())
        
    def upper(self):
        return self.ident.upper()
        
    def lower(self):
        return self.ident.lower()
        
    def __str__(self):
        return self.ident
        
    
class EnumImpl:
    label = ""
    lines = []
    
class Format:
    INCREASE_INDENT = 0
    DECREASE_INDENT = 1
    SCOPE_ENTER = 2
    SCOPE_EXIT = 3
    
class Package(object):
    
    def __init__(self, ident, major, minor, patch):
        self.ident = Ident(ident)
        self.name = ""
        self.description = ""
        self.methods = []
        self.major = major
        self.minor = minor
        self.patch = patch
        
class MethodFragment(object):
    def inputId(self):
        return []
        
    def outputId(self):
        return []
        
    def paramId(self):
        return []
        
    def enumIds(self):
        return []
        
    def paramDecl(self):
        return []
        
    def paramInit(self):
        return []
        
    def paramGet(self):
        return []
        
    def paramSet(self):
        return []
        
    def paramCreate(self):
        return []
        
    def initParamCreate(self):
        return []
        
    def inputCreate(self):
        return []
        
    def outputCreate(self):
        return []  
        
    def readAccess(self):
        return []
        
    def writeAccess(self):
        return []
        
    def castedData(self):
        return []
        
    def allocate(self):
        return []
        
    def cvData(self):
        return []
    
    def arg(self):
        return ["{0}CvData".format(self.ident)]
        
    def outContainer(self):
        return []
        
    def outputInit(self):
        return []

class Argument(MethodFragment):
    
    def __init__(self, ident, name, cvType, dataType):
        self.ident = Ident(ident)
        self.name = name
        self.cvType = cvType
        self.dataType = dataType
        self.description = ""
        self.rules = []
            
    def copyFrom(self, arg):
        self.ident = arg.ident
        self.name = arg.name
        self.description = arg.description
        self.cvType = arg.cvType
        self.dataType = arg.dataType
        self.rules = arg.rules
        
    def accept(self, visitor):
        raise NotImplementedError
    
class InputArgument(Argument):
    pass

class OutputArgument(Argument):
    def __init__(self, ident, name, cvType, dataType):
        super(OutputArgument, self).__init__(ident, name, cvType, dataType)
        self.initIn = []
        self.initOut = []
    
    def copyFrom(self, arg):
        super(OutputArgument, self).copyFrom(arg)
        if isinstance(arg, OutputArgument):
            self.initIn = arg.initIn
            self.initOut = arg.initOut
    
    def __repr__(self):
        return "OutputArgument()"
        
    def outputCreate(self):
        lines = []
        lines.append('runtime::Description* result = new runtime::Description(RESULT, '
                     '{0});'.format(self.dataType.variant()))
        lines.append('result->setTitle("Result");')
        lines.append("outputs.push_back(result);".format(self.ident))
        return [lines]
        
class Parameter(InputArgument):
    def __init__(self, ident, name, cvType, dataType):
        super(Parameter, self).__init__(ident, name, cvType, dataType)
        self.inPlace = False
        self.isInit = False
        self.default = None
    
    def accept(self, visitor):
        visitor.visitParameter(self)
    
    def paramId(self):
        return [self.ident.constant()]
        
    def paramDecl(self):
        decl = "{0} {1}".format(self.dataType.ident(),
                                self.ident.attribute())
        return [decl]
        
    def paramInit(self):
        return ["{0}({1})".format(self.ident.attribute(),
                                  self.default)]
                                  
    def paramGet(self):
        impl = EnumImpl()
        impl.label = self.ident.constant()
        lines = []
        lines.append("return {0};".format(self.ident.attribute()))
        impl.lines = lines
        return [impl]
        
    def paramSet(self):
        impl = EnumImpl()
        impl.label = self.ident.constant()
        lines = []
        lines.append("{0} = runtime::data_cast<{1}>(value);"\
            .format(self.ident.attribute(),
                    self.dataType.ident()))
        lines.append("break;")
        impl.lines = lines
        return [impl]      
        
    def paramCreate(self):
        lines = []
        lines.append("runtime::Parameter* {0} = new runtime::Parameter({1}, {2});"\
            .format(self.ident,
                    self.ident.constant(),
                    self.dataType.variant()))
        lines.append('{0}->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);'\
            .format(self.ident))
        lines.append('{0}->setTitle("{1}");'.format(self.ident, self.name))
        lines.append("parameters.push_back({0});".format(self.ident))
        return [lines]
        
    def cvData(self):
        cast = "{0} {1}CvData = {2};"\
            .format(self.cvType.ident(),
                    self.ident,
                    self.cvType.cast(self.ident.attribute()))
        return [cast]
        
class Size(Parameter):
    
    def accept(self, visitor):
        visitor.visitSize(self)
                               
    def paramId(self):
        return self.x.paramId() + self.y.paramId()
                           
    def paramDecl(self):
        return self.x.paramDecl() + self.y.paramDecl()
        
    def paramInit(self):
        return self.x.paramInit() + self.y.paramInit()
                           
    def paramGet(self):
        return self.x.paramGet() + self.y.paramGet()
                           
    def paramSet(self):
        return self.x.paramSet() + self.y.paramSet()
        
    def paramCreate(self):
        return self.x.paramCreate() + self.y.paramCreate()
        
    def cvData(self):
        l = "cv::Size {0}CvData({1}, {2});".format(self.ident,
                                                   self.x.ident.attribute(),
                                                   self.y.ident.attribute())
        return [l]

class NumericParameter(Parameter):
    def __init__(self, ident, name, cvType, dataType):
        super(NumericParameter, self).__init__(ident, name, cvType, dataType)
        self.minValue = None
        self.maxValue = None
        self.step = None
    
    def accept(self, visitor):
        visitor.visitNumericParameter(self)
    
class EnumParameter(Parameter):
    
    def __init__(self, ident, name):
        super(EnumParameter, self).__init__(ident, name, cvtype.Int(),
                                            datatype.Enum())
        self.descriptions = []
    
    def accept(self, visitor):
        visitor.visitEnumParameter(self)
    
    def enumIds(self):
        values = [str(d.ident) for d in self.descriptions]
        return [("{0}Id".format(self.ident.className()), values)]
        
    def paramCreate(self):
        return self.paramCreateWithAccessMode("ACTIVATED_WRITE")
        
    def paramCreateWithAccessMode(self, accessMode):
        lines = []
        lines.append("runtime::EnumParameter* {0} = new runtime::EnumParameter({1});"\
            .format(self.ident,
                    self.ident.constant(),
                    self.dataType.ident()))
        lines.append('{0}->setAccessMode(runtime::Parameter::{1});'\
            .format(self.ident, accessMode))
        lines.append('{0}->setTitle("{1}");'.format(self.ident, self.name))
        for d in self.descriptions:
            lines.append('{0}->add({1});'.format(self.ident, d.constructor()))
        lines.append("parameters.push_back({0});".format(self.ident))
        return [lines]
    
class Options(EnumParameter):
    MANUAL = 0
    ALLOCATE = 1
    IN_PLACE = 2
    TEST_1 = 3
    
    def __init__(self, options = dict()):
        super(Options, self).__init__("dataFlow", "Data Flow")
        
        if len(options) > 0:
            assert(options.has_key(Options.MANUAL))
        
        self.options = options
        self.default = Options.MANUAL
        
        if self.trivial:
            return
            
        if self.options.has_key(Options.MANUAL):
            self.descriptions.append(EnumDescription("MANUAL", "Manual"))
        
        if self.options.has_key(Options.IN_PLACE):
            self.descriptions.append(EnumDescription("IN_PLACE", "In place"))
        
        if self.options.has_key(Options.ALLOCATE):
            self.descriptions.append(EnumDescription("ALLOCATE", "Allocate"))
        
        if self.options.has_key(Options.TEST_1):
            self.descriptions.append(EnumDescription("TEST_1", "Test 1"))
        
    @property
    def trivial(self):
        return len(self.options) <= 1
    
    def enumConstant(self, option):
        if option == Options.MANUAL:
            return "MANUAL"
        elif option == Options.ALLOCATE:
            return "ALLOCATE"
        elif option == Options.IN_PLACE:
            return "IN_PLACE"
        elif option == Options.TEST_1:
            return "TEST_1"
        else:
            assert(False)
            
    def paramCreate(self):
        return []
        
    def initParamCreate(self):
        if self.trivial:
            return []
            
        return self.paramCreateWithAccessMode("NONE_WRITE")
        
    def cvData(self):
        return []
        
    def arg(self):
        return []
        
class Method(object):
    
    def __init__(self, ident):
        self.ident = Ident(ident)
        self.name = ""
        self.doc = ""
        self.functions = []
        self.options = []
        
    def call(self, option):
        if len(self.customCall):
            return self.customCall
        
        args = ""
        for isEnd, arg in listIterator(self.options[option]):
            args += "{0}CvData".format(arg.ident)
            if not isEnd:
                args += ", "
        l = "cv::{0}({1});".format(self.ident, args)
        return [l]
        
class Option(object):
    def __init__(self, ident):
        self.ident = Ident(ident)
        self.args = []

class Constant(Argument):
    
    def __init__(self, ident, cvType, value):
        self.cvType = cvType
        self.value = value
        self.ident = ident
    
    def accept(self, visitor):
        visitor.visitConstant(self)
        
    def cvData(self):
        l = "{0} {1}CvData = {2};".format(self.cvType.ident(),
                                         self.ident,
                                         self.value)
                                         
        return [l]
    
class Input(InputArgument):
            
    def __init__(self, arg, inPlace = False):
        if arg:
            self.copyFrom(arg)
        self.inPlace = inPlace
    
    def accept(self, visitor):
        visitor.visitInput(self)
        
    def inputId(self):
        return [self.ident.constant()]
        
    def inputCreate(self):
        lines = []
        lines.append("runtime::Description* {0} = new runtime::Description({1}, {2});"\
            .format(self.ident,
                    self.ident.constant(),
                    self.dataType.variant()))
        lines.append('{0}->setTitle("{1}");'.format(self.ident, self.name))
        lines.append("inputs.push_back({0});".format(self.ident))
        return [lines]
        
    def readAccess(self):
        return [self.ident]
        
    def castedData(self):
        cast = "const {0}* {1}CastedData = runtime::data_cast<{0}>({1}Data);"\
            .format(self.dataType.ident(), self.ident)
        return [cast]
        
    def cvData(self):
        src = "*{0}CastedData".format(self.ident)
        cast = "{0} {1}CvData = {2};"\
            .format(self.cvType.ident(),
                    self.ident,
                    self.cvType.cast(src))
        return [cast]

class Output(OutputArgument):
    def __init__(self, arg):
        if arg:
            self.copyFrom(arg)
    
    def accept(self, visitor):
        visitor.visitOutput(self)
        
    def inputId(self):
        return [self.ident.constant()]
        
    def outputId(self):
        return ["RESULT"] 
        
    def inputCreate(self):
        lines = []
        lines.append("runtime::Description* {0} = new runtime::Description({1}, {2});"\
            .format(self.ident,
                    self.ident.constant(),
                    self.dataType.variant()))
        lines.append('{0}->setTitle("{1}");'.format(self.ident, self.name))
        lines.append("inputs.push_back({0});".format(self.ident))
        return [lines]
        
    def writeAccess(self):
        return [self.ident]
        
    def castedData(self):
        cast = "{0}* {1}CastedData = runtime::data_cast<{0}>({1}Data);"\
            .format(self.dataType.ident(), self.ident)
        return [cast]
        
    def cvData(self):
        src = "*{0}CastedData".format(self.ident)
        cast = "{0} {1}CvData = {2};"\
            .format(self.cvType.ident(),
                    self.ident,
                    self.cvType.cast(src))
        return [cast]
    
    def outContainer(self):
        return ["runtime::DataContainer outContainer = inContainer;"]
    
    def outputInit(self):
        return self.initIn
        
class RefInput(OutputArgument):
    def __init__(self, arg, refArg):
        self.copyFrom(arg)
        self.refArg = refArg
        
    def cvData(self):
        lines = []
        l = "{0} {1}CvData = {2}CvData;".format(self.cvType.ident(),
                                                self.ident,
                                                self.refArg.ident)
        lines.append(l)
        return lines
        
    def outputCreate(self):
        return []
        
class Allocation(OutputArgument):
    def __init__(self, arg):
        self.copyFrom(arg)
    
    def accept(self, visitor):
        visitor.visitAllocation(self)
        
    def outputId(self):
        return ["RESULT"] 
    
    def cvData(self):
        lines = []
        lines.append("{0} {1}CvData;".format(self.cvType.ident(), self.ident))
        return lines
        
    def outContainer(self):
        lines = []
        cvData = "{0}CvData".format(self.ident)
        l = "{0}* {2}CastedData = new {1};".format(self.dataType.ident(),
                                                   self.dataType.cast(cvData),
                                                   self.ident)
        lines.append(l)
        l = ("runtime::DataContainer outContainer = "
             "runtime::DataContainer({0}CastedData);".format(self.ident))
        lines.append(l)
        lines.extend(self.initOut)
        return lines

class EnumDescription(object):
    
    def __init__(self, ident, name, cvIdent = None):
        self.ident = ident
        self.name = name
        if cvIdent:
            self.cvIdent = cvIdent
        else:
            self.cvIdent = ident
    
    def constructor(self):
        return ('runtime::EnumDescription({0}, "{1}")'\
            .format(self.ident, self.name))
        
    
class ParameterRule(object):
    def check(self):
        return ""

class OddRule(ParameterRule):
    pass

class MinRule(ParameterRule):
    pass

class MaxRule(ParameterRule):
    pass

if __name__ == "__main__":
    import doctest
    doctest.testmod()
    
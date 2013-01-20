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
    ident = ""
    name = ""
    description = ""
    methods = []
    major = 0
    minor = 0
    patch = 0
    
    def __init__(self, major, minor, patch):
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
    ident = ""
    name = ""
    description = ""
    cvType = cvtype.CvType()
    dataType = datatype.DataType()
    rules = []
    initIn = []
    initOut = []
    
    def __init__(self, ident, name, cvType, dataType, inPlace = False):
        self.ident = Ident(ident)
        self.name = name
        self.cvType = cvType
        self.dataType = dataType
        self.inPlace = inPlace
            
    def copyFrom(self, arg):
        self.ident = arg.ident
        self.name = arg.name
        self.description = arg.description
        self.cvType = arg.cvType
        self.dataType = arg.dataType
        self.rules = arg.rules
        self.initIn = arg.initIn
        self.initOut = arg.initOut
        self.inPlace = arg.inPlace
    
class InputArgument(Argument):
    def __repr__(self):
        return "InputArgument()"

class OutputArgument(Argument):
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
    default = None
    
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
        
class CvSize(Parameter):
    def __init__(self, ident, name):
        self.ident = Ident(ident)
        self.name = name
        self.x = Parameter("{0}X".format(ident), "{0} X".format(name),
                           cvtype.Int(), datatype.UInt32())
        self.y = Parameter("{0}Y".format(ident), "{0} Y".format(name),
                           cvtype.Int(), datatype.UInt32())
                           
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
    minValue = None
    maxValue = None
    
class EnumParameter(Parameter):
    dataType = datatype.Enum()
    descriptions = [] 
    
    def enumIds(self):
        values = [d.ident.constant() for d in self.descriptions]
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
        if len(options) > 0:
            assert(options.has_key(Options.MANUAL))
        
        self.ident = Ident("dataFlow")
        self.name = "Data Flow"
        self.options = options
        self.default = Options.MANUAL
        
        if self.trivial:
            return
            
        if self.options.has_key(Options.MANUAL):
            self.descriptions.append(EnumDescription("manual", "Manual"))
        
        if self.options.has_key(Options.IN_PLACE):
            self.descriptions.append(EnumDescription("inPlace", "In place"))
        
        if self.options.has_key(Options.ALLOCATE):
            self.descriptions.append(EnumDescription("allocate", "Allocate"))
        
        if self.options.has_key(Options.TEST_1):
            self.descriptions.append(EnumDescription("test1", "Test 1"))
        
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
    ident = ""
    name = ""
    description = ""
    customCall = []
    __options = Options()
    
    def __init__(self, ident, name, options):
        self.ident = Ident(ident)
        self.name = name
        self.options = options
    
    @property
    def options(self):
        return self.__options.options
        
    @options.setter
    def options(self, options):
        self.__options = Options(options)
        
    @property
    def optionParameter(self):
        return self.__options
        
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

class Constant(Argument):
    value = ""
    
    def __init__(self, ident, cvType, value):
        self.cvType = cvType
        self.value = value
        self.ident = ident
        
    def cvData(self):
        l = "{0} {1}CvData = {2};".format(self.cvType.ident(),
                                         self.ident,
                                         self.value)
                                         
        return [l]
    
class Input(InputArgument):
    def __init__(self, arg = None):
        if arg:
            self.copyFrom(arg)
        
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
    ident = Ident()
    name = ""
    cvIdent = ""
    
    def __init__(self, ident, name):
        self.ident = Ident(ident)
        self.name = name
    
    def constructor(self):
        return ('runtime::EnumDescription({0}, "{1}")'\
            .format(self.ident.constant(), self.name))
        
    
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
    
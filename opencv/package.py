# -*- coding: utf-8 -*-

import datatype

class CvType:
    UNDEFINED = 0
    MAT = 1
    INT = 2
    
class DataType:
    UNDEFINED = 0
    IMAGE = 1
    UINT_32 = 2
    BOOL = 3
    ENUM = 4
    
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
    
class Names:
    @staticmethod
    def className(s):
        return "{0}{1}".format(s[0].upper(), s[1:])
        
    @staticmethod
    def constantName(s):
        """
        >>> Names.constantName("srcPos")
        'SRC_POS'
        >>> Names.constantName("SrcPosF")
        'SRC_POS_F'
        >>> Names.constantName("SrcPos1")
        'SRC_POS_1'
        """
        result = ""
        for c in s:
            if c.isupper() or c.isdigit():
                result += '_'
            result += c.upper()
        return result.strip("_")
        
    @staticmethod
    def methodName(s):
        return s
        
    @staticmethod
    def attributeName(s):
        return "m_{0}".format(Names.methodName(s))
        
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
    cvType = CvType.UNDEFINED
    dataType = DataType.UNDEFINED
    rules = []
    
    def __init__(self, arg = None):
        if arg:
            self.ident = arg.ident
            self.name = arg.name
            self.description = arg.description
            self.cvType = arg.cvType
            self.dataType = arg.dataType
            self.rules = arg.rules
    
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
        return [Names.constantName(self.ident)]
        
    def paramDecl(self):
        decl = "{0} {1}".format(self.dataType.ident(),
                                 Names.attributeName(self.ident))
        return [decl]
        
    def paramInit(self):
        return ["{0}({1})".format(Names.attributeName(self.ident),
                                  self.default)]
                                  
    def paramGet(self):
        impl = EnumImpl()
        impl.label = Names.constantName(self.ident)
        lines = []
        lines.append("return {0};".format(Names.attributeName(self.ident)))
        impl.lines = lines
        return [impl]
        
    def paramSet(self):
        impl = EnumImpl()
        impl.label = Names.constantName(self.ident)
        lines = []
        lines.append("{0} = runtime::data_cast<{1}>(value);"\
            .format(Names.attributeName(self.ident),
                    self.dataType.ident()))
        lines.append("break;")
        impl.lines = lines
        return [impl]      
        
    def paramCreate(self):
        lines = []
        lines.append("runtime::Parameter* {0} = new runtime::Parameter({1}, {2});"\
            .format(self.ident,
                    Names.constantName(self.ident),
                    self.dataType.variant()))
        lines.append('{0}->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);'\
            .format(self.ident))
        lines.append('{0}->setTitle("{1}");'.format(self.ident, self.name))
        lines.append("parameters.push_back({0});".format(self.ident))
        return [lines]
        
    def cvData(self):
        cast = "{0} {1}CvData = {2}({3});"\
            .format(self.cvType.ident(),
                    self.ident,
                    self.cvType.cast(),
                    Names.attributeName(self.ident))
        return [cast]

class NumericParameter(Parameter):
    minValue = None
    maxValue = None
    
class EnumParameter(Parameter):
    dataType = datatype.Enum()
    descriptions = [] 
    
    def enumIds(self):
        values = [Names.constantName(d.ident) for d in self.descriptions]
        return [("{0}Id".format(Names.className(self.ident)), values)]
        
    def paramCreate(self):
        return self.paramCreateWithAccessMode("ACTIVATED_WRITE")
        
    def paramCreateWithAccessMode(self, accessMode):
        lines = []
        lines.append("runtime::EnumParameter* {0} = new runtime::EnumParameter({1});"\
            .format(self.ident,
                    Names.constantName(self.ident),
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
        
        self.ident = "dataFlow"
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
    args = []
    __options = Options()
    
    @property
    def options(self):
        return self.__options.options
        
    @options.setter
    def options(self, options):
        self.__options = Options(options)
        
    @property
    def optionParameter(self):
        return self.__options

class Constant(InputArgument):
    value = ""
    
class Input(InputArgument):
    def inputId(self):
        return [Names.constantName(self.ident)]
        
    def inputCreate(self):
        lines = []
        lines.append("runtime::Description* {0} = new runtime::Description({1}, {2});"\
            .format(self.ident,
                    Names.constantName(self.ident),
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
        cast = "{0} {2}CvData = {1}(*{2}CastedData);"\
            .format(self.cvType.ident(),
                    self.cvType.cast(),
                    self.ident)
        return [cast]

class Output(OutputArgument):
    def __init__(self, arg, refArg = None):
        super(Output, self).__init__(arg)
        self.refArg = refArg
        
    def inputId(self):
        return [Names.constantName(self.ident)]
        
    def outputId(self):
        return ["RESULT"] 
        
    def inputCreate(self):
        lines = []
        lines.append("runtime::Description* {0} = new runtime::Description({1}, {2});"\
            .format(self.ident,
                    Names.constantName(self.ident),
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
        cast = "{0} {2}CvData = {1}(*{2}CastedData);"\
            .format(self.cvType.ident(),
                    self.cvType.cast(),
                    self.ident)
        return [cast]
    
    def outContainer(self):
        return ["runtime::DataContainer outContainer = inContainer;"]
    
    def outputInit(self):
        if self.refArg == None:
            return []
        
        src = "{0}CastedData".format(self.refArg.ident)
        dst = "{0}CastedData".format(self.ident)
        
        lines = []
        lines.append("if({0} != {1})".format(src, dst))
        lines.append(Format.SCOPE_ENTER)
        lines.append("{0}->initializeImage({1}->width(), {1}->height(), "
                     "{1}->width() * {1}->pixelSize(), {0}->buffer(), "
                     "{1}->pixelType());".format(dst, src))
        lines.append(Format.SCOPE_EXIT)
        
        return lines
        
class RefInput(OutputArgument):
    pass
        
    def outputCreate(self):
        return []
        
class Allocation(OutputArgument):
    def __init__(self, arg, refArg = None):
        super(Allocation, self).__init__(arg)
        self.refArg = refArg
        
    def outputId(self):
        return ["RESULT"] 
    
    def cvData(self):
        lines = []
        if self.refArg:
            src = "{0}CastedData".format(self.refArg.ident)
            
            lines.append("{0}* outData = new {1};"\
                .format(self.dataType.ident(),
                        self.dataType.allocate(src)))
            lines.append("runtime::DataContainer outContainer = "
                         "runtime::DataContainer(outData);")
            lines.append("{0} {1}CvData = {2}(*outData);"\
                .format(self.cvType.ident(),
                        self.ident,
                        self.cvType.cast()))
        else:
            lines.append("{0} {1}CvData;"\
                .format(self.cvType.ident(), self.ident))
        return lines
        
    def outContainer(self):
        lines = []
        if not self.refArg:
            lines.append("{0}* outData = new {1}({2}CvData);"\
                .format(self.dataType.ident(),
                        self.dataType.cast(),
                        self.ident))
            lines.append("runtime::DataContainer outContainer = runtime::DataContainer(outData);")
        return lines

class EnumDescription(object):
    ident = ""
    name = ""
    cvIdent = ""
    
    def __init__(self, ident, name):
        self.ident = ident
        self.name = name
    
    def constructor(self):
        return ('runtime::EnumDescription({0}, "{1}")'\
            .format(Names.constantName(self.ident), self.name))
        
    
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
    
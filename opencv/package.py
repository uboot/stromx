# -*- coding: utf-8 -*-

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
    
    @staticmethod
    def listIterator(l):
        isEnd = [i == len(l) - 1 for i in range(len(l))]
        return zip(isEnd, l)
        
        
class Types:
    @staticmethod
    def dataType(t):
        if t == DataType.BOOL:
            return "core::Bool"
        elif t == DataType.UINT_32:
            return "core::UInt32"
        elif t == DataType.IMAGE:
            return "core::Image"
        elif t == DataType.ENUM:
            return "core::Enum"
        else:
            assert(False)
    
    @staticmethod
    def dataVariant(t):
        if t == DataType.BOOL:
            return "core::DataVariant::BOOL"
        elif t == DataType.UINT_32:
            return "core::DataVariant::UINT_32"
        elif t == DataType.IMAGE:
            return "core::DataVariant::IMAGE"
        elif t == DataType.IMAGE:
            return "core::DataVariant::ENUM"
        else:
            assert(False)
            
    @staticmethod
    def cvType(t):
        if t == DataType.BOOL:
            return "bool"
        elif t == DataType.UINT_32:
            return "unsigned int"
        elif t == DataType.IMAGE:
            return "cv::Mat"
        else:
            assert(False)
            
    @staticmethod
    def cvCast(t):
        if t == DataType.BOOL:
            return "bool"
        elif t == DataType.UINT_32:
            return "(unsigned int)"
        elif t == DataType.IMAGE:
            return "getOpenCvMat"
        else:
            assert(False)
    
class Method(object):
    ident = ""
    name = ""
    description = ""
    args = []
    initOptions = None
        
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
        
class Parameter(InputArgument):
    default = None
    
    def paramId(self):
        return [Names.constantName(self.ident)]
        
    def paramDecl(self):
        decl = "{0} {1}".format(Types.dataType(self.dataType),
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
        lines.append("{0} = core::data_cast<{1}>(value);"\
            .format(Names.attributeName(self.ident),
                    Types.dataType(self.dataType)))
        lines.append("break;")
        impl.lines = lines
        return [impl]      
        
    def paramCreate(self):
        lines = []
        lines.append("Parameter* {0} = new Parameter({1}, {2});"\
            .format(self.ident,
                    Names.constantName(self.ident),
                    Types.dataType(self.dataType)))
        lines.append('{0}->setAccessMode(core::Parameter::ACTIVATED_WRITE);'\
            .format(self.ident))
        lines.append('{0}->setDoc("{1}");'.format(self.ident, self.name))
        lines.append("parameters.push_back({0});".format(self.ident))
        return [lines]
        
    def cvData(self):
        cast = "{0} {1}CvData = {2}({3});"\
            .format(Types.cvType(self.cvType),
                    self.ident,
                    Types.cvCast(self.cvType),
                    Names.attributeName(self.ident))
        return [cast]

class NumericParameter(Parameter):
    minValue = None
    maxValue = None
    
class EnumParameter(Parameter):
    dataType = DataType.ENUM
    descriptions = [] 
    
    def enumIds(self):
        values = [Names.constantName(d.ident) for d in self.descriptions]
        return [("{0}Id".format(Names.className(self.ident)), values)]
        
    def paramCreate(self):
        lines = []
        lines.append("Parameter* {0} = new EnumParameter({1}, {2});"\
            .format(self.ident,
                    Names.constantName(self.ident),
                    Types.dataType(self.dataType)))
        lines.append('{0}->setAccessMode(core::Parameter::ACTIVATED_WRITE);'\
            .format(self.ident))
        lines.append('{0}->setDoc("{1}");'.format(self.ident, self.name))
        for d in self.descriptions:
            lines.append('{0}->add({0});'.format(d.constructor()))
        lines.append("parameters.push_back({0});".format(self.ident))
        return [lines]
    
    
class InitOptions(EnumParameter):
    def __init__(self, inPlace = True, allocate = True):
        self.ident = "dataFlow"
        self.name = "Data Flow"
        self.default = 0
        self.descriptions.append(EnumDescription("inPlace", "In place"))
        self.descriptions.append(EnumDescription("allocate", "Allocate"))
        self.descriptions.append(EnumDescription("manual", "Manual"))
        
    def initParamCreate(self):
        return self.paramCreate()
        
    def cvData(self):
        return []
        
    def arg(self):
        return []

class Constant(InputArgument):
    value = ""
    
class Input(InputArgument):
    def inputId(self):
        return [Names.constantName(self.ident)]
        
    def inputCreate(self):
        lines = []
        lines.append("Description* {0} = new Description({1}, {2});"\
            .format(self.ident,
                    Names.constantName(self.ident),
                    Types.dataVariant(self.dataType)))
        lines.append('{0}->setTitle("{1}");'.format(self.ident, self.name))
        lines.append("inputs.push_back({0});".format(self.ident))
        return [lines]
        
    def readAccess(self):
        return [self.ident]
        
    def castedData(self):
        cast = "const {0}* {1}CastedData = core::data_cast<{0}>({1}Data);"\
            .format(Types.dataType(self.dataType), self.ident)
        return [cast]
        
    def cvData(self):
        cast = "{0} {2}CvData = {1}(*{2}CastedData);"\
            .format(Types.cvType(self.cvType),
                    Types.cvCast(self.cvType),
                    self.ident)
        return [cast]

class Output(OutputArgument):
    def __init__(self, arg, refArg = None):
        super(Output, self).__init__(arg)
        self.refArg = refArg
        
    def outputId(self):
        return [Names.constantName(self.ident)] 
        
    def inputCreate(self):
        lines = []
        lines.append("Description* {0} = new Description({1}, {2});"\
            .format(self.ident,
                    Names.constantName(self.ident),
                    Types.dataVariant(self.dataType)))
        lines.append('{0}->setTitle("{1}");'.format(self.ident, self.name))
        lines.append("inputs.push_back({0});".format(self.ident))
        return [lines]
        
    def outputCreate(self):
        lines = []
        lines.append("Description* {0} = new Description({1}, {2});"\
            .format(self.ident,
                    Names.constantName(self.ident),
                    Types.dataVariant(self.dataType)))
        lines.append('{0}->setTitle("{1}");'.format(self.ident, self.name))
        lines.append("outputs.push_back({0});".format(self.ident))
        return [lines]
        
    def writeAccess(self):
        return [self.ident]
        
    def castedData(self):
        cast = "{0}* {1}CastedData = core::data_cast<{0}>({1}Data);"\
            .format(Types.dataType(self.dataType), self.ident)
        return [cast]
        
    def cvData(self):
        cast = "{0} {2}CvData = {1}(*{2}CastedData);"\
            .format(Types.cvType(self.cvType),
                    Types.cvCast(self.cvType),
                    self.ident)
        return [cast]
    
    def outContainer(self):
        return ["DataContainer outContainer = inContainer;"]
    
    def outputInit(self):
        if self.refArg == None:
            return []
        
        src = "{0}CastedData".format(self.refArg.ident)
        dst = "{0}CastedData".format(self.ident)
        
        lines = []
        lines.append("if({0} != {1})".format(src, dst))
        lines.append(Format.SCOPE_ENTER)
        lines.append("{0}->initializeImage({1}->width(), {1}>height(), "
                     "{1}->width() * {1}->pixelSize(), {0}->buffer(), "
                     "{1}->pixelType());".format(dst, src))
        lines.append(Format.SCOPE_EXIT)
        
        return lines
        
class RefInput(OutputArgument):
    pass
        
class Allocation(OutputArgument):
    def __init__(self, arg, refArg = None):
        super(Allocation, self).__init__(arg)
        self.refArg = refArg
    
    def allocate(self):
        src = "{0}CastedData".format(self.refArg.ident)
        
        lines = []
        lines.append("core::Image* outData = new example::Image({0}->width(), "
                     "{0}->height(), {0}->pixelType());".format(src))
        lines.append("DataContainer outContainer = DataContainer(outData);")
        lines.append("{0} {1}CvData = {2}(outData);"\
            .format(Types.cvType(self.cvType),
                    self.ident,
                    Types.cvCast(self.cvType)))
        return lines

class EnumDescription(object):
    ident = ""
    name = ""
    
    def __init__(self, ident, name):
        self.ident = ident
        self.name = name
    
    def constructor(self):
        return ('EnumDescription(Enum({0}), "{1}")'\
            .format(Names.constantName(self.ident), self.name))
        
    
class Rule(object):
    def check(self):
        return ""

class OddRule(Rule):
    pass

class MinRule(Rule):
    pass

class MaxRule(Rule):
    pass

if __name__ == "__main__":
    import doctest
    doctest.testmod()
    
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
    
class EnumImpl:
    label = ""
    lines = []
    
class Format:
    INCREASE_INDENT = 0
    DECREASE_INDENT = 1
    
class Package(object):
    ident = ""
    name = ""
    description = ""
    
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
        else:
            assert(False)
        
    
class Method(object):
    ident = ""
    name = ""
    description = ""
    args = []
        
class MethodFragment(object):
    def inputId(self):
        return []
        
    def outputId(self):
        return []
        
    def paramId(self):
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
        
    def inputMapper(self):
        return []
        
    def outputMapper(self):
        return []
        
    def receiveInput(self):
        return []
        
    def readAccess(self):
        return []
        
    def writeAccess(self):
        return []

class Argument(MethodFragment):
    ident = ""
    name = ""
    description = ""
    cvType = CvType.UNDEFINED
    dataType = DataType.UNDEFINED
    rules = []
    
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
        lines.append('{0}->setDoc("{1}");'.format(self.ident, self.name))
        lines.append("parameters.push_back({0});".format(self.ident))
        return [lines]

class NumericParameter(Parameter):
    minValue = None
    maxValue = None

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
        lines.append('{0}->setDoc("{1}");'.format(self.ident, self.name))
        lines.append("inputs.push_back({0});".format(self.ident))
        return [lines]
    
    def inputMapper(self):
        lines = []
        lines.append("core::Id2DataMapper {0}InMapper({1});"\
            .format(self.ident, Names.constantName(self.ident)))
        return lines
        
    def receiveInput(self, inPlace = False):
        inputMappers = []
        inputMappers.append("{0}InMapper".format(self.ident))
        return inputMappers
        
    def readAccess(self):
        lines = []
        lines.append("core::ReadAccess<{0}> {1}Access({1}InMapper.data());"\
            .format(Types.dataType(self.dataType),
                    self.ident))
        return lines

class Output(OutputArgument):
    inPlace = None
    
    def outputId(self):
        return [Names.constantName(self.ident)]
        
    def paramId(self):
        if not self.inPlace:
            return []
            
        return [Names.constantName("inPlace")]
            
    def paramDecl(self):
        if not self.inPlace:
            return []
            
        return ["core::Bool m_inPlace"]
            
    def paramInit(self):
        if not self.inPlace:
            return []
            
        return ["m_inPlace(false)"]
                                  
    def paramGet(self):
        if not self.inPlace:
            return []
            
        impl = EnumImpl()
        impl.label = Names.constantName(self.ident)
        lines = []
        lines.append("return m_inPlace;")
        impl.lines = lines
        return [impl]
        
    def paramSet(self):
        if not self.inPlace:
            return []
            
        impl = EnumImpl()
        impl.label = Names.constantName("inPlace")
        lines = []
        lines.append("m_inPlace = core::data_cast<core::Bool>(value);")
        lines.append("break;")
        impl.lines = lines
        return [impl]    
        
    def initParamCreate(self):
        lines = []
        lines.append("Parameter* inPlace = new Parameter({0}, {1});"\
            .format(Names.constantName("inPlace"),
                    Types.dataVariant(self.dataType)))
        lines.append('inPlace->setDoc("In place");')
        lines.append("parameters.push_back(inPlace);")
        return [lines]
        
    def inputCreate(self):
        lines = []
        if self.inPlace:
            lines.append("if(! m_inPlace)")
            lines.append("{")
            lines.append(Format.INCREASE_INDENT)
        lines.append("Description* {0} = new Description({1}, {2});"\
            .format(self.ident,
                    Names.constantName(self.ident),
                    Types.dataVariant(self.dataType)))
        lines.append('{0}->setDoc("{1}");'.format(self.ident, self.name))
        lines.append("inputs.push_back({0});".format(self.ident))
        if self.inPlace:
            lines.append(Format.DECREASE_INDENT)
            lines.append("}")
        return [lines]
        
    def outputCreate(self):
        lines = []
        lines.append("Description* {0} = new Description({1}, {2});"\
            .format(self.ident,
                    Names.constantName(self.ident),
                    Types.dataVariant(self.dataType)))
        lines.append('{0}->setDoc("{1}");'.format(self.ident, self.name))
        lines.append("outputs.push_back({0});".format(self.ident))
        return [lines]
    
    def inputMapper(self):
        lines = []
        lines.append("core::Id2DataMapper {0}InMapper({1});"\
            .format(self.ident, Names.constantName(self.ident)))
        return lines
    
    def outputMapper(self):
        lines = []
        lines.append("core::Id2DataMapper {0}OutMapper({1}, result);"\
            .format(self.ident, Names.constantName(self.ident)))
        return lines
        
    def receiveInput(self):
        inputMappers = []
        inputMappers.append("{0}InMapper".format(self.ident))
        return inputMappers
        
    def writeAccess(self):
        lines = []
        lines.append("core::Write<{0}> {1}Access({1}InMapper.data());"\
            .format(Types.dataType(self.dataType),
                    self.ident))
        return lines
        
class Allocation(OutputArgument):
    pass

class EnumDescription(object):
    ident = ""
    name = ""
    description = ""
    
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
    
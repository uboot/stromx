# -*- coding: utf-8 -*-

class CvType:
    UNDEFINED = 0
    MAT = 1
    INT = 2
    
class DataType:
    UNDEFINED = 0
    IMAGE = 1
    UINT_32 = 2
    
class EnumImpl:
    label = ""
    lines = []
    
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
        if t == DataType.UINT_32:
            return "core::UInt32"
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
            .format(Names.attributeName(self.ident), Types.dataType(self.dataType)))
        lines.append("break;")
        impl.lines = lines
        return [impl]        

class NumericParameter(Parameter):
    minValue = None
    maxValue = None

class Constant(InputArgument):
    value = ""
    
class Input(InputArgument):
    def inputId(self):
        return [Names.constantName(self.ident)]

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
    
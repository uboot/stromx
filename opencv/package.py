# -*- coding: utf-8 -*-

class CvType:
    UNDEFINED = 0
    MAT = 1
    INT = 2
    
class DataType:
    UNDEFINED = 0
    IMAGE = 1
    UINT_32 = 2
    
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
    """
    >>> args = [InputArgument(), OutputArgument(), InputArgument()]
    >>> m = Method()
    >>> m.args = args
    >>> m.inputs
    [InputArgument(), InputArgument()]
    >>> m.output
    OutputArgument()
    """
    ident = ""
    name = ""
    description = ""
    args = []
    
    @property
    def inputs(self):
        inputs =  [arg for arg in self.args if isinstance(arg, InputArgument)]
        assert(len(inputs) > 0)
        return inputs

    @property
    def output(self):
        outputs = [arg for arg in self.args if isinstance(arg, OutputArgument)]
        assert(len(outputs) == 1)
        return outputs [0]
        
class MethodFragment(object):
    def inputId(self):
        return []
        
    def outputId(self):
        return []
        
    def paramId(self):
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
        if self.inPlace:
            return [Names.constantName("inPlace")]
        else:
            return []
    
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
    
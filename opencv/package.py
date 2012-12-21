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
    def initializeParameter(self):
        return ""
    
    def activateParameter(self):
        return ""
    
    def inputConnector(self):
        return ""
        
    def getParameter(self):
        return ""
    
    def setParameter(self):
        return ""
        
    def inputMapper(self):
        return ""
        
    def outputMapper(self):
        return ""
        
    def toCvType(self):
        return ""
        
    def toDataType(self):
        return
        
    def allocate(self):
        return

class Argument(MethodFragment):
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
    pass

class NumericParameter(Parameter):
    minValue = None
    maxValue = None

class Constant(InputArgument):
    value = ""
    
class Input(InputArgument):
    pass

class Output(OutputArgument):
    inPlace = None
    
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

if __name__ == "__main__":
    import doctest
    doctest.testmod()
    
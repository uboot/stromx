# -*- coding: utf-8 -*-

import datatype
import cvtype

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
    
class Package(object):
    def __init__(self, ident, major, minor, patch):
        self.ident = Ident(ident)
        self.name = ""
        self.description = ""
        self.methods = []
        self.major = major
        self.minor = minor
        self.patch = patch
        
class Acceptor(object):
    def accept(self, visitor):
        raise NotImplementedError
        
class Compound(object):
    def __init__(self, args):
        self.args = args
        
    def create(self):
        raise NotImplementedError
        
    def accept(self, visitor):
        for arg in self.args:
            arg.accept(visitor)
            
class Size(Compound):
    def __init__(self, xArg, yArg):
        self.args = [xArg, yArg]
        
    def create(self):
        xCvData = "{0}".format(self.args[0].ident)
        yCvData = "{0}".format(self.args[1].ident)
        return "cv::Size({0}CvData, {1}CvData)".format(xCvData, yCvData)

class Argument(Acceptor):
    def __init__(self, ident, name, cvType, dataType):
        self.ident = Ident(ident)
        self.name = name
        self.cvType = cvType
        self.dataType = dataType
        self.description = ""
        self.rules = []
        self.initIn = []
        self.initOut = []
        
    def copyFrom(self, arg):
        self.ident = arg.ident
        self.name = arg.name
        self.description = arg.description
        self.cvType = arg.cvType
        self.dataType = arg.dataType
        self.rules = arg.rules
        self.initIn = arg.initIn
        self.initOut = arg.initOut
    
class InputArgument(Argument):
    pass

class OutputArgument(Argument):
    def __init__(self, ident, name, cvType, dataType):
        super(OutputArgument, self).__init__(ident, name, cvType, dataType)
        
class Parameter(InputArgument):
    def __init__(self, ident, name, cvType, dataType):
        super(Parameter, self).__init__(ident, name, cvType, dataType)
        self.inPlace = False
        self.isInit = False
        self.default = None
    
    def accept(self, visitor):
        visitor.visitParameter(self)     

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
        
class Method(object):
    def __init__(self, ident):
        self.ident = Ident(ident)
        self.name = ""
        self.doc = ""
        self.functions = []
        self.options = []
        
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
    
class Input(InputArgument):
    def __init__(self, arg, inPlace = False):
        if arg:
            self.copyFrom(arg)
        self.inPlace = inPlace
    
    def accept(self, visitor):
        visitor.visitInput(self)

class Output(OutputArgument):
    def __init__(self, arg):
        if arg:
            self.copyFrom(arg)
    
    def accept(self, visitor):
        visitor.visitOutput(self)
        
class RefInput(OutputArgument):
    def __init__(self, arg, refArg):
        self.copyFrom(arg)
        self.refArg = refArg
        
    def accept(self, visitor):
        visitor.visitRefInput(self)
        
class Allocation(OutputArgument):
    def __init__(self, arg):
        self.copyFrom(arg)
    
    def accept(self, visitor):
        visitor.visitAllocation(self)

class EnumDescription(object):
    
    def __init__(self, ident, name, cvIdent = None):
        self.ident = ident
        self.name = name
        if cvIdent:
            self.cvIdent = cvIdent
        else:
            self.cvIdent = ident        
    
class ParameterRule(object):
    def check(self):
        return ""

class OddRule(ParameterRule):
    pass

class MinRule(ParameterRule):
    pass

class MaxRule(ParameterRule):
    pass
    
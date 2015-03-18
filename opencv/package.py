# -*- coding: utf-8 -*-

import datatype
import cvtype

class Function(object):
    """
    Function consisting of declaration and a definition. For both, declaration
    and definition a list of headers (e.g. ['"stromx/runtime/Matrix.h"', 
    '<sstream>']) can be given. The will be included in the respective files.
    """
    def __init__(self, declaration = None, dclIncludes = None,
                 definition = None, dfnIncludes = None):
        self.dclIncludes = [] if dclIncludes == None else dclIncludes
        self.declaration = declaration
        self.dfnIncludes = [] if dfnIncludes == None else dfnIncludes
        self.definition = definition
        
class Ident(object):
    """
    Identifier of an object. Provides function to retrieve the identifier in
    different formats.
    """
    def __init__(self, ident = ""):
        self.ident = ident
        
    def className(self):
        """
        >>> Ident("srcPos").className()
        'SrcPos'
        """
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
        """
        >>> Ident("srcPos").method()
        'srcPos'
        """
        return self.ident
        
    def attribute(self):
        """
        >>> Ident("srcPos").attribute()
        'm_srcPos'
        """
        return "m_{0}".format(self.method())
        
    def upper(self):
        """
        >>> Ident("srcPos").upper()
        'SRCPOS'
        """
        return self.ident.upper()
        
    def lower(self):
        """
        >>> Ident("srcPos").lower()
        'srcpos'
        """
        return self.ident.lower()
        
    def __str__(self):
        return self.ident
    
class Package(object):
    """
    Represents an OpenCV module which is exported as a stromx operator package.
    """
    def __init__(self, ident, major, minor, patch,
                 description = "", methods = None, functions = None,
                 testFiles = None):
        self.ident = Ident(ident)
        self.name = ""
        self.description = description
        self.methods = [] if methods == None else methods
        self.functions = [] if functions == None else functions
        self.major = major
        self.minor = minor
        self.patch = patch
        self.testFiles = [] if testFiles == None else testFiles
        
class Acceptor(object):
    """
    Abstract base class of all visitor acceptors.
    """
    def accept(self, visitor):
        raise NotImplementedError()
        
class Compound(Acceptor):
    """
    Base class of compound arguments, i.e. arguments which bundle several
    operator parameters into one OpenCV argument.
    """
    def __init__(self, args):
        self.args = args
        
    def accept(self, visitor):
        visitor.visitCompound(self)

class Call(Compound):
    """
    A call object passes one or more arguments to the given function call
    before inserting it into the OpenCV function.
    """
    def __init__(self, call, args):
        self.args = args
        self.call = call
        
    def create(self):
        return self.call
            
class Size(Compound):
    """
    A size object constructs an cv::Size object from two stromx parameters 
    (x and y).
    """
    def __init__(self, xArg, yArg):
        self.args = [xArg, yArg]
        
    def create(self):
        xCvData = "{0}".format(self.args[0].ident)
        yCvData = "{0}".format(self.args[1].ident)
        return "cv::Size({0}CvData, {1}CvData)".format(xCvData, yCvData)
            
class Point(Compound):
    """
    A size object constructs an cv::Point object from two stromx parameters 
    (x and y).
    """
    def __init__(self, xArg, yArg):
        self.args = [xArg, yArg]
        
    def create(self):
        xCvData = "{0}".format(self.args[0].ident)
        yCvData = "{0}".format(self.args[1].ident)
        return "cv::Point({0}CvData, {1}CvData)".format(xCvData, yCvData)
            
class Scalar(Compound):
    """
    A size object constructs an cv::Scalar object from 1-4 stromx parameters.
    """
    def __init__(self, arg0, arg1 = None, arg2 = None, arg3 = None):
        self.args = [arg0, arg1, arg2, arg3]
        
    def create(self):
        cvData = ["{0}CvData".format(arg.ident) for arg in self.args 
                  if arg != None]
        argString = ", ".join(cvData)
        
        return "cv::Scalar({0})".format(argString)
        
class ArgType(object):
    PLAIN = 0
    MATRIX = 1
    NUMERIC = 2
    ENUM = 3

class Argument(Acceptor):
    """
    Base class of all arguments which are not compount arguments. 
    """
    def __init__(self, ident, name, cvType, dataType, argType = ArgType.PLAIN,
                 description = "", initIn = None, initOut = None,
                 rules = None, minValue = None, maxValue = None, step = None,
                 descriptions = None, rows = 0, cols = 0):
        self.ident = Ident(ident)
        self.name = name
        self.cvType = cvType
        self.dataType = dataType
        self.argType = argType
        self.description = description
        self.initIn = initIn
        self.initOut = initOut
        self.rules = [] if rules == None else rules
        self.minValue = minValue
        self.maxValue = maxValue
        self.step = step
        self.descriptions = [] if descriptions == None else descriptions
        self.rows = rows
        self.cols = cols
        
    def copyFrom(self, arg):
        self.ident = arg.ident
        self.name = arg.name
        self.description = arg.description
        self.cvType = arg.cvType
        self.dataType = arg.dataType
        self.argType = arg.argType
        self.initIn = arg.initIn
        self.initOut = arg.initOut
        self.rules = arg.rules
        self.minValue = arg.minValue
        self.maxValue = arg.maxValue
        self.step = arg.step
        self.description = arg.descriptions
        self.rows = arg.rows
        self.cols = arg.cols
        
class MatrixArgument(Argument):
    """
    Input argument which is represented by a matrix argument.
    """
    def __init__(self, ident, name, cvType, dataType, rows = 0,
                 cols = 0, rules = None, initIn = None, initOut = None):
        super(MatrixArgument, self).__init__(
            ident, name, cvType, dataType, rules = rules, 
            argType = ArgType.MATRIX, rows = rows, cols = cols, 
            initIn = initIn, initOut = initOut
        )
    
class InputArgument(Argument):
    """
    Base class of all arguments which are not output arguments. 
    """
    pass

class OutputArgument(Argument):
    """
    An argument which receives the result of the OpenCV function. Only one
    output argument per option is supported.
    """
    pass
        
class Parameter(InputArgument):
    """
    Input argument which is represented by an operator parameter.
    """
    def __init__(self, ident, name, cvType, dataType, argType = ArgType.PLAIN,
                 description = "", initIn = None, initOut = None,
                 rules = None, minValue = None, maxValue = None, step = None,
                 descriptions = None, rows = 0, cols = 0, inPlace = False,
                 isInit = False, default = None):
                     
        super(Parameter, self).__init__(
            ident, name, cvType, dataType, argType = argType,
            description = description, initIn = initIn, initOut = initOut,
            rules = rules, minValue = minValue, maxValue = maxValue,
            step = step, descriptions = descriptions, rows = rows, cols = cols
        )
        
        self.inPlace = inPlace
        self.isInit = isInit
        self.default = default
    
    def accept(self, visitor):
        visitor.visitParameter(self)     

class NumericParameter(Parameter):
    """
    Input argument which is represented by a numeric operator parameter.
    """
    def __init__(self, ident, name, cvType, dataType, default = None,
                 minValue = None, maxValue = None, step = None, rules = None,
                 descriptions = None, rows = 0, cols = 0):
        default = minValue if default == None else default
        super(NumericParameter, self).__init__(
            ident, name, cvType, dataType, default = default, rules = rules,
            argType = ArgType.NUMERIC, minValue = minValue,
            maxValue = maxValue, step = step
        )
    
class EnumParameter(Parameter):
    """
    Input argument which is represented by an enumeration parameter.
    """
    def __init__(self, ident, name, descriptions = None, default = None):
        super(EnumParameter, self).__init__(
            ident, name, cvtype.Int(), datatype.Enum(), default = default, 
            descriptions = descriptions, argType = ArgType.ENUM
        )
        
class MatrixParameter(Parameter):
    """
    Input argument which is represented by a matrix parameter.
    """
    def __init__(self, ident, name, dataType, default = None, rows = 0,
                 cols = 0, rules = None):
        super(MatrixParameter, self).__init__(
            ident, name, cvtype.Mat(), dataType, default = default, 
            rules = rules, argType = ArgType.MATRIX, rows = rows, cols = cols
        )
        
class Method(object):
    """
    Represents an OpenCV method. The member 'ident' is the exact identifier
    of the method.
    """
    def __init__(self, ident, name = "", description = "", namespace = "cv",
                 functions = None, options = None):
        self.ident = Ident(ident)
        self.name = name
        self.description = description
        self.functions = [] if functions == None else functions
        self.options = [] if options == None else options
        self.namespace = namespace
        
class Option(object):
    """
    Each method can have several options. An option is one way to create the
    method arguments from operator parameters and operator inputs. E.g. there
    might be one option for an in-place variant of an operator and another one
    which accepts a separate data input which receives the result data.
    """
    def __init__(self, ident, name, args = None, tests = None, 
                 inputCheck = None, postCall = None):
        self.ident = Ident(ident)
        self.name = name
        self.args = [] if args == None else args
        self.tests = [] if tests == None else tests
        self.inputCheck = inputCheck
        self.postCall = postCall

class Constant(Argument):
    """
    Constant method argument which can not be set as input or parameter. The
    value must be fixed at compile time.
    """
    def __init__(self, value, initIn = None):
        self.initIn = initIn
        self.ident = Ident()
        self.value = value
    
    def accept(self, visitor):
        visitor.visitConstant(self)
    
class Input(InputArgument):
    """
    Operator input.
    """
    def __init__(self, arg, inPlace = False):
        if arg:
            self.copyFrom(arg)
        self.inPlace = inPlace
    
    def accept(self, visitor):
        visitor.visitInput(self)

class Output(OutputArgument):
    """
    Operator input whose data will receive the result of the method. It will be
    forwarded to the output. It represents the only input data among all other
    inputs of the operation which is changed by the method.
    """
    def __init__(self, arg, isReturnValue = False):
        if arg:
            self.copyFrom(arg)
        self.isReturnValue = isReturnValue
    
    def accept(self, visitor):
        visitor.visitOutput(self)

class InputOutput(OutputArgument):
    """
    Operator input whose data represents an input of the method and which will
    be overwritten by the output of the method. It represents the only input 
    data among all other inputs of the operation which is changed by the
    method.
    """
    def __init__(self, arg):
        if arg:
            self.copyFrom(arg)
    
    def accept(self, visitor):
        visitor.visitInputOutput(self)
        
class RefInput(InputArgument):
    """
    Virtual operator input which is not visible but receives its data from the
    referenced input.
    """
    def __init__(self, arg, refArg):
        self.copyFrom(arg)
        self.refArg = refArg
        
    def accept(self, visitor):
        visitor.visitRefInput(self)
        
class Allocation(OutputArgument):
    """
    Virtual operator input which is not visible as input but whose data is
    allocated on demand. The data then is passed to the output.
    """
    def __init__(self, arg):
        self.copyFrom(arg)
    
    def accept(self, visitor):
        visitor.visitAllocation(self)
        
class ReturnValue(Allocation):
    """
    Virtual operator output which corresponds to the return value of the OpenCV
    function. It is not visible as input but its data is allocated during
    execution. The data then is passed to the output.
    """
    def __init__(self, arg):
        self.copyFrom(arg)
    
    def accept(self, visitor):
        visitor.visitReturnValue(self)

class EnumDescription(object):
    """
    Enumeration value of an enumeration parameter. The value can hold second
    value which is passed to the OpenCV method instead of the original
    value. If no original value is given the original value prefixed with
    'cv::' is passed to the method.
    """
    def __init__(self, ident, name, cvIdent = None):
        self.ident = ident
        self.name = name
        if cvIdent:
            self.cvIdent = cvIdent
        else:
            self.cvIdent = "cv::{0}".format(ident)     
    
class DataRule(object):
    """
    Abstract base class of all rules.
    """
    def check(self, parameter, doc):
        return ""

class OddRule(DataRule):
    """
    Marks an argument as odd, i.e. it only accepts odd values.
    """
    def accept(self, visitor):
        visitor.visitOddRule(self)

class EvenRule(DataRule):
    """
    Marks an argument as even, i.e. it only accepts even values.
    """
    def accept(self, visitor):
        visitor.visitEvenRule(self)


    
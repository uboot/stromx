# -*- coding: utf-8 -*-

class TestData(object):
    """
    Abstract base class of all test data classes.
    """
    def __init__(self):
        self.arg = None
        
    def accept(self, visitor):
        raise NotImplementedError()

class Value(TestData):
    """
    Base class of test data classes. Stores a value which represents the 
    construction of the test data.
    """
    def __init__(self, *args):
        super(Value, self).__init__()
        self.value = args[0] if len(args) == 1 else args
        
    def accept(self, visitor):
        visitor.visitValue(self)
    
class List(TestData):
    """
    Image test data which is read from a given file.
    """
    def __init__(self, *args):
        self.values = args
        
    def accept(self, visitor):
        visitor.visitList(self)
    
class ImageFile(Value):
    """
    Image test data which is read from a given file.
    """
    def __init__(self, value, grayscale = False, deepColor = False):
        super(ImageFile, self).__init__(value)
        self.grayscale = grayscale
        self.deepColor = deepColor
        
    def accept(self, visitor):
        visitor.visitImageFile(self)
    
class MatrixFile(Value):
    """
    Matrix test data which is read from a given matrix file.
    """
    def __init__(self, value):
        super(MatrixFile, self).__init__(value)
        
    def accept(self, visitor):
        visitor.visitMatrixFile(self)

class ImageBuffer(Value):
    """
    Image test data which is a memory buffer of a given size (without any
    meaningful content).
    """
    def accept(self, visitor):
        visitor.visitImageBuffer(self)

class Default(TestData):
    """
    Pseudo test data which does not result in setting any parameter or input.
    Can be used to test the default values of parameters.
    """
    def accept(self, visitor):
        visitor.visitDefault(self)
    
class RefData(TestData):
    """
    Test data which is not constructed but simple references an already
    allocated data. Can be used to set two inputs to the same data 
    (e.g. for in-place processing).
    """
    def __init__(self, data):
        super(RefData, self).__init__()
        self.data = data
        
    def accept(self, visitor):
        visitor.visitRefData(self)
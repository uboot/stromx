# -*- coding: utf-8 -*-

class TestData(object):
    def __init__(self):
        self.arg = None
        
    def accept(self, visitor):
        raise NotImplementedError

class Value(TestData):
    def __init__(self, value):
        super(Value, self).__init__()
        self.value = value
        
    def accept(self, visitor):
        visitor.visitValue(self)
    
class ImageFile(Value):
    def __init__(self, value, grayscale = False, deepColor = False):
        super(ImageFile, self).__init__(value)
        self.grayscale = grayscale
        self.deepColor = deepColor
        
    def accept(self, visitor):
        visitor.visitImageFile(self)
    
class MatrixFile(Value):
    def __init__(self, value):
        super(MatrixFile, self).__init__(value)
        
    def accept(self, visitor):
        visitor.visitMatrixFile(self)

class ImageBuffer(Value):
    def accept(self, visitor):
        visitor.visitImageBuffer(self)

class Default(TestData):
    def accept(self, visitor):
        visitor.visitDefault(self)
    
class RefData(TestData):
    def __init__(self, data):
        super(RefData, self).__init__()
        self.data = data
        
    def accept(self, visitor):
        visitor.visitRefData(self)
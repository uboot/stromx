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
    def accept(self, visitor):
        visitor.visitImageFile(self)

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
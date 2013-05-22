# -*- coding: utf-8 -*-

class TestData(object):
    pass

class Value(TestData):
    def __init__(self, value):
        self.value = value
    
class ImageFile(Value):
    pass

class ImageBuffer(Value):
    pass

class Default(TestData):
    pass
    
class InPlace(TestData):
    def __init__(self, data):
        self.data = data
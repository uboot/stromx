# -*- coding: utf-8 -*-

class ArgumentVisitor(object):
    def visitInput(self, inputArg):
        pass
        
    def visitParameter(self, parameter):
        pass
        
    def visitNumericParameter(self, numericParameter):
        pass
        
    def visitConstant(self, const):
        pass
        
    def visitEnumParameter(self, enumParameter):
        pass
        
    def visitRefInput(self, refInput):
        pass
        
    def visitAllocation(self, allocation):
        pass
        
    def visitOutput(self, output):
        pass
        
    def visitCompound(self, compound):
        pass
    
class TestArgumentVisitor(object):
    def visitImageFile(self, testData):
        pass
    
    def visitImageBuffer(self, testData):
        pass
    
    def visitValue(self, testData):
        pass
    
    def visitDefault(self, testData):
        pass
    
    def visitRefData(self, testData):
        pass
    

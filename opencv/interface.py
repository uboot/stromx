# -*- coding: utf-8 -*-

class DataRuleVisitor(object):
    """
    Visitor for different data rules.
    """
    def visitOddRule(self, rule):
        pass
        
    def visitEvenRule(self, rule):
        pass
    
class ArgumentVisitor(object):
    """
    Visitor for different argument types.
    """
    def visitInput(self, inputArg):
        pass
        
    def visitParameter(self, parameter):
        pass
        
    def visitConstant(self, const):
        pass
        
    def visitRefInput(self, refInput):
        pass
        
    def visitAllocation(self, allocation):
        pass
        
    def visitOutput(self, output):
        pass
        
    def visitCompound(self, compound):
        pass
    
    def visitInputOutput(self, inputOutput):
        pass
    
    def visitReturnValue(self, retValue):
        pass
    
class TestArgumentVisitor(object):
    """
    Visitor for different test data types.
    """
    def visitList(self, testData):
        pass
        
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
    
    def visitMatrixFile(self, testData):
        pass
    

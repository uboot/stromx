# -*- coding: utf-8 -*-

import interface
import package
import test

def _isParameter(arg):
    return isinstance(arg, package.Parameter)
    
def _createTestData(arg, dataType, constructorArgs):
    if _isParameter(arg):
        return "{0} {1}({2});".format(dataType, arg.ident, constructorArgs)
    else:
        return ("runtime::DataContainer "
            "{0}(new {1}({2}));".format(arg.ident, dataType, constructorArgs)
        )

class CreateDataVisitor(interface.TestArgumentVisitor):
    def __init__(self, doc):
        self.doc = doc
        
    def visitImageFile(self, testData):
        l = _createTestData(testData.arg, "example::Image", 
                            '"{0}"'.format(testData.value))
        self.doc.line(l)
        
    def visitImageBuffer(self, testData):
        l = _createTestData(testData.arg, "example::Image", 
                            '{0}'.format(testData.value))
        self.doc.line(l)
    
    def visitValue(self, testData):
        l = _createTestData(testData.arg, testData.arg.dataType.typeId(), 
                            '{0}'.format(testData.value))
        self.doc.line(l)
    
class SetDataVisitor(interface.TestArgumentVisitor):
    def __init__(self, doc):
        self.doc = doc
        
    def __visitData(self, arg):
        if _isParameter(arg):
            l = ("m_operator->setParameter({0}, "
                "{1});".format(arg.ident.constant(), arg.ident))
            self.doc.line(l)
        else:
            l = ("m_operator->setInputData({0}, "
                "{1});".format(arg.ident.constant(), arg.ident))
            self.doc.line(l)
            
    def visitImageFile(self, testData):
        self.__visitData(testData.arg)
    
    def visitValue(self, testData):
        self.__visitData(testData.arg)
    
    def visitRefData(self, testData):
        l = ((
            "m_operator->setInputData({0}, {1});"
        ).format(testData.arg.ident.constant(), testData.data.arg.ident))
        self.doc.line(l)
    
def _visitTest(doc, args, testData, visitor):
    for arg, data in zip(args, testData):   
        if isinstance(data, test.Default):
            continue
        
        if isinstance(arg, package.Compound):
            _visitTest(doc, arg.args, data, visitor)
            continue
        
        if not isinstance(data, test.TestData):
            data = test.Value(data)
        
        data.arg = arg
        data.accept(visitor)
    
def generate(doc, args, testData):
    visitor = CreateDataVisitor(doc)
    _visitTest(doc, args, testData, visitor)
    
    doc.blank()
    
    visitor = SetDataVisitor(doc)
    _visitTest(doc, args, testData, visitor)
        
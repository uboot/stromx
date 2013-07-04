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
        flags = []
        if testData.grayscale:
            flags.append("cvsupport::Image::GRAYSCALE")

        if testData.deepColor:
            flags.append("cvsupport::Image::DEPTH_16")
          
        word = ""
        for i, flag in enumerate(flags):
            if i == len(flags) - 1:
                word += flag
            else:
                word += flag + " & "
        
        if word != "":
            l = _createTestData(testData.arg, "cvsupport::Image", 
                                '"{0}", {1}'.format(testData.value, word))
        else:
            l = _createTestData(testData.arg, "cvsupport::Image", 
                                '"{0}"'.format(testData.value))
            
                    
        self.doc.line(l)
        
    def visitMatrixFile(self, testData):
        l = _createTestData(testData.arg, "cvsupport::Matrix", 
                            '"{0}"'.format(testData.value))
                    
        self.doc.line(l)
        
    def visitImageBuffer(self, testData):
        l = _createTestData(testData.arg, "cvsupport::Image", 
                            '{0}'.format(testData.value))
        self.doc.line(l)
    
    def visitValue(self, testData):
        l = _createTestData(testData.arg, testData.arg.dataType.typeId(), 
                            '{0}'.format(testData.value))
        self.doc.line(l)
    
class SetDataVisitor(interface.TestArgumentVisitor):
    def __init__(self, doc, method):
        self.doc = doc
        self.m = method
        
    def __visitData(self, arg):
        index = "{0}::{1}".format(self.m.ident.className(),
                                  arg.ident.constant())
        if _isParameter(arg):
            l = ("m_operator->setParameter({0}, "
                "{1});".format(index, arg.ident))
            self.doc.line(l)
        else:
            l = ("m_operator->setInputData({0}, "
                "{1});".format(index, arg.ident))
            self.doc.line(l)
            
    def visitImageFile(self, testData):
        self.__visitData(testData.arg)
            
    def visitImageBuffer(self, testData):
        self.__visitData(testData.arg)
            
    def visitMatrixFile(self, testData):
        self.__visitData(testData.arg)
    
    def visitValue(self, testData):
        self.__visitData(testData.arg)
    
    def visitRefData(self, testData):
        index = "{0}::{1}".format(self.m.ident.className(),
                                  testData.arg.ident.constant())
        l = ((
            "m_operator->setInputData({0}, {1});"
        ).format(index, testData.data.arg.ident))
        self.doc.line(l)
    
class GetDataVisitor(interface.TestArgumentVisitor):
    def __init__(self, doc, method):
        self.doc = doc
        self.m = method
        
    def __visitData(self, arg):
        if not isinstance(arg, package.OutputArgument):
            return
            
        index = "{0}::{1}".format(self.m.ident.className(),
                                  arg.ident.constant())
                                  
        l = (
            "runtime::DataContainer result = m_operator->getOutputData({0});"
        ).format(index)
        self.doc.line(l)
            
    def visitImageFile(self, testData):
        self.__visitData(testData.arg)
            
    def visitImageBuffer(self, testData):
        self.__visitData(testData.arg)
    
    def visitValue(self, testData):
        self.__visitData(testData.arg)
    
    def visitDefault(self, testData):
        self.__visitData(testData.arg)
    
    def visitRefData(self, testData):
        self.__visitData(testData.arg)
        
class SaveResultVisitor(interface.ArgumentVisitor):
    def __init__(self, doc, testFileName):
        self.doc = doc
        self.testFileName = testFileName
        
    def visitAllocation(self, allocation):
        self.visitOutput(allocation)
        
    def visitOutput(self, output):
        self.doc.line("runtime::ReadAccess<runtime::Image> access(result);")
        
        fileName = "{0}.png".format(self.testFileName)
        self.doc.line((
            'cvsupport::Image::save("{0}", access());'
        ).format(fileName))
    
def _visitTest(doc, args, testData, visitor):
    for arg, data in zip(args, testData):   
        if isinstance(arg, package.Compound):
            if isinstance(data, test.Default):
                compoundData = [test.Default()] * len(arg.args)
            else:
                compoundData = data
                
            _visitTest(doc, arg.args, compoundData, visitor)
            continue
        
        if not isinstance(data, test.TestData):
            data = test.Value(data)
        
        data.arg = arg
        data.accept(visitor)
    
def generate(doc, method, args, testData, testName):
    visitor = CreateDataVisitor(doc)
    _visitTest(doc, args, testData, visitor)
    
    doc.blank()
    
    visitor = SetDataVisitor(doc, method)
    _visitTest(doc, args, testData, visitor)
    
    doc.blank()
    
    visitor = GetDataVisitor(doc, method)
    _visitTest(doc, args, testData, visitor)
    
    doc.blank()
    
    testFileName = "{0}Test_{1}".format(method.ident.className(), testName)
    visitor = SaveResultVisitor(doc, testFileName)
    for arg in args:
        arg.accept(visitor)
        
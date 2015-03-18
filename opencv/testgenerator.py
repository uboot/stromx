# -*- coding: utf-8 -*-

import datatype
import document
import interface
import package
import test

def _isParameter(arg):
    return isinstance(arg, package.Parameter)
        
class CreateDataContructorVisitor(interface.TestArgumentVisitor):
    """
    Abstract base class which creates the constructor elements for each visited
    test data object and forwards them to the abstract member create().
    """
    def __init__(self, doc):
        self.doc = doc
        
    def visitList(self, testData):
        argIdent = testData.arg.ident
        l = "runtime::Data* {0}Item = 0;".format(argIdent)
        self.doc.line(l)
        l = "std::vector<runtime::Data*> {0}Vector;".format(argIdent)
        self.doc.line(l)        
        visitor = CreateHeapObjectVisitor(testData.arg, self.doc)
        for value in testData.values:
            value.accept(visitor)
            self.doc.line("{0}Vector.push_back({0}Item);".format(argIdent))
        dataType = "runtime::List"
        args = "{0}Vector".format(argIdent)
        self.create(testData.arg, dataType, args)
        self.doc.blank()
    
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
            args = '"{0}", {1}'.format(testData.value, word)
        else:
            args = '"{0}"'.format(testData.value)
        
        dataType = "cvsupport::Image"
        self.create(testData.arg, dataType, args)
        
    def visitMatrixFile(self, testData):
        dataType = "cvsupport::Matrix"
        args = '"{0}"'.format(testData.value)
        self.create(testData.arg, dataType, args)
        
    def visitImageBuffer(self, testData):
        dataType = "cvsupport::Image"
        args = testData.value
        self.create(testData.arg, dataType, args)
        
    def visitValue(self, testData):
        dataType = testData.arg.dataType.typeId()
        args = document.pythonToCpp(testData.value)
        self.create(testData.arg, dataType, args)
        
    def create(self, arg, dataType, args):
        raise NotImplementedError()
        
class CreateHeapObjectVisitor(CreateDataContructorVisitor):
    """
    Exports the creation of a data object on the heap for each visited test data
    object.
    """        
    def __init__(self, arg, doc):
        super(CreateHeapObjectVisitor, self).__init__(doc)
        self.arg = arg
        
    def create(self, arg, dataType, args):
        l = "{2}Item = new {0}({1});".format(dataType, args, self.arg.ident)
        self.doc.line(l)
        
class CreateDataContainerVisitor(CreateDataContructorVisitor):
    """
    Exports the creation of a DataContainer for each visited test data object.
    """        
    def create(self, arg, dataType, args):
        l = "runtime::DataContainer {0}(new {1}({2}));".format(arg.ident,
                                                               dataType, args)
        self.doc.line(l)
        
class CreateStackObjectVisitor(CreateDataContructorVisitor):
    """
    Exports the creation of a data object on the stack for each visited test 
    data object.
    """
    def create(self, arg, dataType, args):
        l = "{0} {1}({2});".format(dataType, arg.ident, args)
        self.doc.line(l)
        
class CreateDataVisitor(interface.TestArgumentVisitor):
    """
    Exports the creation of either a DataContainer or a Data object for each 
    visited test data object.
    """
    def __init__(self, doc):
        self.__stackObjectVisitor = CreateStackObjectVisitor(doc)
        self.__dataContainerVisitor = CreateDataContainerVisitor(doc)
        
    def visitList(self, testData):
        self.__visitData(testData)
        
    def visitImageFile(self, testData):
        self.__visitData(testData)
        
    def visitMatrixFile(self, testData):
        self.__visitData(testData)
        
    def visitImageBuffer(self, testData):
        self.__visitData(testData)
        
    def visitValue(self, testData):
        self.__visitData(testData)
        
    def __visitData(self, testData):
        if _isParameter(testData.arg):
            testData.accept(self.__stackObjectVisitor)
        else:
            testData.accept(self.__dataContainerVisitor)
    
class SetDataVisitor(interface.TestArgumentVisitor):
    """
    Sets either a parameter or an input for each visited test data depending
    on the type of the corresponding method argument (parameter or input).
    """
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
    
    def visitList(self, testData):
        self.__visitData(testData.arg)
    
    def visitRefData(self, testData):
        index = "{0}::{1}".format(self.m.ident.className(),
                                  testData.arg.ident.constant())
        l = ((
            "m_operator->setInputData({0}, {1});"
        ).format(index, testData.data.arg.ident))
        self.doc.line(l)
    
class GetDataVisitor(interface.TestArgumentVisitor):
    """
    Exports the acquisition of the data at the operator output for each visited
    test data at an output.
    """
    def __init__(self, doc, method):
        self.doc = doc
        self.m = method
        
    def __visitData(self, arg):
        if not isinstance(arg, package.OutputArgument):
            return
            
        index = "{0}::{1}".format(self.m.ident.className(),
                                  arg.ident.constant())
                                  
        l = (
            "runtime::DataContainer {0}Result = m_operator->getOutputData({1});"
        ).format(arg.ident, index)
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
    
    def visitMatrixFile(self, testData):
        self.__visitData(testData.arg)
        
class SaveResultVisitor(interface.ArgumentVisitor):
    """
    Creates a read access for the output data and saves it as an image for each
    visited output.
    """
    def __init__(self, doc, testFileName):
        self.doc = doc
        self.testFileName = testFileName
        
    def visitAllocation(self, allocation):
        self.visitOutput(allocation)
        
    def visitInputOutput(self, arg):
        self.visitOutput(arg)
        
    def visitReturnValue(self, retValue):
        self.visitOutput(retValue)
        
    def visitOutput(self, output):
        ident = output.ident
        if isinstance(output.dataType, datatype.Image):
            self.doc.line(("runtime::ReadAccess<runtime::Image> {0}Access"
                           "({0}Result);").format(ident))
            fileName = "{0}_{1}.png".format(self.testFileName, ident)
            self.doc.line((
                'cvsupport::Image::save("{0}", {1}Access());'
            ).format(fileName, ident))
        elif isinstance(output.dataType, datatype.Matrix):
            self.doc.line(("runtime::ReadAccess<runtime::Matrix> {0}Access"
                           "({0}Result);").format(ident))
            fileName = "{0}_{1}.npy".format(self.testFileName, ident)
            self.doc.line((
                'cvsupport::Matrix::save("{0}", {1}Access());'
            ).format(fileName, ident))
        elif (isinstance(output.dataType, datatype.List) and
              isinstance(output.dataType.elementType, datatype.Matrix)):
            self.doc.line("std::size_t index = 0;")
            self.doc.line(("runtime::ReadAccess<runtime::List> {0}Access"
                           "({0}Result);").format(ident))
            self.doc.line(("const std::vector<const runtime::Data*> & content ="
                           " {0}Access().content();").format(ident))
            self.doc.line("for (std::vector<const runtime::Data*>::const_iterator "
                          "iter = content.begin(); "
                          "iter != content.end(); "
                          "++iter)")
            self.doc.scopeEnter()
            self.doc.line("const runtime::Matrix* matrix = "
                          "runtime::data_cast<runtime::Matrix>(*iter);")
            self.doc.line(('std::string fileName = "{0}_{1}_" + '
                           'boost::lexical_cast<std::string>(index) + ".npy";'
                           ).format(self.testFileName, ident))
            self.doc.line('cvsupport::Matrix::save(fileName, *matrix);')
            self.doc.line("++index;")
            self.doc.scopeExit()
    
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
    """
    Generates the operator tests for the given method and test data.
    """
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
        
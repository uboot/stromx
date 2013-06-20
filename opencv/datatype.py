# -*- coding: utf-8 -*-
"""
Created on Sat Jan 19 17:03:45 2013

@author: matz
"""

class DataType(object):
    def typeId(self):
        raise NotImplementedError()
        
    def variant(self):
        raise NotImplementedError()
        
    def cast(self, src):
        return "{0}({1})".format(self.typeId(), src)
        
class Bool(DataType):
    def typeId(self):
        return "runtime::Bool"
        
    def variant(self):
        return "runtime::DataVariant::BOOL"
        
class UInt32(DataType):
    def typeId(self):
        return "runtime::UInt32"
        
    def variant(self):
        return "runtime::DataVariant::UINT_32"
        
class Double(DataType):
    def typeId(self):
        return "runtime::Double"
        
    def variant(self):
        return "runtime::DataVariant::DOUBLE"
        
class Enum(DataType):
    def typeId(self):
        return "runtime::Enum"
        
    def variant(self):
        return "runtime::DataVariant::ENUM"
        
class Image(DataType):
    def __init__(self, variant = "runtime::DataVariant::IMAGE"):
        self.__variant = variant
        
    def typeId(self):
        return "runtime::Image"
        
    def variant(self):
        return self.__variant
        
    def cast(self, src):
        return "imgutil::Image({0})".format(src)
            
if __name__ == "__main__":
    import doctest
    doctest.testmod()
    
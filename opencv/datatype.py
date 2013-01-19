# -*- coding: utf-8 -*-
"""
Created on Sat Jan 19 17:03:45 2013

@author: matz
"""

class DataType(object):
    def ident(self):
        raise NotImplementedError()
        
    def initialize(self, ident, src = None):
        return ""
        
    def variant(self):
        raise NotImplementedError()
        
    def cast(self):
        return self.dataType()
        
class Bool(DataType):
    def ident(self):
        return "runtime::Bool"
        
    def dataVariant(self):
        return "runtime::DataVariant::BOOL"
        
class UInt32(DataType):
    def ident(self):
        return "runtime::UInt32"
        
    def variant(self):
        return "runtime::DataVariant::UINT_32"
        
class Enum(DataType):
    def ident(self):
        return "runtime::Enum"
        
    def variant(self):
        return "runtime::DataVariant::ENUM"
        
class Image(DataType):
    def ident(self):
        return "runtime::Image"
        
    def variant(self):
        return "runtime::DataVariant::IMAGE"
        
    def initialize(self, ident, src):
        return ("{0}->initializeImage({0}->width(), {0}->height(), "
                "{0}->stride(), {0}->data(), {1}->pixelType());")\
                .format(ident, src)
        
    def cast(self):
        return "example::Image"

if __name__ == "__main__":
    import doctest
    doctest.testmod()
    
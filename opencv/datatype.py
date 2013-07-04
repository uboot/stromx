# -*- coding: utf-8 -*-
"""
Created on Sat Jan 19 17:03:45 2013

@author: matz
"""

class DataType(object):
    """
    Represents a stromx::Data type.
    """
    def typeId(self):
        """
        Returns the name of the abstract type, e.g. "runtime::Image".
        """
        raise NotImplementedError()
        
    def concreteTypeId(self):
        """
        Returns the name of a concrete type, e.g. "cvsupport::Image".
        """
        return self.typeId()
        
    def variant(self):
        """
        Returns the variant of the type, e.g. "runtime::DataVariant::BOOL".
        """
        raise NotImplementedError()
        
    def cast(self, src):
        """
        Returns a conversion of the input variable to the type, e.g. 
        "cvsupport::Image(img)".
        """
        return "{0}({1})".format(self.concreteTypeId(), src)
        
class Bool(DataType):
    """
    Stromx runtime::Bool type.
    """
    def typeId(self):
        return "runtime::Bool"
        
    def variant(self):
        return "runtime::DataVariant::BOOL"
        
class UInt32(DataType):
    """
    Stromx runtime::UInt32 type.
    """
    def typeId(self):
        return "runtime::UInt32"
        
    def variant(self):
        return "runtime::DataVariant::UINT_32"
        
class Double(DataType):
    """
    Stromx runtime::Double type.
    """
    def typeId(self):
        return "runtime::Double"
        
    def variant(self):
        return "runtime::DataVariant::DOUBLE"
        
class Enum(DataType):
    """
    Stromx runtime::Enum type.
    """
    def typeId(self):
        return "runtime::Enum"
        
    def variant(self):
        return "runtime::DataVariant::ENUM"
        
class Image(DataType):
    """
    Stromx runtime::Image type. Uses the implementation cvsupport::Image to cast
    input data to an runtime::Image object.
    """
    def __init__(self, variant = "runtime::DataVariant::IMAGE"):
        self.__variant = variant
        
    def typeId(self):
        return "runtime::Image"
        
    def concreteTypeId(self):
        return "cvsupport::Image"
        
    def variant(self):
        return self.__variant
        
class Matrix(DataType):
    """
    Stromx runtime::Image type. Uses the implementation cvsupport::Image to cast
    input data to an runtime::Image object.
    """
    def __init__(self, variant = "runtime::DataVariant::MATRIX"):
        self.__variant = variant
        
    def typeId(self):
        return "runtime::Matrix"
        
    def concreteTypeId(self):
        return "cvsupport::Matrix"
        
    def variant(self):
        return self.__variant
            
if __name__ == "__main__":
    import doctest
    doctest.testmod()
    
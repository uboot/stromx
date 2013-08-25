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
        
    def parentVariant(self):
        """
        Returns the most general variant corresponding to this data type, e.g.
        "runtime::DataVariant::MATRIX" for the data type Float32Matrix.
        """
        return self.variant()
        
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
        
class Float32(DataType):
    """
    Stromx runtime::Float32 type.
    """
    def typeId(self):
        return "runtime::Float32"
        
    def variant(self):
        return "runtime::DataVariant::FLOAT_32"
        
class Float64(DataType):
    """
    Stromx runtime::Float64 type.
    """
    def typeId(self):
        return "runtime::Float64"
        
    def variant(self):
        return "runtime::DataVariant::FLOAT_64"
        
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
        
    def parentVariant(self):
        return "runtime::DataVariant::IMAGE"
        
    def variant(self):
        return self.__variant
        
class Matrix(DataType):
    """
    Stromx runtime::Image type. Uses the implementation cvsupport::Matrix to cast
    input data to an runtime::Matrix object.
    """
    def __init__(self, variant = "runtime::DataVariant::MATRIX"):
        self.__variant = variant
        
    def typeId(self):
        return "runtime::Matrix"
        
    def concreteTypeId(self):
        return "cvsupport::Matrix"
        
    def parentVariant(self):
        return "runtime::DataVariant::MATRIX"
        
    def variant(self):
        return self.__variant
    
class FloatMatrix(Matrix):
    def __init__(self):
        super(FloatMatrix, self).__init__("runtime::DataVariant::FLOAT_MATRIX")
    
class Float32Matrix(Matrix):
    def __init__(self):
        super(Float32Matrix, self).__init__("runtime::DataVariant::FLOAT_32_MATRIX")
    
class Float64Matrix(Matrix):
    def __init__(self):
        super(Float32Matrix, self).__init__("runtime::DataVariant::FLOAT_64_MATRIX")
            
if __name__ == "__main__":
    import doctest
    doctest.testmod()
    
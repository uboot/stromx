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
        
    def canBeCreatedFromVariant(self):
        """
        Returns the most general variant of objects which can be converted to
        objects of this type. E.g. any MATRIX can be initialized to a
        FLOAT_32_MATRIX (without preserving its data) provided that its memory
        is large enough to hold the new object.
        """
        return self.variant()
        
    def variant(self):
        """
        Returns the variant of the type, e.g. "runtime::Variant::BOOL".
        """
        raise NotImplementedError()
        
    def cast(self, src):
        """
        Returns a conversion of the input variable to the type, e.g. 
        "cvsupport::Image(img)".
        """
        return "{0}({1})".format(self.concreteTypeId(), src)
        
    def allocate(self, src):
        """
        Returns an allocation on the stack of type which is initialized by the 
        input variable.
        """
        return "new {0}".format(self.cast(src))
        
class Bool(DataType):
    """
    Stromx runtime::Bool type.
    """
    def typeId(self):
        return "runtime::Bool"
        
    def variant(self):
        return "runtime::Variant::BOOL"
        
class UInt8(DataType):
    """
    Stromx runtime::UInt8 type.
    """
    def typeId(self):
        return "runtime::UInt8"
        
    def variant(self):
        return "runtime::Variant::UINT_8"
        
class Int32(DataType):
    """
    Stromx runtime::Int32 type.
    """
    def typeId(self):
        return "runtime::Int32"
        
    def variant(self):
        return "runtime::Variant::INT_32"
        
class UInt32(DataType):
    """
    Stromx runtime::UInt32 type.
    """
    def typeId(self):
        return "runtime::UInt32"
        
    def variant(self):
        return "runtime::Variant::UINT_32"
        
class Float32(DataType):
    """
    Stromx runtime::Float32 type.
    """
    def typeId(self):
        return "runtime::Float32"
        
    def variant(self):
        return "runtime::Variant::FLOAT_32"
        
class Float64(DataType):
    """
    Stromx runtime::Float64 type.
    """
    def typeId(self):
        return "runtime::Float64"
        
    def variant(self):
        return "runtime::Variant::FLOAT_64"
        
class Enum(DataType):
    """
    Stromx runtime::Enum type.
    """
    def typeId(self):
        return "runtime::Enum"
        
    def variant(self):
        return "runtime::Variant::ENUM"
        
class Image(DataType):
    """
    Stromx runtime::Image type. Uses the implementation cvsupport::Image to cast
    input data to an runtime::Image object.
    """
    def __init__(self, variant = "runtime::Variant::IMAGE"):
        self.__variant = variant
        
    def typeId(self):
        return "runtime::Image"
        
    def concreteTypeId(self):
        return "cvsupport::Image"
        
    def variant(self):
        return self.__variant
        
class Matrix(DataType):
    """
    Stromx runtime::Image type. Uses the implementation cvsupport::Matrix to cast
    input data to an runtime::Matrix object.
    """
    def __init__(self, variant = "runtime::Variant::MATRIX"):
        self.__variant = variant
        
    def typeId(self):
        return "runtime::Matrix"
        
    def concreteTypeId(self):
        return "cvsupport::Matrix"
        
    def canBeCreatedFromVariant(self):
        return "runtime::Variant::MATRIX"
        
    def variant(self):
        return self.__variant
    
class IntMatrix(Matrix):
    def __init__(self):
        super(IntMatrix, self).__init__("runtime::Variant::INT_MATRIX")
    
class Int32Matrix(Matrix):
    def __init__(self):
        super(Int32Matrix, self).__init__("runtime::Variant::INT_32_MATRIX")
    
class FloatMatrix(Matrix):
    def __init__(self):
        super(FloatMatrix, self).__init__("runtime::Variant::FLOAT_MATRIX")
    
class Float32Matrix(Matrix):
    def __init__(self):
        super(Float32Matrix, self).__init__("runtime::Variant::FLOAT_32_MATRIX")
    
class Float64Matrix(Matrix):
    def __init__(self):
        super(Float64Matrix, self).__init__("runtime::Variant::FLOAT_64_MATRIX")
    
class Any32BitMatrix(Matrix):
    def __init__(self):
        super(Any32BitMatrix, self).__init__(
            "runtime::Variant::INT_32_MATRIX || "
            "runtime::Variant::FLOAT_32_MATRIX"
        )
    
class List(DataType):
    """
    Stromx runtime::List type.
    """
    def __init__(self, elementType):
        self.elementType = elementType
        
    def typeId(self):
        return "runtime::List"
        
    def canBeCreatedFromVariant(self):
        return "runtime::Variant::Data"
        
    def variant(self):
        return "runtime::Variant::LIST"
        
    def cast(self, src):
        return "runtime::TypedList<{1}>({0})".format(src,
            self.elementType.concreteTypeId())
        
if __name__ == "__main__":
    import doctest
    doctest.testmod()
    
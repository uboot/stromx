# -*- coding: utf-8 -*-

#
#  Copyright 2011 Matthias Fuchs
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

from libruntime import *

from ctypes import pythonapi
import ctypes

class _PY_BUFFER(ctypes.Structure):
    _fields_ = [
        ("buf", ctypes.c_void_p),
        ("obj", ctypes.py_object),
        ("len", ctypes.c_ssize_t),
        ("itemsize", ctypes.c_ssize_t),
        ("readonly", ctypes.c_int),
        ("ndim", ctypes.c_int),
        ("format", ctypes.c_char_p),
        ("shape", ctypes.POINTER(ctypes.c_ssize_t)),
        ("strides", ctypes.POINTER(ctypes.c_ssize_t)),
        ("suboffsets", ctypes.POINTER(ctypes.c_ssize_t)),
        ("smalltable", ctypes.c_ssize_t * 2),
        ("internal", ctypes.c_void_p)
    ]

def _memoryViewForMatrix(self):
    matrix = self
    
    SHAPE = ctypes.c_ssize_t * 2
    matrix.shape = SHAPE(matrix.rows(), matrix.cols())
    
    STRIDES = ctypes.c_ssize_t * 2
    matrix.strides = STRIDES(matrix.stride(), matrix.valueSize())
    
    if matrix.valueType() == Matrix.ValueType.INT_8:
        f = 'b'
    elif matrix.valueType() == Matrix.ValueType.UINT_8:
        f = 'B'
    elif matrix.valueType() == Matrix.ValueType.INT_16:
        f = 'h'
    elif matrix.valueType() == Matrix.ValueType.UINT_16:
        f = 'H'
    elif matrix.valueType() == Matrix.ValueType.INT_32:
        f = 'i'
    elif matrix.valueType() == Matrix.ValueType.UINT_32:
        f = 'I'
    elif matrix.valueType() == Matrix.ValueType.FLOAT_32:
        f = 'f'
    elif matrix.valueType() == Matrix.ValueType.FLOAT_64:
        f = 'd'
    else:
        assert(False)
    
    pybuffer = _PY_BUFFER()
    pybuffer.buf = matrix._data()
    pybuffer.obj = ctypes.py_object()
    pybuffer.len = matrix.rows() * matrix.cols()
    pybuffer.itemsize = matrix.valueSize()
    pybuffer.readonly = 0
    pybuffer.ndim = 2
    pybuffer.format = f
    pybuffer.shape = matrix.shape
    pybuffer.strides = matrix.strides
    pybuffer.suboffsets = ctypes.POINTER(ctypes.c_ssize_t)()
    pybuffer.smalltable[0] = 0
    pybuffer.smalltable[1] = 0
    pybuffer.internal = ctypes.c_void_p()
    
    pythonapi.PyMemoryView_FromBuffer.argtypes = [ctypes.POINTER(_PY_BUFFER)]
    pythonapi.PyMemoryView_FromBuffer.restype = ctypes.py_object
    
    return pythonapi.PyMemoryView_FromBuffer(ctypes.byref(pybuffer))
    
Matrix.data = _memoryViewForMatrix

def _printVector(self):
    string = "["
    for i in range(len(self)):
        string += str(self[i])
        if(i < len(self) - 1):
            string += ", "
    string += "]"
    return string
   
def _extendVectorClass(vectorClass):
    vectorClass.__repr__ = _printVector
    
_extendVectorClass(OperatorVector)
_extendVectorClass(OperatorKernelVector)
_extendVectorClass(DescriptionVector)
_extendVectorClass(ParameterVector)
_extendVectorClass(ThreadVector)
_extendVectorClass(InputVector)
_extendVectorClass(DataVector)

def _returnObject(self):
    return self
    
def _releaseObject(self, exc_type, exc_value, traceback):
    self.release()
    
def _closeObject(self, exc_type, exc_value, traceback):
    self.close()
    
def _extendHandleObject(objectClass):
    objectClass.__enter__ = _returnObject
    objectClass.__exit__ = _releaseObject
    
def _extendFileObject(objectClass):
    objectClass.__enter__ = _returnObject
    objectClass.__exit__ = _closeObject
    
_extendHandleObject(DataContainer)
_extendHandleObject(ReadAccess)
_extendHandleObject(WriteAccess)

_extendFileObject(ZipFileInput)
_extendFileObject(DirectoryFileInput)
_extendFileObject(ZipFileOutput)
_extendFileObject(DirectoryFileOutput)

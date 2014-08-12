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
from ctypes import py_object

def _dataToPyBuffer(self):
    f = pythonapi.PyBuffer_FromMemory
    f.restype = py_object
    return pythonapi.PyBuffer_FromMemory(self._data(), self.bufferSize())
   
Matrix.data = _dataToPyBuffer

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

def _returnObject(self):
    return self
    
def _releaseObject(self, exc_type, exc_value, traceback):
    self.release()
    
def _extendObject(objectClass):
    objectClass.__enter__ = _returnObject
    objectClass.__exit__ = _releaseObject
    
_extendObject(DataContainer)
_extendObject(ReadAccess)
_extendObject(WriteAccess)

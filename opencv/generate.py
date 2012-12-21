# -*- coding: utf-8 -*-
"""
Created on Fri Dec 21 17:07:19 2012

@author: matz
"""

from package import *

p = Package()
p.ident = "imgproc"
p.name = "OpenCV image processing"

m = Method()
m.ident = "medianBlur"
m.name = "Median blur"

arg1 = Input()
arg1.name = "source"
arg1.cvType = CvType.MAT
arg1.dataType = DataType.IMAGE

arg2 = Output()
arg2.name = "destination"
arg2.cvType = CvType.MAT
arg2.dataType = DataType.IMAGE
arg2.inPlace = arg1

arg3 = NumericParameter()
arg3.cvType = CvType.INT
arg3.dataType = DataType.UINT_32
arg3.minValue = 1
arg3.rules.append(OddRule())

m.args = [arg1, arg2, arg3]
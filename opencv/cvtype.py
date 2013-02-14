# -*- coding: utf-8 -*-
"""
Created on Sat Jan 19 17:00:37 2013

@author: matz
"""

class CvType(object):
    def typeId(self):
        raise NotImplementedError()
        
    def cast(self, src):
        return "{0}({1})".format(self.typeId(), src)
        
class Bool(CvType):
    def typeId(self):
        return "bool"
        
class Int(CvType):
    def typeId(self):
        return "int"
        
class Mat(CvType):
    def typeId(self):
        return "cv::Mat"
    
    def cast(self, src):
        return "example::getOpenCvMat({0})".format(src)
        
if __name__ == "__main__":
    import doctest
    doctest.testmod()
    
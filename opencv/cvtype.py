# -*- coding: utf-8 -*-
"""
Created on Sat Jan 19 17:00:37 2013

@author: matz
"""

class CvType(object):
    def ident(self):
        raise NotImplementedError()
        
    def cast(self):
        return self.ident()
        
class Bool(CvType):
    def ident(self):
        return "bool"
        
class Int(CvType):
    def ident(self):
        return "int"
        
class Mat(CvType):
    def ident(self):
        return "cv::Mat"
    
    def cast(self):
        return "example::getOpenCvMat"
        
if __name__ == "__main__":
    import doctest
    doctest.testmod()
    
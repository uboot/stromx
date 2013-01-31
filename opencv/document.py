# -*- coding: utf-8 -*-
"""
Created on Thu Jan 31 21:41:06 2013

@author: matz
"""


class Document(object):
    INDENT = 4
    
    def __init__(self):
        self.lines = []
        self.indent = 0
        
    def string(self):
        s = ""
        for line in self.lines:
            s += line + "\n"
        return s
    
    def line(self, line):
        self.lines.append("{0}{1}".format(" " * self.indent, line))
        
    def increaseIndent(self):
        self.indent += Document.INDENT
        
    def decreaseIndent(self):
        self.indent -= Document.INDENT
        
    def blank(self):
        self.line("")
        
    def className(self):
        return self.m.ident.className()
        
    def namespaceEnter(self, name):
        self.line("namespace {0}".format(self.p.ident.lower()))
        self.scopeEnter()
    
    def namespaceExit(self):
        self.scopeExit()
        
    def scopeEnter(self):
        self.line("{")
        self.increaseIndent()
        
    def scopeExit(self):
        self.decreaseIndent()
        self.line("}")
        
    def label(self, key):
        self.decreaseIndent()
        self.line("{0}:".format(key))
        self.increaseIndent()
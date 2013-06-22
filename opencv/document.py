# -*- coding: utf-8 -*-
"""
Created on Thu Jan 31 21:41:06 2013

@author: matz
"""

class Document(object):
    INDENT = 4
    
    def __init__(self, text = None):
        self.lines = []
        self.indent = 0
        self.lastCmdWasBlank = False
        
        if text != None:
            self.lines = text.split("\n")
        
    def string(self):
        s = ""
        for line in self.lines:
            s += line + "\n"
        return s
    
    def line(self, line):
        self.lines.append("{0}{1}".format(" " * self.indent, line))
        self.lastCmdWasBlank = False
        
    def text(self, text):
        for l in text.split("\n"):
            self.lines.append(l)
        
    def document(self, doc):
        if doc == None:
            return
        
        # remove leading and trailing empty lines
        lines = doc.lines
        if len(lines) and lines[0] == "":
            lines = lines[1:]
            
        if len(lines) and lines[-1] == "":
            lines = lines[:-1]
            
        for l in lines:
            self.line(l)
        
    def increaseIndent(self):
        self.indent += Document.INDENT
        
    def decreaseIndent(self):
        self.indent -= Document.INDENT
        
    def blank(self):
        if not self.lastCmdWasBlank:
            self.line("")
            self.lastCmdWasBlank = True
        
    def className(self):
        return self.m.ident.className()
        
    def namespaceEnter(self, name):
        self.line("namespace {0}".format(name))
        self.scopeEnter()
    
    def namespaceExit(self):
        self.scopeExit()
        
    def scopeEnter(self):
        self.line("{")
        self.increaseIndent()
        self.lastCmdWasBlank = False
        
    def scopeExit(self):
        self.decreaseIndent()
        self.line("}")
        self.lastCmdWasBlank = False
        
    def label(self, key):
        self.decreaseIndent()
        self.line("{0}:".format(key))
        self.increaseIndent()
        self.lastCmdWasBlank = False
        
    def enum(self, name, keys):
        self.line("enum {0}".format(name))
        self.scopeEnter()
        for i, key in enumerate(keys):
            if i != len(keys) - 1:
                self.line("{0},".format(key))
            else:
                self.line(key)
                
        self.decreaseIndent()
        self.line("};")
        self.lastCmdWasBlank = False
# -*- coding: utf-8 -*-
"""
Created on Thu Jan 31 21:41:06 2013

@author: matz
"""

class Document(object):
    """
    Represents a text file composed of single lines.
    """
    INDENT = 4
    
    def __init__(self, text = None):
        self.lines = []
        self.indent = 0
        self.lastCmdWasBlank = False
        
        if text != None:
            self.lines = text.split("\n")
        
    def string(self):
        """
        Returns the current document as string with lines separated by line
        breaks.
        """
        s = ""
        for line in self.lines:
            s += line + "\n"
        return s
    
    def line(self, line):
        """
        Adds a line to the document. The line is automatically intendended by
        the current intendation level.
        """
        self.lines.append("{0}{1}".format(" " * self.indent, line))
        self.lastCmdWasBlank = False
        
    def text(self, text):
        """
        The input text is split at the line breaks and each line is added to 
        the document. The lines are automatically intendended by
        the current intendation level. If the first or the last lines of the
        input are empty they are removed.
        """
        doc = Document()
        for l in text.split("\n"):
            doc.line(l)
            
        self.document(doc)
            
    def document(self, doc):
        """
        Inserts a document into this document by appending each line of the 
        input document. The lines are automatically intendended by
        the current intendation level. If the first or the last lines of the
        input document are empty they are removed.
        """
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
        """
        Increases the current indendation.
        """
        self.indent += Document.INDENT
        
    def decreaseIndent(self):
        """
        Decreases the current indendation.
        """
        self.indent -= Document.INDENT
        
    def blank(self):
        """
        Inserts a blank line.
        """
        if not self.lastCmdWasBlank:
            self.line("")
            self.lastCmdWasBlank = True
        
    def namespaceEnter(self, name):
        """
        Opens a namespace scope.
        """
        self.line("namespace {0}".format(name))
        self.scopeEnter()
    
    def namespaceExit(self, name = ""):
        """
        Closes the current namespace scope.
        """
        self.scopeExit(name)
        
    def scopeEnter(self):
        """
        Inserts a "{" in a new line and increases the intendation.
        """
        self.line("{")
        self.increaseIndent()
        self.lastCmdWasBlank = False
        
    def scopeExit(self, comment = ""):
        """
        Inserts a "}" in a new line and increases the intendation.
        """
        self.decreaseIndent()
        if comment == "":
          self.line("}")
        else:
          self.line("} // " + str(comment))
        self.lastCmdWasBlank = False
        
    def label(self, key):
        """
        Inserts a labely, i.e. the line "key:".
        """
        self.decreaseIndent()
        self.line("{0}:".format(key))
        self.increaseIndent()
        self.lastCmdWasBlank = False
        
    def enum(self, name, keys):
        """
        Inserts a named enumeration with keys.
        """
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
        
def pythonToCpp(value):
    if value is False:
        value = "false"
    elif value is True:
        value = "true"
    elif isinstance(value, tuple):
        value = tuple([pythonToCpp(v) for v in value])
        value = myList = ', '.join(map(str, value)) 
    else:
        pass
        
    return value
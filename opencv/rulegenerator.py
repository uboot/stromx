# -*- coding: utf-8 -*-

import interface
    
class CheckParameterVisitor(interface.DataRuleVisitor):
    """
    Exports a check before the parameter is set for each visited rule of the
    parameter.
    """
    def __init__(self, doc, param):
        self.doc = doc
        self.param = param
        
    def visitOddRule(self, rule):
        self.doc.line("if(int(castedValue) % 2 == 0)")
        self.doc.increaseIndent()
        self.doc.line((
            'throw runtime::WrongParameterValue(*{0}Parameter, '
            '*this, "Only odd values are allowed");'
            ).format(self.param.ident.attribute()))
        self.doc.decreaseIndent()
        
    def visitEvenRule(self, rule):
        self.doc.line("if(int(castedValue) % 2 == 1)")
        self.doc.increaseIndent()
        self.doc.line((
            'throw runtime::WrongParameterValue(*{0}Parameter, '
            '*this, "Only even values are allowed");'
            ).format(self.param.ident.attribute()))
        self.doc.decreaseIndent()

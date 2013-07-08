# -*- coding: utf-8 -*-

import interface
    
class CheckParameterVisitor(interface.DataRuleVisitor):
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

    def visitNumRowsRule(self, rule):
        test = (
"""
if(castedValue.rows() != {0})
    throw runtime::WrongParameterValue(*{1}Parameter, *this, "Number of matrix rows must be {0}.");
"""
        ).format(rule.numRows, self.param.ident.attribute())
        self.doc.text(test)

    def visitNumColsRule(self, rule):
        test = (
"""
if(castedValue.cols() != {0})
    throw runtime::WrongParameterValue(*{1}Parameter, *this, "Number of matrix columns must be {0}.");
"""
        ).format(rule.numCols, self.param.ident.attribute())
        self.doc.text(test)

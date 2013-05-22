# -*- coding: utf-8 -*-

import package
import test

def generate(doc, args, testData):
    for arg, data in zip(args, testData):
        if isinstance(data, test.Default):
            continue
        
        if isinstance(arg, package.Compound):
            generate(doc, arg.args, data)
            continue
        
        doc.line(arg.ident)
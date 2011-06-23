# -*- coding: utf-8 -*-

import datetime
import string
import re

#import argparse

#parser = argparse.ArgumentParser(description='Create empty class templates')
#parser.add_argument('--author', '-a', metavar='STRING', required=True,
                    #help='File author')

#args = parser.parse_args()

def replace(text, author, className, package):
    year = str(datetime.date.today().year)
    preprocessorMacro = package.upper() + '_' + className.upper() + '_H'
    header = className + '.h'
    
    text = re.sub(r'@PREPROCESSOR_MACRO@', preprocessorMacro, text)
    text = re.sub(r'@CLASS@', className, text)
    text = re.sub(r'@PACKAGE@', package, text)
    text = re.sub(r'@AUTHOR@', author, text)
    text = re.sub(r'@YEAR@', year, text)
    text = re.sub(r'@HEADER@', header, text)
    return text
    
author = 'Matthias Fuchs'
className = 'Operator'
package = 'stream'

def process(inFilePath, outFilePath, author, className, package):
    inFile = open(inFilePath, 'r')
    text = inFile.read()
    inFile.close()
    
    outFile = open(outFilePath, 'w')
    outFile.write(replace(text, author, className, package))
    outFile.close()

process('Class.h.in', className + '.h', author, className, package)
process('Class.cpp.in', className + '.cpp', author, className, package)

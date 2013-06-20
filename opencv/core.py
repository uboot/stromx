# -*- coding: utf-8 -*-
"""
Created on Mon Apr  1 18:19:38 2013

@author: matz
"""

import cvtype
import datatype
import document
import generator
import package
import test

# abbreviations
dt = test.Default()

# utilitiy functions
dcl = document.Document()
dcl.line("void checkEnumValue(const stromx::runtime::Enum & value, "
         "const stromx::runtime::EnumParameter* param, "
         "const stromx::runtime::OperatorKernel& op);")
dclIncludes = ["<stromx/runtime/Enum.h>",
               "<stromx/runtime/EnumParameter.h>",
               "<stromx/runtime/OperatorKernel.h>"]
dtn = document.Document()
dtn.line("void checkEnumValue(const stromx::runtime::Enum & value, "
         "const stromx::runtime::EnumParameter* param, "
         "const stromx::runtime::OperatorKernel& op)")
dtn.scopeEnter()
dtn.line("using namespace runtime;")
dtn.blank()
dtn.line("for(std::vector<EnumDescription>::const_iterator "
         "iter = param->descriptions().begin(); iter != "
         "param->descriptions().end(); ++iter)")
dtn.scopeEnter()
dtn.line(" if(value == iter->value())")
dtn.line("return;")
dtn.scopeExit()
dtn.line("throw stromx::runtime::WrongParameterValue(*param, op);")
dtn.scopeExit()
dtnIncludes = ["<stromx/runtime/OperatorException.h>"]
checkEnumValue = package.Function(dcl, dclIncludes, dtn, dtnIncludes)

dcl = document.Document()
dclIncludes = ["<stromx/runtime/NumericParameter.h>",
               "<stromx/runtime/OperatorException.h>"]
dcl.line("template<class T>");
dcl.line("void checkNumericValue(const T & value, const "
         "runtime::NumericParameter<T>* param, "
         "const stromx::runtime::OperatorKernel& op)");
dcl.scopeEnter()
dcl.line("if(value < runtime::data_cast<T>(param->min()))")
dcl.increaseIndent()
dcl.line("throw runtime::WrongParameterValue(*param, op);")
dcl.decreaseIndent()
dcl.line("if(value > runtime::data_cast<T>(param->max()))")
dcl.increaseIndent()
dcl.line("throw runtime::WrongParameterValue(*param, op);")
dcl.decreaseIndent()
dcl.scopeExit()
checkNumericValue = package.Function(dcl, dclIncludes)

# initializations
initInCopy = document.Document((
    "{1}->initializeImage({0}->width(), {0}->height(), {0}->stride(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData"
))
initOutCopy = document.Document((
    "{1}->initializeImage({1}->width(), {1}->height(), {1}->stride(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData"
))

# arguments
srcImg1 = package.Argument(
    "src1", "Source 1", cvtype.Mat(), datatype.Image()
)
srcImg2 = package.Argument(
    "src2", "Source 2", cvtype.Mat(), datatype.Image()
)
dstImg = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(), initIn = initInCopy,
    initOut = initOutCopy
)

# test data
lenna = test.ImageFile("lenna.jpg")
memory = test.ImageBuffer(1000000)

# add
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg1), package.Input(srcImg2), package.Output(dstImg)],
    tests = [
        [lenna, lenna, memory]
    ]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImg1), package.Input(srcImg2), package.Allocation(dstImg)],
    tests = [
        [lenna, lenna, dt]
    ]
)
add = package.Method(
    "add", options = [manual, allocate]
)

core = package.Package(
    "core", 0, 0, 1,
    methods = [
        add
    ],
    functions = [
        checkEnumValue,
        checkNumericValue
    ],
    testFiles = [
        "lenna.jpg"
    ]
)

generator.generatePackageFiles(core) 
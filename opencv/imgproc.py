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

# utilitiy functions
dcl = document.Document()
dcl.line("void checkEnumValue(const stromx::runtime::Enum & value, "
         "const stromx::runtime::EnumParameter* param, "
         "const stromx::runtime::OperatorKernel& op);")
dclIncludes = ["<stromx/runtime/Enum.h>",
               "<stromx/runtime/EnumParameter.h>",
               "<stromx/runtime/OperatorKernel.h>"]
dfn = document.Document()
dfn.line("void checkEnumValue(const stromx::runtime::Enum & value, "
         "const stromx::runtime::EnumParameter* param, "
         "const stromx::runtime::OperatorKernel& op)")
dfn.scopeEnter()
dfn.line("using namespace runtime;")
dfn.blank()
dfn.line("for(std::vector<EnumDescription>::const_iterator "
         "iter = param->descriptions().begin(); iter != "
         "param->descriptions().end(); ++iter)")
dfn.scopeEnter()
dfn.line(" if(value == iter->value())")
dfn.line("return;")
dfn.scopeExit()
dfn.line("throw stromx::runtime::WrongParameterValue(*param, op);")
dfn.scopeExit()
dfnIncludes = ["<stromx/runtime/OperatorException.h>"]
checkEnumValue = package.Function(dcl, dclIncludes, dfn, dfnIncludes)

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

# boxFilter
src = package.Argument(
    "src", "Source", cvtype.Mat(), datatype.Image()
)
initIn = document.Document((
    "{1}->initializeImage({0}->width(), {0}->height(), {0}->stride(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData"
))
initOut = document.Document((
    "{1}->initializeImage({1}->width(), {1}->height(), {1}->stride(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData"
))
dst = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(), initIn = initIn,
    initOut = initOut
)
ddepth = package.Constant(
    "-1"
)
ksizex = package.NumericParameter(
    "ksizex", "Kernel size X", cvtype.Int(), datatype.UInt32(), minValue = 1
)
ksizey = package.NumericParameter(
    "ksizey", "Kernel size Y", cvtype.Int(), datatype.UInt32(), minValue = 1
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(src, True), package.Output(dst), ddepth,
     package.Size(ksizex, ksizey)]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(src), package.Allocation(dst), ddepth,
     package.Size(ksizex, ksizey)]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(src), package.RefInput(dst, src), ddepth,
     package.Size(ksizex, ksizey)]
)
boxFilter = package.Method(
    "boxFilter", options = [manual, allocate, inPlace]
)

# dilate
src = package.Argument(
    "src", "Source", cvtype.Mat(), datatype.Image()
)
initIn = document.Document((
    "{1}->initializeImage({0}->width(), {0}->height(), {0}->stride(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData"
))
initOut = document.Document((
    "{1}->initializeImage({1}->width(), {1}->height(), {1}->stride(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData"
))
dst = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(), initIn = initIn,
    initOut = initOut
)
kernel = package.Constant(
    "cv::Mat()"
)
anchor = package.Constant(
    "cv::Point(-1, -1)"
)
iterations = package.NumericParameter(
    "iterations", "Number of iterations", cvtype.Int(), datatype.UInt32(),
    minValue = 1
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(src, True), package.Output(dst), kernel, anchor, iterations]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(src), package.Allocation(dst), kernel, anchor, iterations]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(src), package.RefInput(dst, src), kernel, anchor, iterations]
)
dilate = package.Method(
    "dilate", options = [manual, allocate, inPlace]
)

# medianBlur
src = package.Argument(
    "src", "Source", cvtype.Mat(), datatype.Image()
)
initIn = document.Document((
    "{1}->initializeImage({0}->width(), {0}->height(), {0}->stride(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData"
))
initOut = document.Document((
    "{1}->initializeImage({1}->width(), {1}->height(), {1}->stride(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData"
))
dst = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(), initIn = initIn,
    initOut = initOut
)
ksize = package.NumericParameter(
    "ksize", "Kernel size", cvtype.Int(), datatype.UInt32(), minValue = 1,
    step = 2, default = 1, rules = [package.OddRule()]
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(src, True), package.Output(dst), ksize]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(src), package.Allocation(dst), ksize]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(src), package.RefInput(dst, src), ksize]
)
medianBlur = package.Method(
    "medianBlur", options = [manual, allocate, inPlace]
)

# resize
src = package.Argument(
    "src", "Source", cvtype.Mat(), datatype.Image()
)
initIn = document.Document((
    "int width = int(m_dsizex) ? int(m_dsizex) : srcCastedData->width() * double(m_fx);\n"
    "int height = int(m_dsizey) ? int(m_dsizey) : srcCastedData->height() * double(m_fy);\n"
    "{1}->initializeImage(width, height, width * {0}->pixelSize(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData")
)
initOut = document.Document((
    "{1}->initializeImage({1}->width(), {1}->height(), {1}->stride(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData")
)
dst = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(), initIn = initIn,
    initOut = initOut
)
dsizex = package.NumericParameter(
    "dsizex", "Size X", cvtype.Int(), datatype.UInt32()
)
dsizey = package.NumericParameter(
    "dsizey", "Size Y", cvtype.Int(), datatype.UInt32()
)
fx = package.NumericParameter(
    "fx", "Scale X", cvtype.Double(), datatype.Double(), default = 1.0
)
fy = package.NumericParameter(
    "fy", "Scale Y", cvtype.Double(), datatype.Double(), default = 1.0
)
descriptions = [
    package.EnumDescription("INTER_NEAREST", "Nearest neighbour"),
    package.EnumDescription("INTER_LINEAR", "Bilinear")
]
interpolation = package.EnumParameter(
    "interpolation", "Interpolation", descriptions = descriptions,
    default = 1
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(src), package.Output(dst),
     package.Size(dsizex, dsizey), fx, fy, interpolation]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(src), package.Allocation(dst),
     package.Size(dsizex, dsizey), fx, fy, interpolation]
)
resize = package.Method(
    "resize", options = [manual, allocate]
)

imgproc = package.Package(
    "imgproc", 0, 0, 1,
    methods = [
        boxFilter,
        dilate,
        medianBlur,
        resize
    ],
    functions = [
        checkEnumValue,
        checkNumericValue
    ]
)

generator.generatePackageFiles(imgproc) 
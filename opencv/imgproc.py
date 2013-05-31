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

# initializations
initInCopy = document.Document((
    "{1}->initializeImage({0}->width(), {0}->height(), {0}->stride(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData"
))
initOutCopy = document.Document((
    "{1}->initializeImage({1}->width(), {1}->height(), {1}->stride(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData"
))
initInResize = document.Document((
    "int width = int(m_dsizex) ? int(m_dsizex) : srcCastedData->width() * double(m_fx);\n"
    "int height = int(m_dsizey) ? int(m_dsizey) : srcCastedData->height() * double(m_fy);\n"
    "{1}->initializeImage(width, height, width * {0}->pixelSize(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData")
)
initInPyr = document.Document((
    "{1}->initializeImage(m_dsizex, m_dsizey, m_dsizey * {0}->pixelSize(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData")
)

# arguments
srcImg = package.Argument(
    "src", "Source", cvtype.Mat(), datatype.Image()
)
dstImg = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(), initIn = initInCopy,
    initOut = initOutCopy
)
ddepthDefault = package.Constant(
    "-1"
)
ksizex = package.NumericParameter(
    "ksizex", "Kernel size X", cvtype.Int(), datatype.UInt32(), default = 3,
    minValue = 1
)
ksizey = package.NumericParameter(
    "ksizey", "Kernel size Y", cvtype.Int(), datatype.UInt32(), default = 3,
    minValue = 1
)
ksizexOdd = package.NumericParameter(
    "ksizex", "Kernel size X", cvtype.Int(), datatype.UInt32(), default = 3,
    minValue = 1, rules = [package.OddRule()]
)
ksizeyOdd = package.NumericParameter(
    "ksizey", "Kernel size Y", cvtype.Int(), datatype.UInt32(), default = 3,
    minValue = 1, rules = [package.OddRule()]
)
descriptions = [
    package.EnumDescription("MORPH_RECT", "Rectangle"),
    package.EnumDescription("MORPH_ELLIPSE", "Ellipse"),
    package.EnumDescription("MORPH_CROSS", "Cross")
]
shape = package.EnumParameter(
    "shape", "Kernel shape", descriptions = descriptions,
    default = 0
)
kernel = package.Call(
    "getStructuringElement(shapeCvData, cv::Size(ksizexCvData, ksizeyCvData))",
    [ksizex, ksizey, shape]
)
anchor = package.Constant(
    "cv::Point(-1, -1)"
)
iterations = package.NumericParameter(
    "iterations", "Number of iterations", cvtype.Int(), datatype.UInt32(),
    minValue = 1, default = 1
)
ksize = package.NumericParameter(
    "ksize", "Kernel size", cvtype.Int(), datatype.UInt32(), minValue = 1,
    step = 2, default = 1, rules = [package.OddRule()]
)
descriptions = [
    package.EnumDescription("MORPH_OPEN", "Open"),
    package.EnumDescription("MORPH_CLOSE", "Close"),
    package.EnumDescription("MORPH_GRADIENT", "Gradient"),
    package.EnumDescription("MORPH_TOPHAT", "Tophat"),
    package.EnumDescription("MORPH_BLACKHAT", "Blackhat")
]
op = package.EnumParameter(
    "op", "Operation", descriptions = descriptions,
    default = 1
)
d = package.NumericParameter(
    "d", "Pixel neigbourhood diameter", cvtype.Int(), datatype.UInt32(),
    default = 9
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
sigmaColor = package.NumericParameter(
    "sigmaColor", "Sigma color", cvtype.Double(), datatype.Double(),
    default = 50.0
)
sigmaSpace = package.NumericParameter(
    "sigmaSpace", "Sigma space", cvtype.Double(), datatype.Double(),
    default = 50.0
)
sigmaX = package.NumericParameter(
    "sigmaX", "Sigma X", cvtype.Double(), datatype.Double(), default = 0.0
)
sigmaY = package.NumericParameter(
    "sigmaY", "Sigma Y", cvtype.Double(), datatype.Double(), default = 0.0
)
descriptions = [
    package.EnumDescription("INTER_NEAREST", "Nearest neighbour"),
    package.EnumDescription("INTER_LINEAR", "Bilinear")
]
interpolation = package.EnumParameter(
    "interpolation", "Interpolation", descriptions = descriptions,
    default = 1
)
descriptions = [
    package.EnumDescription("SAME", "Same as input", -1),
    package.EnumDescription("DEPTH_8_BIT", "8-bit", "CV_8U"),
    package.EnumDescription("DEPTH_16_BIT", "16-bit", "CV_16U")
]
ddepth = package.EnumParameter(
    "ddepth", "Destination depth", descriptions = descriptions,
    default = 0
)
scale = package.NumericParameter(
    "scale", "Scale", cvtype.Double(), datatype.Double(), default = 1.0
)
delta = package.NumericParameter(
    "delta", "Delta", cvtype.Double(), datatype.Double(), default = 0.0
)

# test data
lenna = test.ImageFile("lenna.jpg")
lenna_bw = test.ImageFile("lenna.jpg", color = False)
memory = test.ImageBuffer(1000000)

# bilateralFilter
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImg), d, sigmaColor,
     sigmaSpace],
    tests = [
        [lenna, memory, 9, 100, 75]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg), d, sigmaColor,
     sigmaSpace],
    tests = [
        [lenna, test.Default(), test.Default(), test.Default(), test.Default()],
        [lenna_bw, test.Default(), 9, 100, 75]
    ]
)
bilateralFilter = package.Method(
    "bilateralFilter", options = [manual, allocate]
)

# blur
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg, True), package.Output(dstImg),
     package.Size(ksizex, ksizey)],
    tests = [
        [lenna, memory, (3, 4)],
        [lenna_bw, test.RefData(lenna), test.Default()]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg),
     package.Size(ksizex, ksizey)],
    tests = [
        [lenna, test.Default(), test.Default()],
        [lenna_bw, test.Default(), test.Default()]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(srcImg), package.RefInput(dstImg, srcImg),
     package.Size(ksizex, ksizey)],
    tests = [
        [lenna, test.Default(), test.Default()]
    ]
)
blur = package.Method(
    "blur", options = [manual, allocate, inPlace]
)

# boxFilter
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg, True), package.Output(dstImg), ddepthDefault,
     package.Size(ksizex, ksizey)],
    tests = [
        [lenna, memory, test.Default(), (5, 4)],
        [lenna, test.RefData(lenna), test.Default(), test.Default()]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg), ddepthDefault,
     package.Size(ksizex, ksizey)],
    tests = [
        [lenna_bw, test.Default(), test.Default(), (4, 5)],
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(srcImg), package.RefInput(dstImg, srcImg), ddepthDefault,
     package.Size(ksizex, ksizey)],
    tests = [
        [lenna, test.Default(), test.Default(), test.Default()],
    ]
)
boxFilter = package.Method(
    "boxFilter", options = [manual, allocate, inPlace]
)

# dilate and erode
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg, True), package.Output(dstImg), kernel, anchor,
     iterations],
    tests = [
        [lenna, memory, (3, 4, 1), test.Default(), 2],
        [lenna_bw, memory, test.Default(), test.Default(), test.Default()]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg), kernel, anchor,
     iterations],
    tests = [
        [lenna, test.Default(), test.Default(), test.Default(), test.Default()]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(srcImg), package.RefInput(dstImg, srcImg), kernel,
     anchor, iterations],
    tests = [
        [lenna_bw, test.Default(), (test.Default(), test.Default(), 2),
         test.Default(), test.Default()]
    ]
)
dilate = package.Method(
    "dilate", options = [manual, allocate, inPlace]
)
erode = package.Method(
    "erode", options = [manual, allocate, inPlace]
)

# GaussianBlur
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg, True), package.Output(dstImg),
     package.Size(ksizexOdd, ksizeyOdd), sigmaX, sigmaY],
    tests = [
        [lenna, memory, (3, 5), 1.5, 2.5],
        [lenna, test.RefData(lenna), test.Default(), test.Default(),
         test.Default()]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg),
     package.Size(ksizexOdd, ksizeyOdd), sigmaX, sigmaY],
    tests = [
        [lenna, test.Default(), (3, 5), -1, -1]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(srcImg), package.RefInput(dstImg, srcImg),
     package.Size(ksizexOdd, ksizeyOdd), sigmaX, sigmaY],
    tests = [
        [lenna, test.Default(), test.Default(), 0, 0]
    ]
)
GaussianBlur = package.Method(
    "GaussianBlur", options = [manual, allocate, inPlace]
)

# medianBlur
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg, True), package.Output(dstImg), ksize],
    tests = [
        [lenna, memory, 4],
        [lenna_bw, test.RefData(lenna), 5]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg), ksize],
    tests = [
        [lenna_bw, test.Default(), test.Default()]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(srcImg), package.RefInput(dstImg, srcImg), ksize],
    tests = [
        [lenna, test.Default(), test.Default()]
    ]
)
medianBlur = package.Method(
    "medianBlur", options = [manual, allocate, inPlace]
)

# morphologyEx
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg, True), package.Output(dstImg), op, kernel,
     anchor, iterations],
    tests = [
        [lenna, memory, 0, (3, 4, 0), test.Default(), test.Default()],
        
        [lenna, memory, 0, (test.Default(), test.Default(), 1), test.Default(), 3]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg), op, kernel,
     anchor, iterations]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(srcImg), package.RefInput(dstImg, srcImg), op, kernel,
     anchor, iterations]
)
morphologyEx = package.Method(
    "morphologyEx", options = [manual, allocate, inPlace]
)

# Laplacian
initInLaplacian = initInCopy
dstImgLaplacian = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(),
    initIn = initInLaplacian, initOut = initOutCopy
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImgLaplacian), ddepth,
     ksize, scale, delta]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImgLaplacian), ddepth,
     ksize, scale, delta]
)
laplacian = package.Method(
    "Laplacian", options = [manual, allocate]
)

# pyrDown
dstImgPyr = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(),
    initIn = initInPyr, initOut = initOutCopy
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImgPyr), 
     package.Size(dsizex, dsizey)]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImgPyr), 
     package.Size(dsizex, dsizey)]
)
pyrDown = package.Method(
    "pyrDown", options = [manual, allocate]
)

# pyrUp
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImgPyr), 
     package.Size(dsizex, dsizey)]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImgPyr), 
     package.Size(dsizex, dsizey)]
)
pyrUp = package.Method(
    "pyrUp", options = [manual, allocate]
)

# resize
dstImgResize = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(),
    initIn = initInResize, initOut = initOutCopy
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImgResize),
     package.Size(dsizex, dsizey), fx, fy, interpolation]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImg), package.Allocation(dstImgResize),
     package.Size(dsizex, dsizey), fx, fy, interpolation]
)
resize = package.Method(
    "resize", options = [manual, allocate]
)

imgproc = package.Package(
    "imgproc", 0, 0, 1,
    methods = [
        bilateralFilter,
        blur,
        boxFilter,
        dilate,
        erode,
        GaussianBlur,
        medianBlur,
        morphologyEx,
        laplacian,
        pyrDown,
        pyrUp,
        resize
    ],
    functions = [
        checkEnumValue,
        checkNumericValue
    ],
    testFiles = [
        "lenna.jpg"
    ]
)

generator.generatePackageFiles(imgproc) 
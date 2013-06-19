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
initInResize = document.Document((
    "int width = int(m_dsizex) ? int(m_dsizex) : int(srcCastedData->width() * double(m_fx));\n"
    "int height = int(m_dsizey) ? int(m_dsizey) : int(srcCastedData->height() * double(m_fy));\n"
    "{1}->initializeImage(width, height, width * {0}->pixelSize(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData")
)
initInPyrDown = document.Document((
    "int width = int((srcCastedData->width() + 1) / 2 );\n"
    "int height = int((srcCastedData->height() + 1) / 2 );\n"
    "{1}->initializeImage(width, height, width * {0}->pixelSize(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData")
)
initInPyrUp = document.Document((
    "int width = 2  * srcCastedData->width();\n"
    "int height = 2 * srcCastedData->height();\n"
    "{1}->initializeImage(width, height, width * {0}->pixelSize(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData")
)
initInDdepth = document.Document((
    "runtime::Image::PixelType pixelType = imgutil::computeOutPixelType("
    "convertDdepth(m_ddepth), srcCastedData->pixelType());\n"
    "unsigned int stride = runtime::Image::pixelSize(pixelType) * "
    "srcCastedData->width();\n"
    "{1}->initializeImage({0}->width(), {0}->height(), stride, "
    "{1}->data(), pixelType);").format("srcCastedData", "dstCastedData"
))
initOutDdepth = document.Document((
    "runtime::Image::PixelType pixelType = imgutil::computeOutPixelType("
    "convertDdepth(m_ddepth), srcCastedData->pixelType());\n"
    "unsigned int stride = runtime::Image::pixelSize(pixelType) * "
    "srcCastedData->width();\n"
    "{1}->initializeImage({1}->width(), {1}->height(), stride, "
    "{1}->data(), pixelType);").format("srcCastedData", "dstCastedData"
))

# arguments
srcImg = package.Argument(
    "src", "Source", cvtype.Mat(), datatype.Image()
)
srcImgMono = package.Argument(
    "src", "Source", cvtype.Mat(),
    datatype.Image("runtime::DataVariant::MONO_IMAGE")
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
    step = 2, default = 3, rules = [package.OddRule()]
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
dx = package.NumericParameter(
    "dx", "Order X derivative", cvtype.Int(), datatype.UInt32(), default = 1
)
dy = package.NumericParameter(
    "dy", "Order Y derivative", cvtype.Int(), datatype.UInt32(), default = 0
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
dstImgDdepth = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(),
    initIn = initInDdepth, initOut = initOutDdepth
)
thresh = package.NumericParameter(
    "threshold", "Threshold", cvtype.Double(), datatype.Double(),
    default = 127.0
)
maxval = package.NumericParameter(
    "maxval", "Maximal value", cvtype.Double(), datatype.Double(),
    default = 255.0
)
descriptions = [
    package.EnumDescription("THRESH_BINARY", "Binary"),
    package.EnumDescription("THRESH_BINARY_INV", "Binary inverted"),
    package.EnumDescription("THRESH_TRUNC", "Truncate"),
    package.EnumDescription("THRESH_TOZERO", "Truncate to zero"),
    package.EnumDescription("THRESH_TOZERO_INV", "Truncate to zero inverted")
]
thresholdType = package.EnumParameter(
    "thresholdType", "Threshold type", descriptions = descriptions,
    default = 0
)

# test data
lenna = test.ImageFile("lenna.jpg")
lenna_bw = test.ImageFile("lenna.jpg", color = False)
memory = test.ImageBuffer(1000000)
bigMemory = test.ImageBuffer(10000000)

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
        [lenna, dt, dt, dt, dt],
        [lenna_bw, dt, 9, 100, 75]
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
        [lenna_bw, test.RefData(lenna), dt]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg),
     package.Size(ksizex, ksizey)],
    tests = [
        [lenna, dt, dt],
        [lenna_bw, dt, dt]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(srcImg), package.RefInput(dstImg, srcImg),
     package.Size(ksizex, ksizey)],
    tests = [
        [lenna, dt, dt]
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
        [lenna, memory, dt, (5, 4)],
        [lenna, test.RefData(lenna), dt, dt]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg), ddepthDefault,
     package.Size(ksizex, ksizey)],
    tests = [
        [lenna_bw, dt, dt, (4, 5)],
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(srcImg), package.RefInput(dstImg, srcImg), ddepthDefault,
     package.Size(ksizex, ksizey)],
    tests = [
        [lenna, dt, dt, dt],
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
        [lenna, memory, (3, 4, 1), dt, 2],
        [lenna_bw, memory, dt, dt, dt]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg), kernel, anchor,
     iterations],
    tests = [
        [lenna, dt, dt, dt, dt]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(srcImg), package.RefInput(dstImg, srcImg), kernel,
     anchor, iterations],
    tests = [
        [lenna_bw, dt, (dt, dt, 2), dt, dt]
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
        [lenna, test.RefData(lenna), dt, dt, dt]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg),
     package.Size(ksizexOdd, ksizeyOdd), sigmaX, sigmaY],
    tests = [
        [lenna, dt, (3, 5), -1, -1]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(srcImg), package.RefInput(dstImg, srcImg),
     package.Size(ksizexOdd, ksizeyOdd), sigmaX, sigmaY],
    tests = [
        [lenna, dt, dt, 0, 0]
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
        [lenna, memory, 3],
        [lenna_bw, test.RefData(lenna), 5]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg), ksize],
    tests = [
        [lenna_bw, dt, dt]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(srcImg), package.RefInput(dstImg, srcImg), ksize],
    tests = [
        [lenna, dt, dt]
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
        [lenna, memory, 0, (3, 4, 0), dt, dt],
        [lenna, test.RefData(lenna), 2, (dt, dt, 1), dt, 3]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg), op, kernel,
     anchor, iterations],
    tests = [
        [lenna_bw, dt, 0, dt, dt, dt],
        [lenna, dt, 3, (dt, dt, 2), dt, dt]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(srcImg), package.RefInput(dstImg, srcImg), op, kernel,
     anchor, iterations],
    tests = [
        [lenna_bw, dt, 1, (dt, dt, 1), dt, dt],
        [lenna, dt, 3, dt, dt, dt]
    ]
)
morphologyEx = package.Method(
    "morphologyEx", options = [manual, allocate, inPlace]
)

# Laplacian
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImgDdepth), ddepth,
     ksize, scale, delta],
    tests = [
        [lenna, memory, 0, 3, dt, dt],
        [lenna_bw, memory, 1, 3, 1, 0]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImgDdepth), ddepth,
     ksize, scale, delta],
    tests = [
        [lenna_bw, dt, 2, 5, 100, 1000],
        [lenna, dt, 2, 7, 50, 500]
    ]
)
laplacian = package.Method(
    "Laplacian", options = [manual, allocate]
)

# Sobel
sobelKsize = package.NumericParameter(
    "ksize", "Kernel size", cvtype.Int(), datatype.UInt32(), minValue = 1,
    maxValue = 7, step = 2, default = 3, rules = [package.OddRule()]
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImgDdepth), ddepth, 
     dx, dy, sobelKsize, scale, delta],
    tests = [
        [lenna, memory, 0, 1, 1, 1, 1, 0],
        [lenna_bw, memory, 1, 2, 0, 3, 1, 0]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImgDdepth), ddepth,
     dx, dy, sobelKsize, scale, delta],
    tests = [
        [lenna, dt, 0, dt, 2, 5, 2, dt],
        [lenna_bw, dt, 2, dt, dt, dt, 100, dt]
    ]
)
sobel = package.Method(
    "Sobel", options = [manual, allocate]
)

# Scharr
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImgDdepth), ddepth, 
     dx, dy, scale, delta],
    tests = [
        [lenna, memory, 0, 0, 1, 1, 0],
        [lenna_bw, memory, 1, 1, 0, 1, 0]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImgDdepth), ddepth,
     dx, dy, scale, delta],
    tests = [
        [lenna, dt, 0, dt, dt, 2, dt],
        [lenna_bw, dt, 2, 0, 1, 100, dt]
    ]
)
scharr = package.Method(
    "Scharr", options = [manual, allocate]
)

# pyrDown
dstImgPyr = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(),
    initIn = initInPyrDown, initOut = initOutCopy
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImgPyr)],
    tests = [
        [lenna, memory]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImgPyr)],
    tests = [
        [lenna_bw, dt]
    ]
)
pyrDown = package.Method(
    "pyrDown", options = [manual, allocate]
)

# pyrUp
dstImgPyr = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(),
    initIn = initInPyrUp, initOut = initOutCopy
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImgPyr)],
    tests = [
        [lenna, bigMemory]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImgPyr)],
    tests = [
        [lenna_bw, dt]
    ]
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
     package.Size(dsizex, dsizey), fx, fy, interpolation],
    tests = [
        [lenna, memory, dt, dt, dt],
        [lenna, memory, (100, 200), 0],
        [lenna_bw, memory, (100, 200), 0.5, 0.3, dt]
    ]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImg), package.Allocation(dstImgResize),
     package.Size(dsizex, dsizey), fx, fy, interpolation],
    tests = [
        [lenna_bw, dt, dt, 0.5, 0.3, dt]
    ]
)
resize = package.Method(
    "resize", options = [manual, allocate]
)

# threshold
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImgMono, True), package.Output(dstImg), thresh, maxval,
     thresholdType],
    tests = [
        [lenna_bw, memory, dt, dt, dt],
        [lenna_bw, test.RefData(lenna_bw), 128, dt, 2]
    ]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono), package.Allocation(dstImg), thresh, maxval,
     thresholdType],
    tests = [
        [lenna_bw, dt, dt, dt, 3]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Output(srcImgMono), package.RefInput(dstImg, srcImg), thresh, maxval,
     thresholdType],
    tests = [
        [lenna_bw, dt, dt, dt, 4]
    ]
)
threshold = package.Method(
    "threshold", options = [manual, allocate, inPlace]
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
        scharr,
        sobel,
        resize,
        threshold
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
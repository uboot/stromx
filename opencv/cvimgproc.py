# -*- coding: utf-8 -*-
"""
Created on Mon Apr  1 18:19:38 2013

@author: matz
"""

import math
import sys

import cvtype
import datatype
import document
import generator
import package
import test

# abbreviations
DT = test.Default()

# calcHistWrapper
dcl = document.Document()
dclIncludes = ["<opencv2/core/core.hpp>"]
dcl.text(
"""
void calcHist1D(const cv::Mat & input, cv::Mat & result, const float min, const float max, int size);
""")
dtnIncludes = ["<opencv2/imgproc/imgproc.hpp>"]
dtn = document.Document()              
dtn.text(
"""
void calcHist1D(const cv::Mat & input, cv::Mat & result, const float min, const float max, int size)
{
    int channels[] = {0};
    float range[] = {min, max};
    const float* ranges[] = {range};
    
    cv::calcHist(&input, 1, channels, cv::Mat(), result, 1, &size, ranges);
}

""")
calcHistWrapper = package.Function(dcl, dclIncludes, dtn, dtnIncludes)

# minEnclosingCircleWrapper
dcl = document.Document()
dclIncludes = ["<opencv2/core/core.hpp>"]
dcl.text(
"""
void minEnclosingCircle(const cv::Mat & points, cv::Mat & result);
""")
dtnIncludes = ["<opencv2/imgproc/imgproc.hpp>"]
dtn = document.Document()              
dtn.text(
"""
void minEnclosingCircle(const cv::Mat & points, cv::Mat & result)
{
    cv::Point2f center;
    float radius;
    cv::minEnclosingCircle(points, center, radius);
    
    result = cv::Mat(1, 3, CV_32F);
    result.at<float>(0, 0) = center.x;
    result.at<float>(0, 1) = center.y;
    result.at<float>(0, 2) = radius;
}
""")
minEnclosingCircleWrapper = package.Function(dcl, dclIncludes, dtn, dtnIncludes)

# fitLineWrapper
dcl = document.Document()
dclIncludes = ["<opencv2/core/core.hpp>"]
dcl.text(
"""
void fitLine(const cv::Mat & points, cv::Mat & result, const int distType,
             const double param, const double reps, const double aeps);
""")
dtnIncludes = ["<cmath>", "<opencv2/imgproc/imgproc.hpp>"]
dtn = document.Document()              
dtn.text(
"""
void fitLine(const cv::Mat & points, cv::Mat & result, const int distType,
             const double param, const double reps, const double aeps)
{
    cv::Vec4f line;
    cv::fitLine(points, line, distType, param, reps, aeps);
    
    result = cv::Mat(1, 2, CV_32F);
    result.at<float>(0, 0) = (line[1]*line[2] - line[0]*line[3]);
    result.at<float>(0, 1) = std::atan2(line[0], line[1]) * 180 / M_PI;
}
""")
fitLineWrapper = package.Function(dcl, dclIncludes, dtn, dtnIncludes)

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
initInDsize = document.Document((
    "int width = int(m_dsizex);\n"
    "int height = int(m_dsizey);\n"
    "{1}->initializeImage(width, height, width * {0}->pixelSize(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData")
)
initInDdepth = document.Document((
    "runtime::Image::PixelType pixelType = cvsupport::computeOutPixelType("
    "convertDdepth(m_ddepth), srcCastedData->pixelType());\n"
    "unsigned int stride = runtime::Image::pixelSize(pixelType) * "
    "srcCastedData->width();\n"
    "{1}->initializeImage({0}->width(), {0}->height(), stride, "
    "{1}->data(), pixelType);").format("srcCastedData", "dstCastedData"
))
initOutDdepth = document.Document((
    "runtime::Image::PixelType pixelType = cvsupport::computeOutPixelType("
    "convertDdepth(m_ddepth), srcCastedData->pixelType());\n"
    "unsigned int stride = runtime::Image::pixelSize(pixelType) * "
    "srcCastedData->width();\n"
    "{1}->initializeImage({1}->width(), {1}->height(), stride, "
    "{1}->data(), pixelType);").format("srcCastedData", "dstCastedData"
))
initInFloat32 = document.Document((
    "unsigned int stride = {0}->cols() * runtime::Matrix::valueSize(runtime::Matrix::FLOAT_32);\n"
    "{1}->initializeMatrix({0}->rows(), {0}->cols(), stride, "
    "{1}->data(), runtime::Matrix::FLOAT_32);").format("srcCastedData",
                                                       "dstCastedData"
))

# arguments
srcImg = package.Argument(
    "src", "Source", cvtype.Mat(), datatype.Image()
)
srcImgMono = package.Argument(
    "src", "Source", cvtype.Mat(),
    datatype.Image("runtime::Variant::MONO_IMAGE")
)
srcImgMono8bit = package.Argument(
    "src", "Source", cvtype.Mat(),
    datatype.Image("runtime::Variant::MONO_8_IMAGE")
)
dstImg = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(), initIn = initInCopy,
    initOut = initOutCopy
)
dstImgResize = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(),
    initIn = initInResize, initOut = initOutCopy
)
dstImgDsize = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(),
    initIn = initInDsize, initOut = initOutCopy
)
dstMatrix = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Matrix()
)
dstImgFloat32 = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Float32Matrix(),
    initIn = initInFloat32
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
defaultSize = package.Constant(
    "cv::Size(-1, -1)"
)
iterations = package.NumericParameter(
    "iterations", "Number of iterations", cvtype.Int(), datatype.UInt32(),
    minValue = 1, default = 1
)
ksize = package.NumericParameter(
    "ksize", "Kernel size", cvtype.Int(), datatype.UInt32(), minValue = 1,
    step = 2, default = 3, rules = [package.OddRule()]
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
sigmaColor = package.NumericParameter(
    "sigmaColor", "Sigma color", cvtype.Float64(), datatype.Float64(),
    default = 50.0
)
sigmaSpace = package.NumericParameter(
    "sigmaSpace", "Sigma space", cvtype.Float64(), datatype.Float64(),
    default = 50.0
)
sigmaX = package.NumericParameter(
    "sigmaX", "Sigma X", cvtype.Float64(), datatype.Float64(), default = 0.0
)
sigmaY = package.NumericParameter(
    "sigmaY", "Sigma Y", cvtype.Float64(), datatype.Float64(), default = 0.0
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
    "scale", "Scale", cvtype.Float64(), datatype.Float64(), default = 1.0
)
delta = package.NumericParameter(
    "delta", "Delta", cvtype.Float64(), datatype.Float64(), default = 0.0
)
dstImgDdepth = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(),
    initIn = initInDdepth, initOut = initOutDdepth
)
thresh = package.NumericParameter(
    "threshold", "Threshold", cvtype.Float64(), datatype.Float64(),
    default = 127.0
)
maxval = package.NumericParameter(
    "maxval", "Maximal value", cvtype.Float64(), datatype.Float64(),
    default = 255.0
)
blockSize = package.NumericParameter(
    "blockSize", "Block size", cvtype.Int(), datatype.UInt32(),
    default = 3, minValue = 1, rules = [package.OddRule()]
)
descriptions = [
    package.EnumDescription("SIZE_3", "3","3"),
    package.EnumDescription("SIZE_5", "5","5"),
    package.EnumDescription("SIZE_PRECISE", "Precise", "CV_DIST_MASK_PRECISE")
]
maskSize = package.EnumParameter(
    "maskSize", "Mask size", descriptions = descriptions,
    default = 0
)
seedPointX = package.NumericParameter(
    "seedPointX", "Seed point X", cvtype.Int(), datatype.UInt32()
)
seedPointY = package.NumericParameter(
    "seedPointY", "Seed point Y", cvtype.Int(), datatype.UInt32()
)
newVal =  package.NumericParameter(
    "newVal", "New value", cvtype.Float64(), datatype.Float64()
)
harrisK = package.NumericParameter(
    "k", "Harris parameter", cvtype.Float64(), datatype.Float64(),
    default = 1
)
accumulatorThreshold = package.NumericParameter(
    "threshold", "Accumulator threshold", cvtype.Int(), datatype.UInt32(),
    default = 100
)
minLineLength = package.NumericParameter(
    "minLineLength", "Minimum line length", cvtype.Float64(), datatype.Float64(),
    default = 50
)
maxLineGap = package.NumericParameter(
    "maxLineGap", "Maximum allowed gap", cvtype.Float64(), datatype.Float64(),
    default = 5
)
pointMatrix = package.MatrixArgument(
    "pointMatrix", "Point coordinates", cvtype.Mat(), datatype.Float32Matrix(),
    cols = 2
)
winSizeX = package.NumericParameter(
    "winSizeX", "Width of search window", cvtype.Int(), datatype.UInt32(),
    default = 5
)
winSizeY = package.NumericParameter(
    "winSizeY", "Height of search window", cvtype.Int(), datatype.UInt32(),
    default = 5
)
noArray = package.Constant(
    "cv::noArray()"
)

# test data
lenna = test.ImageFile("lenna.jpg")
lenna_bw = test.ImageFile("lenna.jpg", grayscale = True)
edges = test.ImageFile("edges.png", grayscale = True)
affine_transformation = test.MatrixFile("affine.npy")
perspective_transformation = test.MatrixFile("perspective.npy")
camera_matrix = test.MatrixFile("camera_matrix.npy")
dist_coeffs = test.MatrixFile("dist_coeffs.npy")
memory = test.ImageBuffer(1000000)
bigMemory = test.ImageBuffer(10000000) 
circle = test.ImageFile("circle.png", grayscale = True)
contours = test.ImageFile("contours.png", grayscale = True)
cornerImage = test.ImageFile("corners.png", grayscale = True)
cornerCoordinates = test.MatrixFile("corners.npy")
contour_1 = test.MatrixFile("contour_1.npy") # 32-bit integer coordinates
contour_2 = test.MatrixFile("contour_2.npy") # 32-bit integer coordinates
contour_f32 = test.MatrixFile("contour_f32.npy")
contour_f64 = test.MatrixFile("contour_f64.npy")
points_i32 = test.MatrixFile("points_i32.npy")
points_f32 = test.MatrixFile("points_f32.npy")
points_f64 = test.MatrixFile("points_f64.npy")
non_convex_f32 = test.MatrixFile("non_convex_f32.npy")
contourList = test.List(contour_1, contour_2)

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
        [lenna, DT, DT, DT, DT],
        [lenna_bw, DT, 9, 100, 75]
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
        [lenna_bw, test.RefData(lenna), DT]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg),
     package.Size(ksizex, ksizey)],
    tests = [
        [lenna, DT, DT],
        [lenna_bw, DT, DT]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.InputOutput(srcImg), package.RefInput(dstImg, srcImg),
     package.Size(ksizex, ksizey)],
    tests = [
        [lenna, DT, DT]
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
        [lenna, memory, DT, (5, 4)],
        [lenna, test.RefData(lenna), DT, DT]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg), ddepthDefault,
     package.Size(ksizex, ksizey)],
    tests = [
        [lenna_bw, DT, DT, (4, 5)],
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.InputOutput(srcImg), package.RefInput(dstImg, srcImg), ddepthDefault,
     package.Size(ksizex, ksizey)],
    tests = [
        [lenna, DT, DT, DT],
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
        [lenna, memory, (3, 4, 1), DT, 2],
        [lenna_bw, memory, DT, DT, DT]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg), kernel, anchor,
     iterations],
    tests = [
        [lenna, DT, DT, DT, DT]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.InputOutput(srcImg), package.RefInput(dstImg, srcImg), kernel,
     anchor, iterations],
    tests = [
        [lenna_bw, DT, (DT, DT, 2), DT, DT]
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
        [lenna, test.RefData(lenna), DT, DT, DT]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg),
     package.Size(ksizexOdd, ksizeyOdd), sigmaX, sigmaY],
    tests = [
        [lenna, DT, (3, 5), -1, -1]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.InputOutput(srcImg), package.RefInput(dstImg, srcImg),
     package.Size(ksizexOdd, ksizeyOdd), sigmaX, sigmaY],
    tests = [
        [lenna, DT, DT, 0, 0]
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
        [lenna_bw, DT, DT]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.InputOutput(srcImg), package.RefInput(dstImg, srcImg), ksize],
    tests = [
        [lenna, DT, DT]
    ]
)
medianBlur = package.Method(
    "medianBlur", options = [manual, allocate, inPlace]
)

# morphologyEx
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
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg, True), package.Output(dstImg), op, kernel,
     anchor, iterations],
    tests = [
        [lenna, memory, 0, (3, 4, 0), DT, DT],
        [lenna, test.RefData(lenna), 2, (DT, DT, 1), DT, 3]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImg), op, kernel,
     anchor, iterations],
    tests = [
        [lenna_bw, DT, 0, DT, DT, DT],
        [lenna, DT, 3, (DT, DT, 2), DT, DT]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.InputOutput(srcImg), package.RefInput(dstImg, srcImg), op, kernel,
     anchor, iterations],
    tests = [
        [lenna_bw, DT, 1, (DT, DT, 1), DT, DT],
        [lenna, DT, 3, DT, DT, DT]
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
        [lenna, memory, 0, 3, DT, DT],
        [lenna_bw, memory, 1, 3, 1, 0]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImgDdepth), ddepth,
     ksize, scale, delta],
    tests = [
        [lenna_bw, DT, 2, 5, 100, 1000],
        [lenna, DT, 2, 7, 50, 500]
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
        [lenna, DT, 0, DT, 2, 5, 2, DT],
        [lenna_bw, DT, 2, DT, DT, DT, 100, DT]
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
        [lenna, DT, 0, DT, DT, 2, DT],
        [lenna_bw, DT, 2, 0, 1, 100, DT]
    ]
)
scharr = package.Method(
    "Scharr", options = [manual, allocate]
)

# pyrDown
initInPyrDown = document.Document((
    "int width = int((srcCastedData->width() + 1) / 2 );\n"
    "int height = int((srcCastedData->height() + 1) / 2 );\n"
    "{1}->initializeImage(width, height, width * {0}->pixelSize(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData")
)
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
        [lenna_bw, DT]
    ]
)
pyrDown = package.Method(
    "pyrDown", options = [manual, allocate]
)

# pyrUp
initInPyrUp = document.Document((
    "int width = 2  * srcCastedData->width();\n"
    "int height = 2 * srcCastedData->height();\n"
    "{1}->initializeImage(width, height, width * {0}->pixelSize(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData")
)
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
        [lenna_bw, DT]
    ]
)
pyrUp = package.Method(
    "pyrUp", options = [manual, allocate]
)

# resize
fx = package.NumericParameter(
    "fx", "Scale X", cvtype.Float64(), datatype.Float64(), default = 1.0
)
fy = package.NumericParameter(
    "fy", "Scale Y", cvtype.Float64(), datatype.Float64(), default = 1.0
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
    [package.Input(srcImg), package.Output(dstImgResize),
     package.Size(dsizex, dsizey), fx, fy, interpolation],
    tests = [
        [lenna, memory, DT, DT, DT],
        [lenna, memory, (100, 200), 0],
        [lenna_bw, memory, (100, 200), 0.5, 0.3, DT]
    ]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImg), package.Allocation(dstImgResize),
     package.Size(dsizex, dsizey), fx, fy, interpolation],
    tests = [
        [lenna_bw, DT, DT, 0.5, 0.3, DT]
    ]
)
resize = package.Method(
    "resize", options = [manual, allocate]
)

# warpAffine
affineM = package.MatrixParameter(
    "affineM", "Affine transformation", datatype.FloatMatrix(),
    default = "cvsupport::Matrix::eye(2, 3, runtime::Matrix::FLOAT_32)", rows = 2,
    cols = 3
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImgDsize), affineM, 
     package.Size(dsizex, dsizey)],
    tests = [
        [lenna_bw, memory, affine_transformation, (400, 500)],
        [lenna, memory, DT, (400, 500)]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImgDsize), affineM, 
     package.Size(dsizex, dsizey)],
    tests = [
        [lenna, DT, affine_transformation, (400, 500)]
    ]
)
warpAffine = package.Method(
    "warpAffine", options = [manual, allocate]
)

# warpPerspective
perspectiveM = package.MatrixParameter(
    "affineM", "Perspective transformation", datatype.FloatMatrix(),
    default = "cvsupport::Matrix::eye(3, 3, runtime::Matrix::FLOAT_32)", rows = 3,
    cols = 3
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImgDsize), perspectiveM, 
     package.Size(dsizex, dsizey)],
    tests = [
        [lenna_bw, memory, perspective_transformation, (400, 500)],
        [lenna, memory, DT, (400, 500)]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImgDsize), perspectiveM, 
     package.Size(dsizex, dsizey)],
    tests = [
        [lenna, DT, perspective_transformation, (400, 500)]
    ]
)
warpPerspective = package.Method(
    "warpPerspective", options = [manual, allocate]
)

# undistort
cameraMatrix = package.MatrixParameter(
    "cameraMatrix", "Camera matrix", datatype.FloatMatrix(),
    default = "cvsupport::Matrix::eye(3, 3, runtime::Matrix::FLOAT_32)", rows = 3,
    cols = 3
)
distCoeffs = package.MatrixParameter(
    "distCoeffs", "Distortion coefficients", datatype.FloatMatrix(),
    default = "cvsupport::Matrix::zeros(1, 5, runtime::Matrix::FLOAT_32)", 
    rows = 1, cols = 5
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImg), cameraMatrix,
     distCoeffs],
    tests = [
        [lenna_bw, memory, camera_matrix, dist_coeffs],
        [lenna, memory, DT, DT]
    ]
)
allocate = package.Option(
    "allocate", "Allocate",  
    [package.Input(srcImg), package.Allocation(dstImg), cameraMatrix,
     distCoeffs],
    tests = [
        [lenna, DT, camera_matrix, dist_coeffs]
    ]
)
undistort = package.Method(
    "undistort", options = [manual, allocate]
)

# undistortPoints
srcPts = package.MatrixArgument(
    "src", "Source", cvtype.Mat(channels = 2), datatype.Float32Matrix(),
    cols = 2
)
dstPts = package.MatrixArgument(
    "dst", "Destination", cvtype.Mat(channels = 2), datatype.Float32Matrix(),
    cols = 2
)
allocate = package.Option(
    "allocate", "Allocate",  
    [package.Input(srcPts), package.Allocation(dstPts), cameraMatrix,
     distCoeffs],
    tests = [
        [points_f32, DT, camera_matrix, dist_coeffs],
        [points_f32, DT, DT, DT]
    ]
)
undistortPoints = package.Method(
    "undistortPoints", options = [allocate]
)

# adaptiveThreshold
descriptions = [
    package.EnumDescription("THRESH_BINARY", "Binary"),
    package.EnumDescription("THRESH_BINARY_INV", "Binary inverted")
]
adaptiveThresholdType = package.EnumParameter(
    "thresholdType", "Threshold type", descriptions = descriptions,
    default = 0
)
descriptions = [
    package.EnumDescription("ADAPTIVE_THRESH_MEAN_C", "Mean of block"),
    package.EnumDescription("ADAPTIVE_THRESH_GAUSSIAN_C",
                            "Weighted sum of block")
]
adaptiveMethod = package.EnumParameter(
    "adaptiveMethod", "Adaptive method", descriptions = descriptions,
    default = 0
)
subtractedC = package.Constant("0")
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImgMono8bit, True), package.Output(dstImg), maxval,
     adaptiveMethod, adaptiveThresholdType, blockSize, subtractedC],
    tests = [
        [lenna_bw, memory, DT, DT, DT, DT, DT],
        [lenna_bw, test.RefData(lenna_bw), 128, 1, 1, 5, DT]
    ]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono8bit, True), package.Allocation(dstImg), maxval,
     adaptiveMethod, adaptiveThresholdType, blockSize, subtractedC],
    tests = [
        [lenna_bw, DT, 200, 1, 0, 9, DT]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.InputOutput(srcImgMono8bit), package.RefInput(dstImg, srcImgMono8bit),
     maxval, adaptiveMethod, adaptiveThresholdType, blockSize, subtractedC],
    tests = [
        [lenna_bw, DT, 80, 0, 1, 7, DT]
    ]
)
adaptiveThreshold = package.Method(
    "adaptiveThreshold", options = [manual, allocate, inPlace]
)

# threshold
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
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImgMono, True), package.Output(dstImg), thresh, maxval,
     thresholdType],
    tests = [
        [lenna_bw, memory, DT, DT, DT],
        [lenna_bw, test.RefData(lenna_bw), 128, DT, 2]
    ]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono), package.Allocation(dstImg), thresh, maxval,
     thresholdType],
    tests = [
        [lenna_bw, DT, DT, DT, 3]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.InputOutput(srcImgMono), package.RefInput(dstImg, srcImgMono),
     thresh, maxval, thresholdType],
    tests = [
        [lenna_bw, DT, DT, DT, 4]
    ]
)
threshold = package.Method(
    "threshold", options = [manual, allocate, inPlace]
)

# distanceTransform
descriptions = [
    package.EnumDescription("DIST_L1", "L1 distance","CV_DIST_L1"),
    package.EnumDescription("DIST_L2", "L2 distance", "CV_DIST_L2"),
    package.EnumDescription("DIST_C", "C", "CV_DIST_C")
]
distanceType = package.EnumParameter(
    "distanceType", "Distance type", descriptions = descriptions,
    default = 0
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImgMono), package.Output(dstImgFloat32), distanceType, 
     maskSize],
    tests = [
        [circle, memory, DT, DT]
    ]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono), package.Allocation(dstImgFloat32), distanceType, 
     maskSize],
    tests = [
        [circle, DT, 2, 0],
        [circle, DT, 1, 1],
        [circle, DT, 0, 2]
    ]
)
distanceTransform = package.Method(
    "distanceTransform", options = [manual, allocate]
)

# floodFill
seedPointX = package.NumericParameter(
    "seedPointX", "Seed point X", cvtype.Int(), datatype.UInt32()
)
seedPointY = package.NumericParameter(
    "seedPointY", "Seed point Y", cvtype.Int(), datatype.UInt32()
)
inPlace = package.Option(
    "inPlace", "In place", 
    [package.InputOutput(srcImgMono), package.Point(seedPointX, seedPointY), newVal],
    tests = [
        [circle, (20, 10), 125.]
    ]
)
floodFill = package.Method(
    "floodFill", options = [inPlace]
)

# integral
initInIntegral = document.Document((
    "unsigned int stride = ({0}->cols() + 1) * runtime::Matrix::valueSize(runtime::Matrix::INT_32);\n"
    "{1}->initializeMatrix({0}->rows() + 1, {0}->cols() + 1, stride, "
    "{1}->data(), runtime::Matrix::INT_32);").format("srcCastedData",
                                                       "dstCastedData"
))
dstImgIntegral = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Matrix(),
    initIn = initInIntegral
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImgMono), package.Output(dstImgIntegral)],
    tests = [
        [lenna_bw, bigMemory]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImgMono), package.Allocation(dstImgIntegral)],
    tests = [
        [circle, DT]
    ]
)
integral = package.Method(
    "integral", options = [manual, allocate]
)

# hist
histMin = package.NumericParameter(
    "histMin", "Minimum", cvtype.Float32(), datatype.Float32(),
    default = 0
)
histMax = package.NumericParameter(
    "histMax", "Maximum", cvtype.Float32(), datatype.Float32(),
    default = 256
)
histSize = package.NumericParameter(
    "histSize", "Number of bins", cvtype.Int(), datatype.UInt32(),
    default = 16
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImgMono), package.Allocation(dstMatrix), histMin, histMax, histSize],
    tests = [
        [circle, DT, 0, 256, 5],
        [lenna_bw, DT, 0, 256, 20]
    ]
)
calcHist = package.Method(
    "calcHist1D", namespace = "", options = [allocate]
)

# findContours
descriptions = [
    package.EnumDescription("RETR_EXTERNAL", "Extreme outer contours", "CV_RETR_EXTERNAL"),
    package.EnumDescription("RETR_LIST", "All contours", "CV_RETR_LIST")
]
findContoursMode = package.EnumParameter(
    "mode", "Mode", descriptions = descriptions,
    default = 0
)
descriptions = [
    package.EnumDescription("CHAIN_APPROX_NONE", "Store all points", "CV_CHAIN_APPROX_NONE"),
    package.EnumDescription("CHAIN_APPROX_SIMPLE", "Compress straight segments", "CV_CHAIN_APPROX_SIMPLE"),
    package.EnumDescription("CHAIN_APPROX_TC89_L1", "Teh-Chin L1", "CV_CHAIN_APPROX_TC89_L1"),
    package.EnumDescription("CHAIN_APPROX_TC89_KCOS", "Teh-Chin Kcos", "CV_CHAIN_APPROX_TC89_KCOS")
]
findContoursMethod = package.EnumParameter(
    "method", "Mode", descriptions = descriptions,
    default = 0
)
dstListOfMatrices = package.Argument(
    "dst", "Destination", cvtype.VectorOfMat(),
    datatype.List(datatype.Int32Matrix())
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono8bit), package.Allocation(dstListOfMatrices),
     findContoursMode, findContoursMethod],
    tests = [
        [contours, DT, DT, DT],
        [contours, DT, DT, 1]
    ]
)
findContours = package.Method(
    "findContours", options = [allocate]
)

# drawContours
ch1 = package.NumericParameter(
    "ch1", "Channel 1", cvtype.Int(), datatype.UInt8(), default = 0
)
ch2 = package.NumericParameter(
    "ch2", "Channel 2", cvtype.Int(), datatype.UInt8(), default = 0
)
ch3 = package.NumericParameter(
    "ch3", "Channel 3", cvtype.Int(), datatype.UInt8(), default = 0
)
thickness = package.NumericParameter(
    "thickness", "Thickness", cvtype.Int(), datatype.Int32(), default = 1
)
listOfContours = package.Argument(
    "contours", "Contours", cvtype.VectorOfMat(),
    datatype.List(datatype.Float32Matrix())
)
drawContoursImage = package.Argument(
    "img", "Image", cvtype.Mat(), datatype.Image()
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.InputOutput(drawContoursImage), package.Input(listOfContours),
     package.Constant(-1), package.Scalar(ch1, ch2, ch3), thickness],
    tests = [
        [lenna_bw, contourList, DT, (255, 0, 0), DT],
        [lenna, contourList, DT, (255, 0, 0), -1]
    ]
)
drawContours = package.Method(
    "drawContours", options = [inPlace]
)

# approxPolyDP
curve = package.MatrixArgument(
    "curve", "Polygon", cvtype.Mat(channels = 2), datatype.Any32BitMatrix(),
    cols = 2
)
outCurve = package.MatrixArgument(
    "outCurve", "Polygon", cvtype.Mat(channels = 2), datatype.Any32BitMatrix(),
    cols = 2
)
epsilon = package.NumericParameter(
    "epsilon", "Maximal error in pixels", cvtype.Float64(), datatype.Float64(),
    default = 10.0, minValue = 0.0
)
closed = package.Parameter(
    "closed", "Curve is closed", cvtype.Bool(), datatype.Bool(), default = False
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(curve), package.Allocation(outCurve), epsilon, closed],
    tests = [
        [contour_1, DT, DT, DT],
        [contour_f32, DT, 5.0, DT]
    ]
)
approxPolyDP = package.Method(
    "approxPolyDP", options = [allocate]
)

# boundingRect
rect = package.MatrixArgument(
    "rect", "Rectangle", cvtype.Rect(), datatype.Int32Matrix(),
    cols = 4, rows = 1
)
points = package.MatrixArgument(
    "points", "Point set", cvtype.Mat(channels = 2), datatype.Any32BitMatrix(),
    cols = 2
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(points), package.ReturnValue(rect)],
    tests = [
        [points_i32, DT],
        [points_f32, DT]
    ]
)
boundingRect = package.Method(
    "boundingRect", options = [allocate]
)

# convexHull
points = package.MatrixArgument(
    "curve", "Input points", cvtype.Mat(channels = 2), datatype.Any32BitMatrix(),
    cols = 2
)
hull = package.MatrixArgument(
    "outCurve", "Convex hull", cvtype.Mat(channels = 2), datatype.Any32BitMatrix(),
    cols = 2
)
epsilon = package.NumericParameter(
    "epsilon", "Maximal error in pixels", cvtype.Float64(), datatype.Float64(),
    default = 10.0, minValue = 0.0
)
clockwise = package.Parameter(
    "clockwise", "Output orientation", cvtype.Bool(), datatype.Bool(), default = False
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(points), package.Allocation(hull), clockwise],
    tests = [
        [non_convex_f32, DT, DT],
        [points_i32, DT, DT]
    ]
)
convexHull = package.Method(
    "convexHull", options = [allocate]
)

# fitEllipse
ellipse = package.MatrixArgument(
    "ellipse", "Bounding box", cvtype.RotatedRect(), datatype.Float32Matrix(),
    cols = 5, rows = 1
)
points = package.MatrixArgument(
    "points", "Point set", cvtype.Mat(channels = 2), datatype.Any32BitMatrix(),
    cols = 2
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(points), package.ReturnValue(ellipse)],
    tests = [
        [points_i32, DT],
        [points_f32, DT]
    ]
)
fitEllipse = package.Method(
    "fitEllipse", options = [allocate]
)

# fitLine
line = package.MatrixArgument(
    "line", "Line (\\u03C1, \\u03B8)", cvtype.Mat(), datatype.Float32Matrix(),
    cols = 3, rows = 1
)
points = package.MatrixArgument(
    "points", "Point set", cvtype.Mat(channels = 2), datatype.Any32BitMatrix(),
    cols = 2
)
descriptions = [
    package.EnumDescription("DIST_L2", "L2", "CV_DIST_L2"),
    package.EnumDescription("DIST_L1", "L1", "CV_DIST_L1"),
    package.EnumDescription("DIST_L12", "L12", "CV_DIST_L12"),
    package.EnumDescription("DIST_FAIR", "Fair", "CV_DIST_FAIR"),
    package.EnumDescription("DIST_WELSCH", "Welsch", "CV_DIST_WELSCH"),
    package.EnumDescription("DIST_HUBER", "Huber", "CV_DIST_HUBER")
]
distType = package.EnumParameter(
    "distType", "Distance type", descriptions = descriptions,
    default = 0
)
param  = package.Constant("0")
reps  = package.NumericParameter(
    "reps", "Accuracy of \\u03C1", cvtype.Float64(), datatype.Float64(),
    default = 0.01, minValue = 0.0
)
aeps  = package.NumericParameter(
    "aeps", "Accuracy of \\u03B8", cvtype.Float64(), datatype.Float64(),
    default = 0.01, minValue = 0.0
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(points), package.Allocation(line), distType, param, reps,
     aeps],
    tests = [
        [points_i32, DT, DT, DT, DT],
        [points_f32, DT, DT, DT, DT]
    ]
)
fitLine = package.Method(
    "fitLine", namespace = "", options = [allocate]
)

# minAreaRect
rect = package.MatrixArgument(
    "rect", "Rectangle", cvtype.RotatedRect(), datatype.Float32Matrix(),
    cols = 5, rows = 1
)
points = package.MatrixArgument(
    "points", "Point set", cvtype.Mat(channels = 2), datatype.Any32BitMatrix(),
    cols = 2
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(points), package.ReturnValue(rect)],
    tests = [
        [points_i32, DT],
        [points_f32, DT]
    ]
)
minAreaRect = package.Method(
    "minAreaRect", options = [allocate]
)

# minEnclosingCircle
circle = package.MatrixArgument(
    "circle", "Circle", cvtype.Mat(), datatype.Float32Matrix(),
    cols = 3, rows = 1
)
points = package.MatrixArgument(
    "points", "Point set", cvtype.Mat(channels = 2), datatype.Any32BitMatrix(),
    cols = 2
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(points), package.Allocation(circle)],
    tests = [
        [points_i32, DT],
        [points_f32, DT]
    ]
)
minEnclosingCircle = package.Method(
    "minEnclosingCircle", namespace = "", options = [allocate]
)

# Canny
threshold1 = package.NumericParameter(
    "threshold1", "Threshold 1", cvtype.Float64(), datatype.Float64(),
    default = 64
)
threshold2 = package.NumericParameter(
    "threshold2", "Threshold 2", cvtype.Float64(), datatype.Float64(),
    default = 128
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImgMono, True), package.InputOutput(dstImg), threshold1,
     threshold2],
    tests = [
        [lenna_bw, memory, DT, DT],
        [lenna_bw, test.RefData(lenna_bw), 64, 128]
    ]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono), package.Allocation(dstImg), threshold1,
     threshold2],
    tests = [
        [lenna_bw, DT, DT, DT]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.InputOutput(srcImgMono), package.RefInput(dstImg, srcImgMono),  threshold1,
     threshold2],
    tests = [
        [lenna_bw, DT, DT, DT]
    ]
)
canny = package.Method(
    "Canny", options = [manual, allocate, inPlace]
)

# cornerHarris
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImgMono, False), package.Output(dstImgFloat32), blockSize,
     ksize, harrisK],
    tests = [
        [lenna_bw, bigMemory, DT, DT, DT],
    ]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono), package.Allocation(dstImgFloat32), blockSize,
     ksize, harrisK],
    tests = [
        [lenna_bw, DT, DT, DT, DT]
    ]
)
cornerHarris = package.Method(
    "cornerHarris", options = [manual, allocate]
)

# cornerMinEigenVal
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImgMono, False), package.Output(dstImgFloat32), blockSize,
     ksize],
    tests = [
        [lenna_bw, bigMemory, DT, DT, DT],
    ]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono), package.Allocation(dstImgFloat32), blockSize,
     ksize],
    tests = [
        [lenna_bw, DT, DT, DT, DT]
    ]
)
cornerMinEigenVal = package.Method(
    "cornerMinEigenVal", options = [manual, allocate]
)

# cornerSubPix
defaultTermCriteria = package.Constant(
    "cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, -1, -1)"
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.Input(srcImgMono), package.InputOutput(pointMatrix),
     package.Size(winSizeX, winSizeY), defaultSize, defaultTermCriteria],
    tests = [
        [cornerImage, cornerCoordinates, (DT, DT)]
    ]
)
cornerSubPix = package.Method(
    "cornerSubPix", options = [inPlace]
)

# goodFeaturesToTrack
useHarrisDetector = package.Parameter(
    "useHarrisDetector", "Use Harris detector", cvtype.Bool(), datatype.Bool(),
    default = False
)
maxCorners = package.NumericParameter(
    "maxCorners", "Maximum number of corners", cvtype.Int(), datatype.UInt32(),
    default = 10
)
qualityLevel = package.NumericParameter(
    "qualityLevel", "Minimal accepted quality", 
    cvtype.Float64(), datatype.Float64(), default = 0.01
)
minDistance = package.NumericParameter(
    "minDistance", "Minimal distance between corners", 
    cvtype.Float64(), datatype.Float64(), default = 1.0
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono), package.Allocation(pointMatrix), maxCorners,
     qualityLevel, minDistance, noArray, blockSize, useHarrisDetector, harrisK],
    tests = [
        [cornerImage, DT, DT, DT, DT, DT, DT]
    ]
)
goodFeaturesToTrack = package.Method(
    "goodFeaturesToTrack", options = [allocate]
)

# HoughLinesP
dstMatrixLineSegments = package.MatrixArgument(
    "dst", "Destination", cvtype.Mat(), datatype.Matrix(), cols = 4
)
rho = package.NumericParameter(
    "rho", "Distance resolution", cvtype.Float64(), datatype.Float64(),
    default = 1.0
)
theta = package.NumericParameter(
    "theta", "Angle resolution", cvtype.Float64(), datatype.Float64(),
    default = math.pi / 180
)
lineSegmentsPostCall = document.Document(
    "dstCvData = dstCvData.reshape(1, dstCvData.cols);"
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono), package.Allocation(dstMatrixLineSegments), rho, theta,
     accumulatorThreshold, minLineLength, maxLineGap],
    tests = [
        [edges, DT, DT, DT, DT, DT, DT]
    ], 
    postCall = lineSegmentsPostCall
)
houghLinesP = package.Method(
    "HoughLinesP", options = [allocate]
)

# preCornerDetect
descriptions = [
    package.EnumDescription("BORDER_DEFAULT", "Default"),
    package.EnumDescription("BORDER_CONSTANT", "Constant"),
    package.EnumDescription("BORDER_REFLECT", "Reflect"),
    package.EnumDescription("BORDER_REPLICATE", "Replicate"),
]
borderType = package.EnumParameter(
    "borderType", "Border type", descriptions = descriptions,
    default = "BORDER_DEFAULT"
)
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImgMono8bit), package.Output(dstImgFloat32), sobelKsize,
    borderType],
    tests = [
        [lenna_bw, bigMemory, DT, DT]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImgMono8bit), package.Allocation(dstImgFloat32),
     sobelKsize, borderType],
    tests = [
        [lenna_bw, DT, 5, 2]
    ]
)
preCornerDetect = package.Method(
    "preCornerDetect", options = [manual, allocate]
)

imgproc = package.Package(
    "cvimgproc", 0, 0, 1,
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
        adaptiveThreshold,
        threshold,
        warpAffine,
        warpPerspective,
        undistort,
        undistortPoints,
        distanceTransform,
        floodFill,
        integral,
        calcHist,
        findContours,
        drawContours,
        approxPolyDP,
        boundingRect,
        convexHull,
        fitEllipse,
        fitLine,
        minAreaRect,
        minEnclosingCircle,
        canny,
        cornerHarris,
        cornerMinEigenVal,
        cornerSubPix,
        goodFeaturesToTrack,
        houghLinesP,
        preCornerDetect
    ],
    functions = [
        calcHistWrapper,
        minEnclosingCircleWrapper,
        fitLineWrapper
    ],
    testFiles = [
        "lenna.jpg",
        "circle.png",
        "affine.npy",
        "perspective.npy",
        "camera_matrix.npy",
        "dist_coeffs.npy",
        "edges.png",
        "contours.png",
        "corners.png",
        "corners.npy",
        "contour_1.npy",
        "contour_2.npy",
        "contour_f64.npy",
        "contour_f32.npy",
        "non_convex_f32.npy",
        "points_i32.npy",
        "points_f32.npy",
        "points_f64.npy"
    ]
)

package = imgproc

if __name__ == '__main__':
    if len(sys.argv) > 1:
        for arg in sys.argv[1:]:
            generator.generateMethodFiles(package, globals()[arg])
    else:
        generator.generatePackageFiles(package) 
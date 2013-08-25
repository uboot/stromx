# -*- coding: utf-8 -*-
"""
Created on Mon Apr  1 18:19:38 2013

@author: matz
"""

import cvcommon
import cvtype
import datatype
import document
import generator
import package
import test

# abbreviations
dt = test.Default()

# checkMatrixData
dcl = document.Document()
dclIncludes = ["<stromx/runtime/Matrix.h>",
               "<stromx/runtime/MatrixDescription.h>",
               "<stromx/runtime/OperatorKernel.h>"]
dcl.text(
"""
void checkMatrixData(const stromx::runtime::Matrix & value,
                     const stromx::runtime::MatrixDescription* desc,
                     const stromx::runtime::OperatorKernel& op);
""")
dtnIncludes = ["<sstream>"]
dtn = document.Document()              
dtn.text(
"""
void checkMatrixData(const stromx::runtime::Matrix & value,
                     const stromx::runtime::MatrixDescription* desc,
                     const stromx::runtime::OperatorKernel& op)
{
    if(desc->rows() && value.rows() != desc->rows())
    {
        std::ostringstream str;
        str << desc->rows();
        throw runtime::InputError(desc->id(), op, "Number of matrix rows must be " + str.str() + " .");
    }
    
    if(desc->cols() && value.cols() != desc->cols())
    {
        std::ostringstream str;
        str << desc->cols();
        throw runtime::InputError(desc->id(), op, "Number of matrix columns must be " + str.str() + " .");
    }
}
""")
checkMatrixData = package.Function(dcl, dclIncludes, dtn, dtnIncludes)

# checkMatrixValue
dcl = document.Document()
dclIncludes = ["<stromx/runtime/Matrix.h>",
               "<stromx/runtime/MatrixParameter.h>",
               "<stromx/runtime/OperatorKernel.h>"]
dcl.text(
"""
void checkMatrixValue(const stromx::runtime::Matrix & value,
                      const stromx::runtime::MatrixParameter* param,
                      const stromx::runtime::OperatorKernel& op);
""")
dtnIncludes = ["<sstream>"]
dtn = document.Document()              
dtn.text(
"""
void checkMatrixValue(const stromx::runtime::Matrix & value,
                      const stromx::runtime::MatrixParameter* param,
                      const stromx::runtime::OperatorKernel& op)
{
    if(param->rows() && value.rows() != param->rows())
    {
        std::ostringstream str;
        str << param->rows();
        throw runtime::WrongParameterValue(*param, op, "Number of matrix rows must be " + str.str() + " .");
    }
    
    if(param->cols() && value.cols() != param->cols())
    {
        std::ostringstream str;
        str << param->cols();
        throw runtime::WrongParameterValue(*param, op, "Number of matrix columns must be " + str.str() + " .");
    }
}
""")
checkMatrixValue = package.Function(dcl, dclIncludes, dtn, dtnIncludes)

# compute histogram
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
calcHist1D = package.Function(dcl, dclIncludes, dtn, dtnIncludes)

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
initInIntegral = document.Document((
    "unsigned int stride = ({0}->cols() + 1) * runtime::Matrix::valueSize(runtime::Matrix::INT_32);\n"
    "{1}->initializeMatrix({0}->rows() + 1, {0}->cols() + 1, stride, "
    "{1}->data(), runtime::Matrix::INT_32);").format("srcCastedData",
                                                       "dstCastedData"
))

# arguments
srcImg = package.Argument(
    "src", "Source", cvtype.Mat(), datatype.Image()
)
srcImgMono = package.Argument(
    "src", "Source", cvtype.Mat(),
    datatype.Image("runtime::DataVariant::MONO_IMAGE")
)
srcImgMono8bit = package.Argument(
    "src", "Source", cvtype.Mat(),
    datatype.Image("runtime::DataVariant::MONO_8_IMAGE")
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
    "dst", "Destination", cvtype.Mat(), datatype.Matrix(),
    initIn = initInFloat32
)
dstImgIntegral = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Matrix(),
    initIn = initInIntegral
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
    "fx", "Scale X", cvtype.Float64(), datatype.Float64(), default = 1.0
)
fy = package.NumericParameter(
    "fy", "Scale Y", cvtype.Float64(), datatype.Float64(), default = 1.0
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
    package.EnumDescription(" ADAPTIVE_THRESH_GAUSSIAN_C",
                            "Weighted sum of block")
]
adaptiveMethod = package.EnumParameter(
    "adaptiveMethod", "Adaptive method", descriptions = descriptions,
    default = 0
)
blockSize = package.NumericParameter(
    "blockSize", "Block size", cvtype.Int(), datatype.UInt32(),
    default = 3, minValue = 1, rules = [package.OddRule()]
)
subtractedC = package.Constant("0")
affineM = package.MatrixParameter(
    "affineM", "2x3 affine transformation", datatype.FloatMatrix(),
    default = "cvsupport::Matrix::eye(2, 3, runtime::Matrix::FLOAT_32)", rows = 2,
    cols = 3
)
perspectiveM = package.MatrixParameter(
    "affineM", "3x3 perspective transformation", datatype.FloatMatrix(),
    default = "cvsupport::Matrix::eye(3, 3, runtime::Matrix::FLOAT_32)", rows = 3,
    cols = 3
)
cameraMatrix = package.MatrixParameter(
    "cameraMatrix", "3x3 camera matrix", datatype.FloatMatrix(),
    default = "cvsupport::Matrix::eye(3, 3, runtime::Matrix::FLOAT_32)", rows = 3,
    cols = 3
)
distCoeffs = package.MatrixParameter(
    "distCoeffs", "4x1 distortion coefficients", datatype.FloatMatrix(),
    default = "cvsupport::Matrix::zeros(4, 1, runtime::Matrix::FLOAT_32)", 
    rows = 4, cols = 1
)
srcPts = package.MatrixArgument(
    "src", "Source", cvtype.Mat(channels = 2), datatype.FloatMatrix(),
    cols = 2
)
dstPts = package.MatrixArgument(
    "dst", "Destination", cvtype.Mat(channels = 2), datatype.FloatMatrix()
)
descriptions = [
    package.EnumDescription("DIST_L1", "L1 distance","CV_DIST_L1"),
    package.EnumDescription("DIST_L2", "L2 distance", "CV_DIST_L2"),
    package.EnumDescription("DIST_C", "C", "CV_DIST_C")
]
distanceType = package.EnumParameter(
    "distanceType", "Distance type", descriptions = descriptions,
    default = 0
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
threshold1 = package.NumericParameter(
        "threshold1", "Threshold 1", cvtype.Float64(), datatype.Float64(),
        default = 64
)
threshold2 = package.NumericParameter(
        "threshold2", "Threshold 2", cvtype.Float64(), datatype.Float64(),
        default = 128
)
harrisK = package.NumericParameter(
        "k", "Harris parameter", cvtype.Float64(), datatype.Float64(),
        default = 1
)

# test data
lenna = test.ImageFile("lenna.jpg")
lenna_bw = test.ImageFile("lenna.jpg", grayscale = True)
affine_transformation = test.MatrixFile("affine.npy")
perspective_transformation = test.MatrixFile("perspective.npy")
camera_matrix = test.MatrixFile("camera_matrix.npy")
dist_coeffs = test.MatrixFile("dist_coeffs.npy")
points_2d = test.MatrixFile("points_2d.npy")
memory = test.ImageBuffer(1000000)
bigMemory = test.ImageBuffer(10000000) 
circle = test.ImageFile("circle.png", grayscale = True)

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
    [package.InputOutput(srcImg), package.RefInput(dstImg, srcImg),
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
    [package.InputOutput(srcImg), package.RefInput(dstImg, srcImg), ddepthDefault,
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
    [package.InputOutput(srcImg), package.RefInput(dstImg, srcImg), kernel,
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
    [package.InputOutput(srcImg), package.RefInput(dstImg, srcImg),
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
    [package.InputOutput(srcImg), package.RefInput(dstImg, srcImg), ksize],
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
    [package.InputOutput(srcImg), package.RefInput(dstImg, srcImg), op, kernel,
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

# warpAffine
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImgDsize), affineM, 
     package.Size(dsizex, dsizey)],
    tests = [
        [lenna_bw, memory, affine_transformation, (400, 500)],
        [lenna, memory, dt, (400, 500)]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImgDsize), affineM, 
     package.Size(dsizex, dsizey)],
    tests = [
        [lenna, dt, affine_transformation, (400, 500)]
    ]
)
warpAffine = package.Method(
    "warpAffine", options = [manual, allocate]
)

# warpPerspective
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImgDsize), perspectiveM, 
     package.Size(dsizex, dsizey)],
    tests = [
        [lenna_bw, memory, perspective_transformation, (400, 500)],
        [lenna, memory, dt, (400, 500)]
    ]
)
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImg), package.Allocation(dstImgDsize), perspectiveM, 
     package.Size(dsizex, dsizey)],
    tests = [
        [lenna, dt, perspective_transformation, (400, 500)]
    ]
)
warpPerspective = package.Method(
    "warpPerspective", options = [manual, allocate]
)

# undistort
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg), package.Output(dstImg), cameraMatrix,
     distCoeffs],
    tests = [
        [lenna_bw, memory, camera_matrix, dist_coeffs],
        [lenna, memory, dt, dt]
    ]
)
allocate = package.Option(
    "allocate", "Allocate",  
    [package.Input(srcImg), package.Allocation(dstImg), cameraMatrix,
     distCoeffs],
    tests = [
        [lenna, dt, camera_matrix, dist_coeffs]
    ]
)
undistort = package.Method(
    "undistort", options = [manual, allocate]
)

# undistortPoints
allocate = package.Option(
    "allocate", "Allocate",  
    [package.Input(srcPts), package.Allocation(dstPts), cameraMatrix,
     distCoeffs],
    tests = [
        [points_2d, dt, camera_matrix, dist_coeffs],
        [points_2d, dt, dt, dt]
    ]
)
undistortPoints = package.Method(
    "undistortPoints", options = [allocate]
)

# adaptiveThreshold
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImgMono8bit, True), package.Output(dstImg), maxval,
     adaptiveMethod, thresholdType, blockSize, subtractedC],
    tests = [
        [lenna_bw, memory, dt, dt, dt, dt, dt],
        [lenna_bw, test.RefData(lenna_bw), 128, 1, 1, 5, dt]
    ]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono8bit, True), package.Allocation(dstImg), maxval,
     adaptiveMethod, thresholdType, blockSize, subtractedC],
    tests = [
        [lenna_bw, dt, 200, 1, 0, 9, dt]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.InputOutput(srcImgMono8bit), package.RefInput(dstImg, srcImgMono8bit),
     maxval, adaptiveMethod, thresholdType, blockSize, subtractedC],
    tests = [
        [lenna_bw, dt, 80, 0, 1, 7, dt]
    ]
)
adaptiveThreshold = package.Method(
    "adaptiveThreshold", options = [manual, allocate, inPlace]
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
    [package.InputOutput(srcImgMono), package.RefInput(dstImg, srcImgMono), thresh, maxval,
     thresholdType],
    tests = [
        [lenna_bw, dt, dt, dt, 4]
    ]
)
threshold = package.Method(
    "threshold", options = [manual, allocate, inPlace]
)

# distanceTransform
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImgMono), package.Output(dstImgFloat32), distanceType, 
     maskSize],
    tests = [
        [circle, memory, dt, dt]
    ]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono), package.Allocation(dstImgFloat32), distanceType, 
     maskSize],
    tests = [
        [circle, dt, 2, 0],
        [circle, dt, 1, 1],
        [circle, dt, 0, 2]
    ]
)
distanceTransform = package.Method(
    "distanceTransform", options = [manual, allocate]
)

# floodFill
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
        [circle, dt]
    ]
)
integral = package.Method(
    "integral", options = [manual, allocate]
)

# hist
allocate = package.Option(
    "allocate", "Allocate", 
    [package.Input(srcImgMono), package.Allocation(dstMatrix), histMin, histMax, histSize],
    tests = [
        [circle, dt, 0, 256, 5],
        [lenna_bw, dt, 0, 256, 20]
    ]
)
calcHist = package.Method(
    "calcHist1D", namespace = "", options = [allocate]
)

# Canny
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImgMono, True), package.Output(dstImg), threshold1,
     threshold2],
    tests = [
        [lenna_bw, memory, dt, dt],
        [lenna_bw, test.RefData(lenna_bw), 64, 128]
    ]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono), package.Allocation(dstImg), threshold1,
     threshold2],
    tests = [
        [lenna_bw, dt, dt, dt]
    ]
)
inPlace = package.Option(
    "inPlace", "In place",
    [package.InputOutput(srcImgMono), package.RefInput(dstImg, srcImgMono),  threshold1,
     threshold2],
    tests = [
        [lenna_bw, dt, dt, dt]
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
        [lenna_bw, bigMemory, dt, dt, dt],
    ]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono), package.Allocation(dstImgFloat32), blockSize,
     ksize, harrisK],
    tests = [
        [lenna_bw, dt, dt, dt, dt]
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
        [lenna_bw, bigMemory, dt, dt, dt],
    ]
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono), package.Allocation(dstImgFloat32), blockSize,
     ksize],
    tests = [
        [lenna_bw, dt, dt, dt, dt]
    ]
)
cornerMinEigenVal = package.Method(
    "cornerMinEigenVal", options = [manual, allocate]
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
        canny,
        cornerHarris,
        cornerMinEigenVal
    ],
    functions = [
        cvcommon.checkEnumValue,
        cvcommon.checkNumericValue,
        checkMatrixData,
        checkMatrixValue,
        calcHist1D
    ],
    testFiles = [
        "lenna.jpg",
        "circle.png",
        "affine.npy",
        "perspective.npy",
        "camera_matrix.npy",
        "dist_coeffs.npy",
        "points_2d.npy"
    ]
)

generator.generateMethodFiles(imgproc, cornerHarris)
generator.generatePackageFiles(imgproc) 
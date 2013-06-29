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
    "{1}->data(), {0}->pixelType());").format("src1CastedData", "dstCastedData"
))
initOutCopy = document.Document((
    "{1}->initializeImage({1}->width(), {1}->height(), {1}->stride(), "
    "{1}->data(), {0}->pixelType());").format("src1CastedData", "dstCastedData"
))
initInDdepth = document.Document((
    "runtime::Image::PixelType pixelType = cvsupport::computeOutPixelType("
    "convertDdepth(m_ddepth), src1CastedData->pixelType());\n"
    "unsigned int stride = runtime::Image::pixelSize(pixelType) * "
    "src1CastedData->width();\n"
    "{1}->initializeImage({0}->width(), {0}->height(), stride, "
    "{1}->data(), pixelType);").format("src1CastedData", "dstCastedData"
))
initOutDdepth = document.Document((
    "runtime::Image::PixelType pixelType = cvsupport::computeOutPixelType("
    "convertDdepth(m_ddepth), src1CastedData->pixelType());\n"
    "unsigned int stride = runtime::Image::pixelSize(pixelType) * "
    "src1CastedData->width();\n"
    "{1}->initializeImage({1}->width(), {1}->height(), stride, "
    "{1}->data(), pixelType);").format("src1CastedData", "dstCastedData"
))
pixelTypeCheck = document.Document(
"""
if(src1CastedData->numChannels() != src2CastedData->numChannels())
    throw runtime::InputError(SRC_1, *this, "Input images must have the same number of channels.");
    
if(m_ddepth == SAME && (src1CastedData->depth() != src2CastedData->depth()))
    throw runtime::InputError(SRC_1, *this, "Input images must have the same depth if the destination depth is not explicitely given.");
""")

# arguments
srcImg1 = package.Argument(
    "src1", "Source 1", cvtype.Mat(), datatype.Image()
)
srcImg2 = package.Argument(
    "src2", "Source 2", cvtype.Mat(), datatype.Image()
)
dstImg = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(), initIn = initInDdepth,
    initOut = initOutDdepth
)
descriptions = [
    package.EnumDescription("SAME", "Same as inputs", -1),
    package.EnumDescription("DEPTH_8_BIT", "8-bit", "CV_8U"),
    package.EnumDescription("DEPTH_16_BIT", "16-bit", "CV_16U")
]
ddepth = package.EnumParameter(
    "ddepth", "Destination depth", descriptions = descriptions,
    default = 0
)
noArray = package.Constant("cv::noArray()")

# test data
lenna = test.ImageFile("lenna.jpg")
barbara = test.ImageFile("barbara.jpg")
lenna_16bit = test.ImageFile("lenna.jpg", deepColor = True)
barbara_16bit = test.ImageFile("barbara.jpg", deepColor = True)
barbara = test.ImageFile("barbara.jpg")
lenna_bw = test.ImageFile("lenna.jpg", grayscale = True)
barbara_bw = test.ImageFile("barbara.jpg", grayscale = True)
memory = test.ImageBuffer(1000000)

# add
manual = package.Option(
    "manual", "Manual", 
    [package.Input(srcImg1), package.Input(srcImg2), package.Output(dstImg),
     noArray, ddepth],
    tests = [
        [lenna, barbara, memory, dt, dt],
        [lenna_bw, barbara_bw, memory, dt, dt],
        [lenna_16bit, barbara, memory, dt, 1]
    ],
    inputCheck = pixelTypeCheck
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImg1), package.Input(srcImg2),
     package.Allocation(dstImg), noArray, ddepth],
    tests = [
        [lenna_16bit, barbara_16bit, dt, dt, dt],
        [lenna_16bit, barbara, dt, dt, 2]
    ],
    inputCheck = pixelTypeCheck
)
add = package.Method(
    "add", options = [manual, allocate]
)

core = package.Package(
    "cvcore", 0, 0, 1,
    methods = [
        add
    ],
    functions = [
        checkEnumValue,
        checkNumericValue
    ],
    testFiles = [
        "barbara.jpg",
        "lenna.jpg"
    ]
)

generator.generatePackageFiles(core) 
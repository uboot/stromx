# -*- coding: utf-8 -*-
"""
Created on Mon Apr  1 18:19:38 2013

@author: matz
"""

import cvtype
import datatype
import generator
import package

p = package.Package("imgproc", 0, 0, 1)
p.name = "OpenCV image processing"

# medianBlur

m = package.Method("medianBlur")
arg1 = package.Argument(
    "src", "Source", cvtype.Mat(), datatype.Image()
)
initIn = [(
    "{1}->initializeImage({0}->width(), {0}->height(), {0}->stride(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData"
)]
initOut = [(
    "{1}->initializeImage({1}->width(), {1}->height(), {1}->stride(), "
    "{1}->data(), {0}->pixelType());").format("srcCastedData", "dstCastedData"
)]
arg2 = package.Argument(
    "dst", "Destination", cvtype.Mat(), datatype.Image(), initIn = initIn,
    initOut = initOut
)
arg3 = package.NumericParameter(
    "ksize", "Kernel size", cvtype.Int(), datatype.UInt32(), minValue = 1,
    step = 2, default = 1, rules = [package.OddRule()]
)
opt = package.Option("manual", "Manual", 
                     [package.Input(arg1, True), package.Output(arg2), arg3])
m.options.append(opt)
opt = package.Option("allocate", "Allocate", 
                     [package.Input(arg1), package.Allocation(arg2), arg3])
m.options.append(opt)
opt = package.Option("inPlace", "In place",
                     [package.Output(arg1), package.RefInput(arg2, arg1), arg3])
m.options.append(opt)
p.methods.append(m)

### resize

m = package.Method("resize")
arg1 = package.Argument("src", "Source", cvtype.Mat(), datatype.Image())
arg2 = package.Argument("dst", "Destination", cvtype.Mat(), datatype.Image())
initIn = []
initIn.append(
    "int width = int(m_dsizex) ? int(m_dsizex) : "
    "srcCastedData->width() * double(m_fx);")
initIn.append(
    "int height = int(m_dsizey) ? int(m_dsizey) : "
    "srcCastedData->height() * double(m_fy);")
    
initIn.append((
    "{1}->initializeImage(width, height, "
    "width * {0}->pixelSize(), "
    "{1}->data(), {0}->pixelType());"
    ).format("srcCastedData", "dstCastedData"))
arg2.initIn.extend(initIn)
initOut = ("{1}->initializeImage({1}->width(), {1}->height(), "
           "{1}->stride(), {1}->data(), {0}->pixelType());"
          ).format("srcCastedData", "dstCastedData")
arg2.initOut.append(initOut)
arg3x = package.NumericParameter("dsizex", "Size X", cvtype.Int(), datatype.UInt32())
arg3y = package.NumericParameter("dsizey", "Size Y", cvtype.Int(), datatype.UInt32())
arg4 = package.NumericParameter("fx", "Scale X", cvtype.Double(), datatype.Double())
arg5 = package.NumericParameter("fy", "Scale Y", cvtype.Double(), datatype.Double())
arg4.default = 1.0
arg5.default = 1.0
arg6 = package.EnumParameter("interpolation", "Interpolation")
arg6.descriptions = [package.EnumDescription("INTER_NEAREST", "Nearest neighbour"),
                     package.EnumDescription("INTER_LINEAR", "Bilinear")]
arg6.default = 1

opt = package.Option("manual", "Manual")
opt.args.extend([package.Input(arg1), package.Output(arg2),
                 package.Size(arg3x, arg3y), arg4, arg5, arg6])
m.options.append(opt)
opt = package.Option("allocate", "Allocate")
opt.args.extend([package.Input(arg1), package.Allocation(arg2),
                 package.Size(arg3x, arg3y), arg4, arg5, arg6])
m.options.append(opt)
p.methods.append(m)

generator.generatePackageFiles(p) 
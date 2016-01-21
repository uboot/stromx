# -*- coding: utf-8 -*-

import sys

import cvtype
import datatype
import generator
import package
import test

# abbreviations
DT = test.Default()

# cascadeClassifier
contours = test.ImageFile("contours.png", grayscale = True)
dstListOfMatrices = package.Argument(
    "dst", "Destination", cvtype.VectorOfMat(),
    datatype.List(datatype.Int32Matrix())
)
srcImgMono8bit = package.Argument(
    "src", "Source", cvtype.Mat(),
    datatype.Image("runtime::Variant::MONO_8_IMAGE")
)
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(srcImgMono8bit), package.Allocation(dstListOfMatrices)],
    tests = [
        [contours, DT],
        [contours, DT]
    ]
)
cascadeClassifier = package.Method(
    "cascadeClassifier", options = [allocate]
)

objdetect = package.Package(
    "cvobjdetect", 0, 1, 0,
    methods = [
        #cascadeClassifier
    ]
)

package = objdetect

if __name__ == '__main__':
    if len(sys.argv) > 1:
        for arg in sys.argv[1:]:
            generator.generateMethodFiles(package, globals()[arg])
    else:
        generator.generatePackageFiles(package) 
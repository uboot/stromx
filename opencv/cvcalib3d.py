# -*- coding: utf-8 -*-
"""
Created on Mon Apr  1 18:19:38 2013

@author: matz
"""

import sys

import cvtype
import datatype
import document
import generator
import package
import test

# default test data
DT = test.Default()

# findChessboardCorners
image = package.Argument(
    "image", "Image", cvtype.Mat(), datatype.Image()
)
sizex = package.NumericParameter(
    "patternSizeX", "Pattern size X", cvtype.Int(), datatype.UInt32(), default = 7,
    minValue = 1
)
sizey = package.NumericParameter(
    "patternSizeY", "Pattern size Y", cvtype.Int(), datatype.UInt32(), default = 5,
    minValue = 1
)
size = package.Size(sizex, sizey)
corners = package.MatrixArgument(
    "corners", "Corners", cvtype.Mat(channels = 2), datatype.Float32Matrix(),
    cols = 2
)
chessboard_bw = test.ImageFile("chess.png", grayscale = True)
chessboard_color = test.ImageFile("chess.png", )
allocate = package.Option(
    "allocate", "Allocate",
    [package.Input(image), size, package.Allocation(corners)],
    tests = [
        [chessboard_bw, DT, DT, DT],
        [chessboard_color, DT, DT, DT]
    ]
)
findChessboardCorners = package.Method(
    "findChessboardCorners", options = [allocate]
)

calib3d = package.Package(
    "cvcalib3d", 0, 0, 1,
    methods = [
        findChessboardCorners
    ],
    testFiles = [
        "chess.png" # 8-bit grayscale
    ]
)

package = calib3d

if __name__ == '__main__':
    if len(sys.argv) > 1:
        for arg in sys.argv[1:]:
            generator.generateMethodFiles(package, globals()[arg])
    else:
        generator.generatePackageFiles(package) 
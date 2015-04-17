# -*- coding: utf-8 -*-

import stromx.runtime
import stromx.cvsupport

# stromx.runtime.Image
image = stromx.cvsupport.Image(20, 10, stromx.runtime.Image.PixelType.MONO_8)
casted = stromx.runtime.Image.data_cast(image)
print casted.width()

container = stromx.runtime.DataContainer(image)
with stromx.runtime.ReadAccess(container) as access:
    casted = stromx.runtime.Image.data_cast(access.get())
    print casted.width()

primitive = stromx.runtime.UInt32(5)
casted = stromx.runtime.Image.data_cast(primitive)
assert(casted == None)

# stromx.runtime.Matrix
matrix = stromx.cvsupport.Matrix(20, 10, stromx.runtime.Matrix.ValueType.INT_32)
casted = stromx.runtime.Matrix.data_cast(matrix)
print casted.cols()

container = stromx.runtime.DataContainer(matrix)
with stromx.runtime.ReadAccess(container) as access:
    casted = stromx.runtime.Matrix.data_cast(access.get())
    print casted.cols()

primitive = stromx.runtime.UInt32(5)
casted = stromx.runtime.Matrix.data_cast(primitive)
assert(casted == None)
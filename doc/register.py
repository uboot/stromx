# -*- coding: utf-8 -*-

import stromx.runtime
import ctypes

lib = ctypes.CDLL("/home/matz/Projects/build/uboots-stream/stromx/example/libstromx_example.so")

f = stromx.runtime.Factory()
ptr = ctypes.c_void_p(f.ptr())

v = lib.stromxRegisterExample(ptr)
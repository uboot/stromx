# -*- coding: utf-8 -*-

import stromx.runtime

s = stromx.runtime.Stream()
try:
    s.pause()
except stromx.runtime.Exception as e:
    print str(e)
    
try:
    s.pause()
except stromx.runtime.WrongState as e:
    print str(e)

try:
    s.pause()
except stromx.runtime.WrongId as e:
    print str(e), 0
except stromx.runtime.WrongState as e:
    print str(e), 1
except stromx.runtime.Exception as e:
    print str(e), 2
except Exception as e:
    print str(e), 3

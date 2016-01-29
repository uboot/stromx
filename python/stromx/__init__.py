# -*- coding: utf-8 -*-

#
#  Copyright 2011 Matthias Fuchs
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

import ctypes

def register(package, factory):
    libName = 'libstromx_{0}.so'.format(package)
    try:
        lib = ctypes.CDLL(libName)
    except OSError:
        print 'Failed to load {0}'.format(libName)
        return 1

    functionName = 'stromxRegister{0}'.format(package.capitalize())
    try:
        function = lib[functionName]
    except AttributeError:
        print '{0}() is not defined'.format(functionName)
        return 1
        
    if function(factory._this()):
        print 'Failed to call {0}'.format(functionName)
        return 1
    
    return 0
        
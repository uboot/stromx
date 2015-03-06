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

from stromx import runtime, cvimgproc, cvsupport

factory = runtime.Factory()

runtime.register(factory)
cvimgproc.register(factory)
cvsupport.register(factory)

stream = runtime.XmlReader().readStream("camera.xml", factory)

stream.start()

camera = stream.operators()[0]
canny = stream.operators()[2]

for i in range(5):
    with canny.getOutputData(1) as data, runtime.ReadAccess(data) as image:
        print "Received image {0}x{1}".format(image.get().width(),
                                              image.get().height())
    
    canny.clearOutputData(1)
    camera.clearOutputData(1)

stream.stop()
stream.join()

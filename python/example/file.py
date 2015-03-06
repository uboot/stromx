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

from stromx import runtime

factory = runtime.Factory()

runtime.register(factory)

stream = runtime.XmlReader().readStream("file.xml", factory)

stream.start()

timer = stream.operators()[1]

for i in range(5):
    data = timer.getOutputData(0)
    count = runtime.ReadAccess(data)
    with runtime.ReadAccess(data) as count:
        print "Received {0}".format(count.get().get())
    
    timer.clearOutputData(0)

stream.stop()
stream.join()

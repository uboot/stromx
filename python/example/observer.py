# -*- coding: utf-8 -*-

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

from stromx import *

class MyObserver(core.ConnectorObserver):
    def observe(self, connector, data):
        print "test"
#        print connector.type()
#        print connector.id()
#        if not data.empty():
#            access = core.ReadAccess(data)
#            print access.get()
#            del access

raw_input()

factory = core.Factory()

core.registerCore(factory)
base.registerBase(factory)

stream = core.XmlReader().read("file.xml", factory)

observer = MyObserver()

source = stream.operators()[0]
source.addObserver(observer)

timer = stream.operators()[1]
timer.setParameter(0, core.UInt32(100))

stream.start()

for i in range(1000):
    data = timer.getOutputData(0)
    count = core.ReadAccess(data)
    print "Received {0}".format(count.get().get())
    del count
    
    timer.clearOutputData(0)

stream.stop()
#stream.join()


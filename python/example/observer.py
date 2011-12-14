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
        dataStr = ""
        if data.empty():
            dataStr = "empty"
        else:
            dataStr = str(core.ReadAccess(data).get().get())
            
        print "{0} {1} of Operator '{2}/{3}' was set to: {4}".format(
            connector.type(),
            connector.id(),
            connector.op().info().package(),
            connector.op().info().type(),
            dataStr)

factory = core.Factory()

core.registerCore(factory)
base.registerBase(factory)

stream = core.XmlReader().read("file.xml", factory)

source = stream.operators()[0]
timer = stream.operators()[1]

observer = MyObserver()
source.addObserver(observer)
timer.addObserver(observer)

stream.start()

for i in range(5):
    data = timer.getOutputData(0)
    timer.clearOutputData(0)

stream.stop()
stream.join()


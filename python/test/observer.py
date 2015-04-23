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

from stromx import runtime, test
import time

class MyObserver(runtime.ConnectorObserver):
    def observe(self, connector, data, thread):
        if data.empty() or connector.type() == runtime.Connector.INPUT:
            return
        
        with runtime.ReadAccess(data) as access:
            print access.get().get()

stream = runtime.Stream()
counter = stream.addOperator(runtime.Counter())
test = stream.addOperator(test.DeadlockOperator())
dump = stream.addOperator(runtime.Dump())

stream.initializeOperator(counter)
stream.initializeOperator(test)
stream.initializeOperator(dump)

# obtain write access in deadlock operator
test.setParameter(3, runtime.Bool(True))

stream.connect(counter, 0, test, 0)
stream.connect(test, 1, dump, 0)

thread = stream.addThread()
thread.addInput(test, 0)
thread.addInput(dump, 0)

stream.setDelay(1000)

observer = MyObserver()
test.addObserver(observer)

stream.start()

time.sleep(2)

stream.stop()
stream.join()


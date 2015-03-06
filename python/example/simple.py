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

from stromx import runtime

stream = runtime.Stream()

source = stream.addOperator(runtime.Counter())
stream.initializeOperator(source)

timer = stream.addOperator(runtime.PeriodicDelay())
stream.initializeOperator(timer)

timer.setParameter(0, runtime.UInt32(1000))

stream.connect(source, 0, timer, 0)

thread = stream.addThread()
thread.addInput(timer, 0)

stream.start()

for i in range(5):
    data = timer.getOutputData(0)
    with runtime.ReadAccess(data) as count:
        print "Received {0}".format(count.get().get())
    
    timer.clearOutputData(0)

stream.stop()
stream.join()

# -*- coding: utf-8 -*-

#  Copyright 2015 Matthias Fuchs
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
import unittest

class DeadlockObserver(runtime.ConnectorObserver):
    interrupt = False
    
    def observe(self, connector, oldData, newData, thread):
        # consider outputs only
        if connector.type() == runtime.Connector.INPUT:
            return
        
        # consider only 'sendOutputData()' calls -> leads to a deadlock when
        # acquiring the write access below
        if newData.empty():
            return
        
        try:
            with runtime.ReadAccess(newData) as access:
                print access.get().get()
        except runtime.Exception as e:
            print e
            self.interrupt = True
            
class OutputObserver(runtime.ConnectorObserver):
    data = None
    
    def observe(self, connector, oldData, newData, thread):
        # consider outputs only
        if connector.type() == runtime.Connector.INPUT:
            return
        
        # consider only 'clearOutputData()' calls
        if oldData.empty():
            return
        
        with runtime.ReadAccess(oldData) as access:
            print access.get().get()
            self.data = access.get().get()

class ObserversTest(unittest.TestCase):
    def setUp(self):
      stream = runtime.Stream()
      counter = stream.addOperator(runtime.Counter())
      deadlock = stream.addOperator(test.DeadlockOperator())
      dump = stream.addOperator(runtime.Dump())

      stream.initializeOperator(counter)
      stream.initializeOperator(deadlock)
      stream.initializeOperator(dump)

      # obtain write access in deadlock operator
      deadlock.setParameter(3, runtime.Bool(True))

      stream.connect(counter, 0, deadlock, 0)
      stream.connect(deadlock, 1, dump, 0)

      thread = stream.addThread()
      thread.addInput(deadlock, 0)
      thread.addInput(dump, 0)

      stream.setDelay(100)

      self.stream = stream
      self.deadlock = deadlock
        
    def tearDown(self):
        self.stream = None

    def testInterrupt(self):
        observer = DeadlockObserver()
        self.deadlock.addObserver(observer)
        self.stream.start()

        time.sleep(0.3)

        self.stream.stop()
        self.stream.join()
        
        self.assertTrue(observer.interrupt)

    def testObserverOutput(self):
        observer = OutputObserver()
        self.deadlock.addObserver(observer)
        self.stream.start()

        time.sleep(0.3)

        self.stream.stop()
        self.stream.join()
        
        self.assertNotEqual(None, observer.data)

if __name__ == '__main__':
    unittest.main()
    

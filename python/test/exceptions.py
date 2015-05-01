# -*- coding: utf-8 -*-

from stromx import runtime
import unittest


    

class ExceptionsTest(unittest.TestCase):
    def setUp(self):
      stream = runtime.Stream()
      counter = stream.addOperator(runtime.Counter())
      dump = stream.addOperator(runtime.Dump())

      stream.initializeOperator(counter)
      stream.initializeOperator(dump)

      stream.connect(counter, 0, dump, 0)

      thread = stream.addThread()
      thread.addInput(dump, 0)

      stream.setDelay(100)

      self.stream = stream
      self.dump = dump
      self.counter = counter
        
    def tearDown(self):
        self.stream = None
        
    def testExceptions(self):
        s = runtime.Stream()
        try:
            s.pause()
        except runtime.Exception as e:
            print str(e)
            
        try:
            s.pause()
        except runtime.WrongState as e:
            print str(e)
        
        i = -1
        try:
            s.pause()
        except runtime.WrongId as e:
            print str(e)
            i = 0
        except runtime.WrongState as e:
            print str(e)
            i = 1
        except runtime.Exception as e:
            print str(e)
            i = 2
        except Exception as e:
            print str(e)
            i = 3
            
        self.assertEqual(1, i)
    
    def testRemoveConnection(self):
        self.assertRaises(runtime.WrongArgument, 
                          self.stream.disconnect, self.dump, 1)
                          
        self.assertRaises(runtime.Exception, 
                          self.stream.disconnect, None, 0)

if __name__ == '__main__':
    unittest.main()
    
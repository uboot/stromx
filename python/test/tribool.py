# -*- coding: utf-8 -*-

from stromx.runtime import Tribool
import unittest

class TriboolTest(unittest.TestCase):     
    def testTrue(self):
        t = Tribool(True)
        self.assertEqual(Tribool(True), t)
        self.assertFalse(t.undefined())
        self.assertTrue(bool(t))   
        
    def testFalse(self):
        t = Tribool(False)
        self.assertEqual(Tribool(False), t)
        self.assertFalse(t.undefined())
        self.assertFalse(bool(t))
        
    def testUndefined(self):
        t = Tribool()
        self.assertEqual(Tribool(), t)
        self.assertTrue(t.undefined())
        self.assertFalse(bool(t))

if __name__ == '__main__':
    unittest.main()
    
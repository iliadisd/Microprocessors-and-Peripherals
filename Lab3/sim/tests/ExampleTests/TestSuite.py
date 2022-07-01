#!/usr/bin/python
from redBlocksTestSuite import *

# This test suite tests the normal workflow, which includes buying a product, 
# simulating a jammed slot and resetting the slot by entering maintenance mode.

s = Simulator("../../VendingMachine.rbp", "-c tcp:10000")
s.startAfterSimulation("runBinary.bat")

t = TestSuite(s, r"results\result.txt", "logs")
# Test the normal product buying workflow.
t.addTest(r"Buy_Product_Test.py")
t.beginTestSequence()
# Simulate a blocked product slot 4.
t.addTest(r"Blocked_Slot_Test.py")
# Clear blocked slot during maintenance.
t.addTest(r"Maintenance_Mode_Test.py")
# Repeat the normal buying workflow test, in order to check whether the cleard slot is handled normally.
t.addTest(r"Buy_Product_Test.py", "Buy_Product_Via_Cleared_Slot_Test")
t.endTestSequence()
t.run()

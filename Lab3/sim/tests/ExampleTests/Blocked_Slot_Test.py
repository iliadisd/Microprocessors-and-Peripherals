#!/usr/bin/python
from redBlocks import *
from redBlocksTest import *

# This script was generated by the redBlocks Simulator's test script recorder.
# It serves for demonstration purposes and tests the application's behaviour when a product slot is blocked, so the bottle cannot be realeased.
# It needs to be started with all slots being unblocked and will leave the first slot being blocked.

scf = SimulatorConnectionFactory()

# Registration of interfaces
scf.registerDO("Product 4 Lamp")
scf.registerDO("Product 3 Lamp")
scf.registerDO("Product 2 Lamp")
scf.registerDO("Product 1 Lamp")
scf.registerDO("Dispense Bay Lamp")
scf.registerDO("Change Bay Lamp")
scf.registerDO("Open Cash Box Actor")
scf.registerDO("Cash Box Return Actor")
scf.registerDO("Product 1 Release Gear")
scf.registerDO("Product 2 Release Gear")
scf.registerDO("Product 3 Release Gear")
scf.registerDO("Product 4 Release Gear")
scf.registerCC("GSM Modem")
scf.registerDP("Display")
scf.registerSBO("Automatic light barrier")
scf.registerDI("Product 4 Button")
scf.registerSBO("Euro100")
scf.registerDI("Return Money Button")
sc = scf.create()

# Define set of interfaces to observe
observer = Observer()
observer.add(sc.DO("Product 4 Lamp"))
observer.add(sc.DO("Product 3 Lamp"))
observer.add(sc.DO("Product 2 Lamp"))
observer.add(sc.DO("Product 1 Lamp"))
observer.add(sc.DO("Dispense Bay Lamp"))
observer.add(sc.DO("Change Bay Lamp"))
observer.add(sc.DO("Open Cash Box Actor"))
observer.add(sc.DO("Cash Box Return Actor"))
observer.add(sc.DO("Product 1 Release Gear"))
observer.add(sc.DO("Product 2 Release Gear"))
observer.add(sc.DO("Product 3 Release Gear"))
observer.add(sc.DO("Product 4 Release Gear"))
observer.add(sc.CC("GSM Modem"), 0.10)
observer.add(sc.DP("Display"), 0.10)

# time tolerance: adjust to a suitable value depending on the latency of the test setup
TOL = 0.20
observer.setTolerance(TOL)

observer.start()
time = observer.getTime()


# Test Preconditions
observer[sc.DO("Product 4 Lamp")].checkData(False)
observer[sc.DO("Product 3 Lamp")].checkData(False)
observer[sc.DO("Product 2 Lamp")].checkData(False)
observer[sc.DO("Product 1 Lamp")].checkData(False)
observer[sc.DO("Dispense Bay Lamp")].checkData(False)
observer[sc.DO("Change Bay Lamp")].checkData(False)
observer[sc.DO("Open Cash Box Actor")].checkData(True)
observer[sc.DO("Cash Box Return Actor")].checkData(False)
observer[sc.DO("Product 1 Release Gear")].checkData(False)
observer[sc.DO("Product 2 Release Gear")].checkData(False)
observer[sc.DO("Product 3 Release Gear")].checkData(False)
observer[sc.DO("Product 4 Release Gear")].checkData(False)
observer[sc.DP("Display")].checkData(DisplayData.fromFileContents(binaryData('0.rbb')))

observer.sleep(1.153)

time = observer.getTime()
sc.SBO("Automatic light barrier").setValue(False)

observer.sleep(1.530)

time = observer.getTime()
sc.DI("Product 4 Button").setValue(True)

observer[sc.DO("Product 4 Lamp")].addNoChangeCheck(time + 0.03 - TOL)
observer[sc.DO("Product 4 Lamp")].addEventCheck(time + 0.03 + TOL, True)

observer[sc.DP("Display")].addNoChangeCheck(time + 0.03 - TOL)
observer[sc.DP("Display")].addEventCheck(time + 0.03 + TOL, DisplayData.fromFileContents(binaryData('1.rbb')))

observer.sleep(0.091)

time = observer.getTime()
sc.DI("Product 4 Button").setValue(False)

observer.sleep(1.002)

time = observer.getTime()
sc.SBO("Euro100").setValue(True)

observer[sc.DP("Display")].addNoChangeCheck(time + 0.04 - TOL)
observer[sc.DP("Display")].addEventCheck(time + 0.04 + TOL, DisplayData.fromFileContents(binaryData('2.rbb')))

observer.sleep(0.295)

time = observer.getTime()
sc.SBO("Euro100").setValue(False)

observer.sleep(0.696)

time = observer.getTime()
sc.SBO("Euro100").setValue(True)

observer[sc.DO("Product 4 Release Gear")].addNoChangeCheck(time + 0.02 - TOL)
observer[sc.DO("Product 4 Release Gear")].addEventCheck(time + 0.02 + TOL, True)

observer[sc.DO("Product 4 Lamp")].addNoChangeCheck(time + 0.02 - TOL)
observer[sc.DO("Product 4 Lamp")].addEventCheck(time + 0.02 + TOL, False)

observer[sc.DP("Display")].addNoChangeCheck(time + 0.03 - TOL)
observer[sc.DP("Display")].addEventCheck(time + 0.03 + TOL, DisplayData.fromFileContents(binaryData('3.rbb')))

observer.sleep(0.303)

time = observer.getTime()
sc.SBO("Euro100").setValue(False)

observer[sc.DO("Product 4 Release Gear")].addNoChangeCheck(time + 0.72 - TOL)
observer[sc.DO("Product 4 Release Gear")].addEventCheck(time + 0.72 + TOL, False)

observer[sc.DO("Product 4 Release Gear")].addNoChangeCheck(time + 1.22 - TOL)
observer[sc.DO("Product 4 Release Gear")].addEventCheck(time + 1.22 + TOL, True)

observer[sc.DO("Product 4 Release Gear")].addNoChangeCheck(time + 2.22 - TOL)
observer[sc.DO("Product 4 Release Gear")].addEventCheck(time + 2.22 + TOL, False)

observer[sc.DO("Product 4 Release Gear")].addNoChangeCheck(time + 2.72 - TOL)
observer[sc.DO("Product 4 Release Gear")].addEventCheck(time + 2.72 + TOL, True)

observer[sc.DO("Product 4 Release Gear")].addNoChangeCheck(time + 3.72 - TOL)
observer[sc.DO("Product 4 Release Gear")].addEventCheck(time + 3.72 + TOL, False)

observer[sc.DO("Product 4 Release Gear")].addNoChangeCheck(time + 4.22 - TOL)
observer[sc.DO("Product 4 Release Gear")].addEventCheck(time + 4.22 + TOL, True)

observer[sc.DO("Open Cash Box Actor")].addNoChangeCheck(time + 5.21 - TOL)
observer[sc.DO("Open Cash Box Actor")].addEventCheck(time + 5.21 + TOL, False)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 5.21 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 5.21 + TOL, True)

observer[sc.DO("Product 4 Release Gear")].addNoChangeCheck(time + 5.22 - TOL)
observer[sc.DO("Product 4 Release Gear")].addEventCheck(time + 5.22 + TOL, False)

observer[sc.CC("GSM Modem")].addNoChangeCheck(time + 5.23 - TOL)
observer[sc.CC("GSM Modem")].addEventCheck(time + 5.23 + TOL, bytearray("AT+CMGF=1\x0aAT+CMGS=\"+"))

observer[sc.DP("Display")].addNoChangeCheck(time + 5.24 - TOL)
observer[sc.DP("Display")].addEventCheck(time + 5.24 + TOL, DisplayData.fromFileContents(binaryData('4.rbb')))

observer[sc.CC("GSM Modem")].addNoChangeCheck(time + 5.24 - TOL)
observer[sc.CC("GSM Modem")].addEventCheck(time + 5.24 + TOL, bytearray("4915233767667\"\x0aSlot "))

observer[sc.CC("GSM Modem")].addNoChangeCheck(time + 5.25 - TOL)
observer[sc.CC("GSM Modem")].addEventCheck(time + 5.25 + TOL, bytearray("blocked\x1a\x0a"))

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 5.72 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 5.72 + TOL, False)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 6.22 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 6.22 + TOL, True)

observer.sleep(6.486)

time = observer.getTime()
sc.SBO("Automatic light barrier").setValue(True)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 0.23 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 0.23 + TOL, False)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 0.73 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 0.73 + TOL, True)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 1.23 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 1.23 + TOL, False)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 1.74 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 1.74 + TOL, True)

observer[sc.DP("Display")].addNoChangeCheck(time + 1.74 - TOL)
observer[sc.DP("Display")].addEventCheck(time + 1.74 + TOL, DisplayData.fromFileContents(binaryData('0.rbb')))

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 2.24 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 2.24 + TOL, False)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 2.73 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 2.73 + TOL, True)

observer[sc.DO("Open Cash Box Actor")].addNoChangeCheck(time + 3.23 - TOL)
observer[sc.DO("Open Cash Box Actor")].addEventCheck(time + 3.23 + TOL, True)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 3.23 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 3.23 + TOL, False)

observer.sleep(4.829)

time = observer.getTime()
sc.DI("Product 4 Button").setValue(True)

observer[sc.DO("Product 4 Lamp")].addNoChangeCheck(time + 0.02 - TOL)
observer[sc.DO("Product 4 Lamp")].addEventCheck(time + 0.02 + TOL, True)

observer[sc.DP("Display")].addNoChangeCheck(time + 0.02 - TOL)
observer[sc.DP("Display")].addEventCheck(time + 0.02 + TOL, DisplayData.fromFileContents(binaryData('5.rbb')))

observer.sleep(0.141)

time = observer.getTime()
sc.DI("Product 4 Button").setValue(False)

observer.sleep(1.907)

time = observer.getTime()
sc.DI("Return Money Button").setValue(True)

observer[sc.DO("Open Cash Box Actor")].addNoChangeCheck(time + 0.02 - TOL)
observer[sc.DO("Open Cash Box Actor")].addEventCheck(time + 0.02 + TOL, False)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 0.02 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 0.02 + TOL, True)

observer[sc.DO("Product 4 Lamp")].addNoChangeCheck(time + 0.02 - TOL)
observer[sc.DO("Product 4 Lamp")].addEventCheck(time + 0.02 + TOL, False)

observer[sc.DP("Display")].addNoChangeCheck(time + 0.04 - TOL)
observer[sc.DP("Display")].addEventCheck(time + 0.04 + TOL, DisplayData.fromFileContents(binaryData('6.rbb')))

observer.sleep(0.102)

time = observer.getTime()
sc.DI("Return Money Button").setValue(False)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 0.43 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 0.43 + TOL, False)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 0.93 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 0.93 + TOL, True)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 1.42 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 1.42 + TOL, False)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 1.92 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 1.92 + TOL, True)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 2.43 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 2.43 + TOL, False)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 2.92 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 2.92 + TOL, True)

observer[sc.DP("Display")].addNoChangeCheck(time + 2.92 - TOL)
observer[sc.DP("Display")].addEventCheck(time + 2.92 + TOL, DisplayData.fromFileContents(binaryData('0.rbb')))

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 3.42 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 3.42 + TOL, False)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 3.92 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 3.92 + TOL, True)

observer[sc.DO("Open Cash Box Actor")].addNoChangeCheck(time + 4.42 - TOL)
observer[sc.DO("Open Cash Box Actor")].addEventCheck(time + 4.42 + TOL, True)

observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time + 4.42 - TOL)
observer[sc.DO("Change Bay Lamp")].addEventCheck(time + 4.42 + TOL, False)

observer.sleep(6.149 + TOL)

time = observer.getTime()

# Test end conditions
observer[sc.DO("Product 4 Lamp")].addNoChangeCheck(time - TOL)
observer[sc.DO("Product 3 Lamp")].addNoChangeCheck(time - TOL)
observer[sc.DO("Product 2 Lamp")].addNoChangeCheck(time - TOL)
observer[sc.DO("Product 1 Lamp")].addNoChangeCheck(time - TOL)
observer[sc.DO("Dispense Bay Lamp")].addNoChangeCheck(time - TOL)
observer[sc.DO("Change Bay Lamp")].addNoChangeCheck(time - TOL)
observer[sc.DO("Open Cash Box Actor")].addNoChangeCheck(time - TOL)
observer[sc.DO("Cash Box Return Actor")].addNoChangeCheck(time - TOL)
observer[sc.DO("Product 1 Release Gear")].addNoChangeCheck(time - TOL)
observer[sc.DO("Product 2 Release Gear")].addNoChangeCheck(time - TOL)
observer[sc.DO("Product 3 Release Gear")].addNoChangeCheck(time - TOL)
observer[sc.DO("Product 4 Release Gear")].addNoChangeCheck(time - TOL)
observer[sc.CC("GSM Modem")].addNoChangeCheck(time - TOL)
observer[sc.DP("Display")].addNoChangeCheck(time - TOL)

observer.stop()

#!/usr/bin/python
from redBlocks import *

def releaseProductCallback(sc, interface,value):
	if value and sc.SBO("Automatic light barrier").getValue():
		sc.DI("Light Barrier").setValue(True)
		sleep(0.01)
		sc.DI("Light Barrier").setValue(False)
		print "Product released"

scf = SimulatorConnectionFactory()
scf.registerDO("Product 1 Release Gear", releaseProductCallback)
scf.registerDO("Product 2 Release Gear", releaseProductCallback)
scf.registerDO("Product 3 Release Gear", releaseProductCallback)
scf.registerDO("Product 4 Release Gear", releaseProductCallback)
scf.registerDI("Light Barrier")
scf.registerSBO("Automatic light barrier")
sc = scf.create()

sc.SBO("Automatic light barrier").setValue(True)	

loopForever()

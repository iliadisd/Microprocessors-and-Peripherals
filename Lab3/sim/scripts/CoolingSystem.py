#!/usr/bin/python
from redBlocks import *

def enableCallback(sc, interface, value):
	if sc.SBO("EnableCoolingSystem").getValue():
		print "Cooling system enabled"
	else:
		print "Cooling system disabled"

scf = SimulatorConnectionFactory()
scf.registerSBO("EnableCoolingSystem", enableCallback)
scf.registerAI("Temperature Sensor")
scf.registerDO("Cooling System Actor")
sc = scf.create()

sc.SBO("EnableCoolingSystem").setValue(True)

while (True):
	sc.lock()
	
	if sc.isSimulationRunning() and sc.SBO("EnableCoolingSystem").getValue():
		temperature = sc.AI("Temperature Sensor").getValue()
		if sc.DO("Cooling System Actor").getValue():
			temperature -= 3
		if temperature < 0:
			temperature = 0
		else:
			temperature += 1
		if temperature > 255:
			temperature = 255
		sc.AI("Temperature Sensor").setValue(temperature)
		
	sc.unlock()
	sleep(0.5)

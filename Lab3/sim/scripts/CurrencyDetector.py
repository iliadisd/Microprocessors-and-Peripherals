#!/usr/bin/python
from redBlocks import *

def coinCallback(sc,interface,value):
	if value :
		print "Button " + interface.getName() + " pressed."
		if "Euro200" == interface.getName():
			value = "4"
		elif "Euro100" == interface.getName():
			value = "3"
		elif "Euro50" == interface.getName():
			value = "2"
		elif "Euro20" == interface.getName():
			value = "1"
		elif "Euro10" == interface.getName():
			value = "0"
		sc.CC("Coin Detector").sendMessage( value )
	
scf = SimulatorConnectionFactory()
scf.registerCC("Coin Detector")
scf.registerSBO("Euro200", coinCallback)
scf.registerSBO("Euro100", coinCallback)
scf.registerSBO("Euro50", coinCallback)
scf.registerSBO("Euro20", coinCallback)
scf.registerSBO("Euro10", coinCallback)
sc = scf.create()
loopForever()

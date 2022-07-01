#!/usr/bin/python
from redBlocks import *

def keyPadCallback(sc, interface,value):
	if value:
		key = ord(interface.getName()[6]) - 0x30
		print "Key " + interface.getName()[6] + " pressed."
		sc.CC("Keypad").sendMessage( bytearray([key]) )
	

scf = SimulatorConnectionFactory()
scf.registerCC("Keypad")
scf.registerSBO("KeyPad0", keyPadCallback)
scf.registerSBO("KeyPad1", keyPadCallback)
scf.registerSBO("KeyPad2", keyPadCallback)
scf.registerSBO("KeyPad3", keyPadCallback)
scf.registerSBO("KeyPad4", keyPadCallback)
scf.registerSBO("KeyPad5", keyPadCallback)
scf.registerSBO("KeyPad6", keyPadCallback)
scf.registerSBO("KeyPad7", keyPadCallback)
scf.registerSBO("KeyPad8", keyPadCallback)
scf.registerSBO("KeyPad9", keyPadCallback)
sc = scf.create()

loopForever()
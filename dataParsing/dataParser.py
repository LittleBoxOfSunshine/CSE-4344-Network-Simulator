import sys

lncString = "with network coding"
noLncString = "Starting simulator without network coding"
tickString = "Total number of ticks: "
tickPacketString = "Number of ticks during which a packet reached a destination: "
ctsString = "Total number of CTS: "
rtsString = "Total number of RTS: "
endSimulatorString = "End of simulator"

iterations = 1000

usingLnc = {}
noLnc = {}

usingLnc["totalTicks"] = 0
usingLnc["totalTicksCount"] = 0
usingLnc["tickPacket"] = 0
usingLnc["tickPacketCount"] = 0
usingLnc["ctsNum"] = 0
usingLnc["ctsNumCount"] = 0
usingLnc["rtsNum"] = 0
usingLnc["rtsNumCount"] = 0

noLnc["totalTicks"] = 0
noLnc["totalTicksCount"] = 0
noLnc["tickPacket"] = 0
noLnc["tickPacketCount"] = 0
noLnc["ctsNum"] = 0
noLnc["ctsNumCount"] = 0
noLnc["rtsNum"] = 0
noLnc["rtsNumCount"] = 0


fileType = sys.argv[1]
usingLnc = True
for x in range(1, iterations+1):
	filename = fileType + "/" + fileType + "Log" + str(x)
	dataFile = open(filename, 'r')

	for line in dataFile:
		if lncString in line:
			print "Using LNC"
			if tickString in line:
				usingLnc["totalTicks"] += int(line[len(tickString):])
				usingLnc["totalTicksCount"] += 1
			elif tickPacketString in line:
				usingLnc["tickPacket"] += int(line[len(tickPacketString):])
				usingLnc["tickPacketCount"] += 1
			elif ctsString in line:
				usingLnc["ctsNum"] += int(line[len(ctsString):])
				usingLnc["ctsNumCount"] += 1
			elif rtsString in line:
				usingLnc["rtsNum"] += int(line[len(rtsString):])
				usingLnc["rtsNumCount"] += 1
			elif endSimulatorString in line:
					print
					usingLnc = False
		elif noLncString in line:
			print "Not using LNC"
			if tickString in line:
				noLnc["totalTicks"] += int(line[len(tickString):])
				noLnc["totalTicksCount"] += 1
			elif tickPacketString in line:
				noLnc["tickPacket"] += int(line[len(tickPacketString):])
				noLnc["tickPacketCount"] += 1
			elif ctsString in line:
				noLnc["ctsNum"] += int(line[len(ctsString):])
				noLnc["ctsNumCount"] += 1
			elif rtsString in line:
				noLnc["rtsNum"] += int(line[len(rtsString):])
				noLnc["rtsNumCount"] += 1
			elif endSimulatorString in line:
				print
		

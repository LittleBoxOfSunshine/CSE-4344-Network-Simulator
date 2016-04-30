import sys

lncString = "with network coding"
noLncString = "Starting simulator without network coding"
tickString = "Total number of ticks: "
tickPacketString = "Number of ticks during which a packet reached a destination: "
ctsEmitString = "Total number of CTS Emit: "
rtsEmitString = "Total number of RTS Emit: "
ctsReceiveString = "Total number of CTS Receive: "
rtsReceiveString = "Total number of RTS Receive: "
endSimulatorString = "End of simulator"
numDestString = "Number of Destinations: "
latencyString = "has reached destination node"

iterations = 1

usingLnc = {}
noLnc = {}

usingLnc["totalTicks"] = [0, 0]	#[value, # of times this value was taken]
usingLnc["tickPacket"] = [0, 0]
usingLnc["ctsEmit"] = [0, 0]
usingLnc["rtsEmit"] = [0, 0]
usingLnc["ctsReceive"] = [0, 0]
usingLnc["rtsReceive"] = [0, 0]
usingLnc["numDest"] = [0, 0]
usingLnc["latency"] = [0, 0]

noLnc["totalTicks"] = [0, 0]
noLnc["tickPacket"] = [0, 0]
noLnc["ctsEmit"] = [0, 0]
noLnc["rtsEmit"] = [0, 0]
noLnc["ctsReceive"] = [0, 0]
noLnc["rtsReceive"] = [0, 0]
noLnc["numDest"] = [0, 0]
noLnc["latency"] = [0, 0]

fileType = sys.argv[1]
for x in range(1, iterations+1):
	filename = fileType + "/" + fileType + "Log" + str(x)
	filename = "logfile" 	#for testing
	dataFile = open(filename, 'r')
	lnc = True

	for line in dataFile:
		if lncString in line:
			lnc = True
		elif noLncString in line:
			lnc = False
		if lnc:
			if tickString in line:
				usingLnc["totalTicks"][0] += int(line[len(tickString):])
				usingLnc["totalTicks"][1] += 1
			elif latencyString in line:
				usingLnc["latency"][0] += int(line[line.find("in ")+3:line.find("ticks")])
				usingLnc["latency"][1] += 1
			elif numDestString in line:
				usingLnc["numDest"][0] += int(line[len(numDestString):])
				usingLnc["numDest"][1] += 1
			elif tickPacketString in line:
				usingLnc["tickPacket"][0] += int(line[len(tickPacketString):])
				usingLnc["tickPacket"][1] += 1
			elif ctsEmitString in line:
				usingLnc["ctsEmit"][0] += int(line[len(ctsEmitString):])
				usingLnc["ctsEmit"][1] += 1
			elif rtsEmitString in line:
				usingLnc["rtsEmit"][0] += int(line[len(rtsEmitString):])
				usingLnc["rtsEmit"][1] += 1
			elif ctsReceiveString in line:
				usingLnc["ctsReceive"][0] += int(line[len(ctsReceiveString):])
				usingLnc["ctsReceive"][1] += 1
			elif rtsReceiveString in line:
				usingLnc["rtsReceive"][0] += int(line[len(ctsReceiveString):])
				usingLnc["rtsReceive"][1] += 1
			elif endSimulatorString in line:
					lnc = False
		else:
			if tickString in line:
				noLnc["totalTicks"][0] += int(line[len(tickString):])
				noLnc["totalTicks"][1] += 1
			elif latencyString in line:
				noLnc["latency"][0] += int(line[line.find("in ")+3:line.find("ticks")])
				noLnc["latency"][1] += 1
			elif numDestString in line:
				noLnc["numDest"][0] += int(line[len(numDestString):])
				noLnc["numDest"][1] += 1
			elif tickPacketString in line:
				noLnc["tickPacket"][0] += int(line[len(tickPacketString):])
				noLnc["tickPacket"][1] += 1
			elif ctsEmitString in line:
				noLnc["ctsEmit"][0] += int(line[len(ctsEmitString):])
				noLnc["ctsEmit"][1] += 1
			elif rtsEmitString in line:
				noLnc["rtsEmit"][0] += int(line[len(rtsEmitString):])
				noLnc["rtsEmit"][1] += 1
			elif ctsReceiveString in line:
				noLnc["ctsReceive"][0] += int(line[len(ctsReceiveString):])
				noLnc["ctsReceive"][1] += 1
			elif rtsReceiveString in line:
				noLnc["rtsReceive"][0] += int(line[len(ctsReceiveString):])
				noLnc["rtsReceive"][1] += 1
			elif endSimulatorString in line:
				lnc = True

print "Using LNC"
for x in usingLnc.keys():
	print x + " " + str(usingLnc[x])

print "\nNot Using LNC"
for x in noLnc.keys():
	print x + " " + str(noLnc[x])

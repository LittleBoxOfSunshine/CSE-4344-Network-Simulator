
numTickString = "Total number of ticks: "

numTicksCoding = 0
numTicksNoCoding = 0
writeData = open('tickdata.csv', 'w')
for x in range(1,1000):
	filename = "smallMesh/smallMeshLog" + str(x)
	dataFile = open(filename, 'r')
	first = True
	for line in dataFile:
		if numTickString in line and first:
			numTicksCoding += int(line[len(numTickString):])
			first = False
		elif numTickString in line:
			numTicksNoCoding += int(line[len(numTickString):])
	dataFile.close()
avgTicksCoding = numTicksCoding / 1000
avgTicksNoCoding = numTicksNoCoding / 1000

writeData.write("10, " + str(avgTicksCoding) + ", " + '10' + ", " + str(avgTicksNoCoding) + "\n")


numTicksCoding = 0
numTicksNoCoding = 0
for x in range(1,1000):
	filename = "medMesh/medMeshLog" + str(x)
	dataFile = open(filename, 'r')
	first = True
	for line in dataFile:
		if numTickString in line and first:
			numTicksCoding += int(line[len(numTickString):])
			first = False
		elif numTickString in line:
			numTicksNoCoding += int(line[len(numTickString):])
	dataFile.close()
avgTicksCoding = numTicksCoding / 1000
avgTicksNoCoding = numTicksNoCoding / 1000

writeData.write("25, " + str(avgTicksCoding) + ", " + '25' + ", " + str(avgTicksNoCoding) + "\n")

numTicksCoding = 0
numTicksNoCoding = 0
for x in range(1,1000):
	filename = "largeMesh/largeMeshLog" + str(x)
	dataFile = open(filename, 'r')
	first = True
	for line in dataFile:
		if numTickString in line and first:
			numTicksCoding += int(line[len(numTickString):])
			first = False
		elif numTickString in line:
			numTicksNoCoding += int(line[len(numTickString):])
	dataFile.close()
avgTicksCoding = numTicksCoding / 1000
avgTicksNoCoding = numTicksNoCoding / 1000

writeData.write("75, " + str(avgTicksCoding) + ", " + '75' + ", " + str(avgTicksNoCoding) + "\n");


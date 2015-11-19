import json
import sys
import struct

if len(sys.argv) < 2:
    exit(-1)

print("Reading " + sys.argv[1] + "...")

with open(sys.argv[1]) as inputFile:
    jsonData = json.load(inputFile)

print("Done!")

width = jsonData["map"]["width"]
height = jsonData["map"]["height"]
mapData = jsonData["map"]["data"]

print("Map: " + width + "x" + height)

destFileName = sys.argv[1] + '.map'

print("Writing " + destFileName + "...")

destFile = open(destFileName, "w")
destFile.write(struct.pack('B', int(width)))
destFile.write(struct.pack('B', int(height)))
for i in mapData:
    destFile.write(bytearray(struct.pack('B', int(i))))

print("Done!")

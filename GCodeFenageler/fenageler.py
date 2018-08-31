#!/usr/local/bin/python3
import numpy as np

print("~~~~~~~~~~~~~~~~~~~~GCode Fenageler~~~~~~~~~~~~~~~~~~~~")

InputGCode = "testInput.gcode"
OutputMCode = "testOutput.gcode"
gCodeMaxSize = 400 #assume square
SandTableMaxRadius = 12000
SandTableMaxTheta = 8000
scalingRadiusRatio = SandTableMaxRadius/gCodeMaxSize
scalingThetaRatio = SandTableMaxTheta/np.pi
pointList = []

def offsetCartesian(x,y):
    """Returns x and y relative to center\n
    should be used when converting Cartesian to Polar"""
    center = gCodeMaxSize/2
    return x-center,y-center

def cartesianToPolar(x,y):
    """Returns r and theta\n
    inputs must be relative to center"""
    radius = np.sqrt(x**2 + y**2)
    theta = np.arctan2(y, x)
    return radius, theta

def scaleRadius(r):
    """scales radius down or up to steps in full radius Movement"""
    output = r*scalingRadiusRatio
    return int(output)

def scaleTheta(theta):
    """scales theta down or up to steps in full theta Movement"""
    output = theta*scalingThetaRatio
    return int(output)

 # read in input gCode file
f = open(InputGCode, 'r')
for line in f:
    parts = line.split()
    if len(parts) == 3:
        if parts[0] == "G01":
            g = "M1"
            x = float(parts[1][1:])
            y = float(parts[2][1:])
            pointList.append((g,x,y))
f.close()
# print(pointList)





# output the mCode
f = open(OutputMCode,'w')
for point in pointList:
    x,y = offsetCartesian(point[1],point[2])
    r, theta = cartesianToPolar(x,y)
    r=scaleRadius(r)
    theta = scaleTheta(theta)
    f.write(point[0]+ " R"+str(r)+" T"+str(theta)+"\n")

f.close()
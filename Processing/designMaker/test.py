from circleLineIntersectionTest import *

f = open('simpleSquareThing.gcode', 'r')
pointList = []
for line in f:
    parts = line.split()
    if len(parts) == 3:
        if parts[0] == "G01":
            x = float(parts[1][1:])
            y = float(parts[2][1:])
            # print(parts[1][1:],parts[2][1:])
            pointList.append((x,y))
# print(list)
# return list
circleRadius = 200
circleCenter = (200,200)
newList = []
nextPoint = 0
currentlyFixingPoints = False
lastOutsidePoint = 0
lastPoint = 0
for point in pointList:
    nextPoint+=1
    if currentlyFixingPoints:
        currentlyFixingPoints = False
        newList.append(("G0","G0"))
        replacementPoint = findIntersections(point,lastOutsidePoint,circleCenter,circleRadius)
        newList.append(replacementPoint)
        newList.append(point)
        pass
    else:
        if isPointInCircle(circleCenter,point,circleRadius):
            newList.append(point)
            lastPoint = point
        else: #currently assuming two points cant be out of circle in a row
            lastOutsidePoint = point
            replacementPoint = findIntersections(point,lastPoint,circleCenter,circleRadius)
            newList.append(replacementPoint)
            currentlyFixingPoints = True
print(pointList)
print(newList)
for point in newList:
    if point == ("G0","G0"):
        pass
    else:
        print("G01 X"+str(point[0])+" Y"+str(point[1]))
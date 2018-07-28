from circleLineIntersectionTest import *

f = open('simpleSquareThing.gcode', 'r') #simpleSquareThing
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
lastInsidePoint = 0
for point in pointList:
    nextPoint+=1
    if currentlyFixingPoints:
        currentlyFixingPoints = False
        newList.append(("G0","G0"))
        # reEntryPoint = findIntersections(lastOutsidePoint,lastOutsidePoint,circleCenter,circleRadius)
        replacementPoint = findIntersections(point,lastOutsidePoint,circleCenter,circleRadius)
        newList.append(replacementPoint)
        newList.append(point)
        pass
    else:
        if isPointInCircle(circleCenter,point,circleRadius):
            newList.append(point)
            lastInsidePoint = point
        else: #currently assuming two points cant be out of circle in a row
            lastOutsidePoint = point
            #exiting point
            replacementPoint = findIntersections(point,lastInsidePoint,circleCenter,circleRadius)
            newList.append(replacementPoint)
            # currentlyFixingPoints = True #KILLS THIS
            # now need to make a reentryPoint
            #assumes there is a next point and a previous point
            newList.append(("G0","G0"))
            replacementPoint = findIntersections(point,pointList[nextPoint],circleCenter,circleRadius)
            newList.append(replacementPoint)

print(pointList)
print(newList)
for point in newList:
    if point == ("G0","G0"):
        pass
    else:
        print("G01 X"+str(point[0])+" Y"+str(point[1]))
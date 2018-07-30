from circleLineIntersectionTest import *

f = open('squareTest.gcode', 'r') #simpleSquareThing
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
listlen = len(pointList)
lastPointWasInCircle = False
for point in pointList:
    nextPoint+=1
    if isPointInCircle(circleCenter,point,circleRadius):
        newList.append(point)
        lastInsidePoint = point
        lastPointWasInCircle = True
    else: #if this point is not in the circle
        if lastPointWasInCircle: #if this is exit of the circle
            lastOutsidePoint = point
            #exiting point
            replacementPoint = findIntersections(point,pointList[nextPoint-2],circleCenter,circleRadius)
            if replacementPoint != None:
                newList.append(replacementPoint)

        # else: #if last point was not in circle
        #     if nextPoint < listlen: #and there is still another point after this
        #         if not isPointInCircle(circleCenter,pointList[nextPoint],circleRadius): #if next point is not in circle
        #             pass


        # currentlyFixingPoints = True #KILLS THIS
        # now need to make a reentryPoint
        #assumes there is a next point and a previous point
        # try:

        if nextPoint < listlen: # if there are more points after this
            # if isPointInCircle(circleCenter,pointList[nextPoint],circleRadius): #if next point is in circle # need to change this to if point intersects!!!
                # make reentry point
                replacementPoint = findIntersections(point,pointList[nextPoint],circleCenter,circleRadius)
                if replacementPoint != None:
                    newList.append(("G0","G0"))
                    newList.append(replacementPoint)
        lastPointWasInCircle = False
        # except:
        #     pass

print(pointList)
print(newList)
for point in newList:
    if point == ("G0","G0"):
        pass
        # print("G0 X0 Y0")
    else:
        print("G01 X"+str(point[0])+" Y"+str(point[1]))
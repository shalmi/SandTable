from math import sqrt


def calculateRadius(circleCenter,point):
    x = point[0]
    y = point[1]
    # print(x,y)
    dist = sqrt( (circleCenter[0]-point[0])**2 + (circleCenter[1]-point[1])**2)
    # print(dist)
    return dist

def isPointInCircle(circleCenter,point,circleRadius):
    if calculateRadius(circleCenter,point) >circleRadius:
        return False
    return True


# assumes only two points in "points"
# returns the point that is closer to origin
def findCloserPoint(origin, points):
    dist1 = sqrt( (origin[0]-points[0][0])**2 + (origin[1]-points[0][1])**2)
    dist2 = sqrt( (origin[0]-points[1][0])**2 + (origin[1]-points[1][1])**2)
    if dist1 < dist2:
        return points[0]
    else:
        return points[1]

def findIntersections(point1,point2,circleCenter,circleRadius):
    results = []
    p1x = point1[0]
    p1y = point1[1]
    p2x = point2[0]
    p2y = point2[1]
    a = circleCenter[0]
    b = circleCenter[1]

    p1x+=.0000001 # this fixes vertical lines...its a cheat but I accept it for this use case
    p1y+=.0000001
    try:
        m =float( float((p2y-p1y))/float((p2x-p1x)) )
    except:
        # slope of a horizontal line is 0
        # slope of a veritcal line is undefined.
        # if exception. Slope must be a a vertical line
        m= "undefined"
    # if m == 0:
    #     m=0.0000001
    potentiallNoSolution = False
    if type(m) == float and m!=0: #if not a perfectly vertical line
        g = p2y-m*p2x #this is b from y=mx+b
        # print ("m:",m,"g:",g)
        r = circleRadius
        #yint1 = b-sqrt(-1*(cx**2)+2*cx*lineEquationX+circleRadius**2-lineEquationX**2)
        # a will be cx
        # b will be cy
        # g is b?
        try:
            yint1 =  (-1*sqrt((-2 * a * m - 2 * b * m**2 - 2 * g)**2 - 4 * (m**2 + 1) * (a**2 * m**2 + 2 * a * g * m + b**2 * m**2 + g**2 - m**2 * r**2)) + 2 * a * m + 2 * b * m**2 + 2 * g)/(2 * (m**2 + 1))
            # print(yint1)
            # if yint1 <circleCenter[1]:
            #     offset = circleCenter[1]-yint1
            #     yint1=circleCenter[1]+offset
            # else:
            #     offset = yint1-circleCenter[1]
            #     yint1 = circleCenter[1]-offset
        except:
            print("fail1")
            potentiallNoSolution = True
            yint1 = 0
        try:
            yint2 =     (sqrt((-2 * a * m - 2 * b * m**2 - 2 * g)**2 - 4 * (m**2 + 1) * (a**2 * m**2 + 2 * a * g * m + b**2 * m**2 + g**2 - m**2 * r**2)) + 2 * a * m + 2 * b * m**2 + 2 * g)/(2 * (m**2 + 1))
            # if yint2 <circleCenter[1]:
            #     offset = circleCenter[1]-yint2
            #     yint2=circleCenter[1]+offset
            # else:
            #     offset = yint2-circleCenter[1]
            #     yint2 = circleCenter[1]-offset
        except:
            print("fail2")
            if potentiallNoSolution:
                return None
            yint2 = 0
        try:
            xint1 = (yint1 -g)/m
        except:
            print("fail3",yint1,g,m)
            xint1 = 0
        try:
            xint2 = (yint2 -g)/m
        except:
            print("fail4",yint1,g,m)
            xint2 = 0
        
        if (yint1 != 0.0) or (xint1 != 0.0):
            results.append(( round(xint1, 4), round(yint1,4)))

        if (xint2 != 0.0) or (yint2 != 0.0):
            results.append((round(xint2,4),round(yint2,4)))
        #print(yint1,yint2,xint1,xint2)
    if len(results) == 2:
        result = findCloserPoint(point1,results)
        if result[0] == 0 and result[1] == 400.0:
            print("HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO")
            print(point1,point2,circleCenter,circleRadius)
        return result
    # print(results)
    if len(results) ==1:
        return results[0]

print( findIntersections((0.0, 0.0), (0.0, 0.0), (200, 200), 200) )

# circleRadius = 200
# circleCenter = (200,200)
# point1 = (13,24)# (100,100)
# point2 = (0, 1)
# point1 = (200,200)# (100,100)
# point2 = (500,200 )
# print(isPointInCircle(circleCenter,point1,circleRadius))
# print(isPointInCircle(circleCenter,point2,circleRadius))
# print("~findIntersections~")
# findIntersections(point1,point2,circleCenter,circleRadius)
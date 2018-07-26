from math import sqrt
circleRadius = 200
circleCenter = (200,200)
point1 = (13,24)# (100,100)
point2 = (0, 1)

def calculateRadius(circleCenter,point):
    x = point[0]
    y = point[1]
    print x,y
    dist = sqrt( (circleCenter[0]-point[0])**2 + (circleCenter[1]-point[1])**2)
    print dist
    return dist

def isPointInCircle(circleCenter,point,circleRadius):
    if calculateRadius(circleCenter,point) >circleRadius:
        return False
    return True
print(isPointInCircle(circleCenter,point1,circleRadius))
print(isPointInCircle(circleCenter,point2,circleRadius))

def findIntersections(point1,point2,circleCenter,circleRadius):
    p1x = point1[0]
    p1y = point1[1]
    p2x = point2[0]
    p2y = point2[1]
    a = circleCenter[0]
    b = circleCenter[1]
    m =float( float((p2y-p1y))/float((p2x-p1x)) )
    g = p2y-m*p2x #this is b from y=mx+b
    print ("m:",m,"g:",g)
    r = circleRadius
    #yint1 = b-sqrt(-1*(cx**2)+2*cx*lineEquationX+circleRadius**2-lineEquationX**2)
    # a will be cx
    # b will be cy
    # g is b?
    yint1 =  (-1*sqrt((-2 * a * m - 2 * b * m**2 - 2 * g)**2 - 4 * (m**2 + 1) * (a**2 * m**2 + 2 * a * g * m + b**2 * m**2 + g**2 - m**2 * r**2)) + 2 * a * m + 2 * b * m**2 + 2 * g)/(2 * (m**2 + 1))
    yint2 =     (sqrt((-2 * a * m - 2 * b * m**2 - 2 * g)**2 - 4 * (m**2 + 1) * (a**2 * m**2 + 2 * a * g * m + b**2 * m**2 + g**2 - m**2 * r**2)) + 2 * a * m + 2 * b * m**2 + 2 * g)/(2 * (m**2 + 1))
    xint1 = (yint1 -g)/m
    xint2 = (yint2 -g)/m
    print (xint1,yint1)
    print (xint2,yint2)
print("~findIntersections~")
findIntersections(point1,point2,circleCenter,circleRadius)
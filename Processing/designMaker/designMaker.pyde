import time

lastX = 0
lastY = 0
squareSize = 2
sizeIncrementBy = 5
rotDeg = 0 #in degrees
degIncrementBy = 4
tableDiam = 400
tableRadius = tableDiam/2
screenSize = tableDiam+50
centerScreen = screenSize/2
endProgram = False
iterations = 45
state=0
xys = [(0,0)]
def setup():
    size(screenSize*2, screenSize)
    # colorMode(HSB, width, 100, width)
    noStroke()
    background(0)

def draw():
    global state
    if(state==0):
        drawTable()
        state+=1
    elif(state==1):
        global squareSize
        global rotDeg
        global iterations
        # line (100,100,200,200)
        # pushMatrix()
        translate(centerScreen,centerScreen) #Go to Center of Screen
        drawBrokenSquare(squareSize)
        # drawBrokenSquare(65)
        # popMatrix()
        squareSize+=sizeIncrementBy
        rotDeg += degIncrementBy
        # time.sleep(.2)
        iterations-=1
        if(iterations==0):
            state+=1
            iterations = 0
            print(xys)
    elif(state == 2):
        translate(centerScreen*2,0)
        drawTable()
        state+=1
    elif(state ==3):
        translate(centerScreen*3,centerScreen)
        line(xys[iterations][0],xys[iterations][1],xys[iterations+1][0],xys[iterations+1][1])
        iterations+=1
        if iterations+1>=len(xys):
            state+=1

def drawTable():
    stroke(255)
    noFill()
    ellipse(centerScreen,centerScreen,tableDiam,tableDiam)
def RotatePoint(x,y,deg):
    theta = radians(deg)
    newX = x*cos(theta)-y*sin(theta)
    newY = x*sin(theta)+y*cos(theta)
    return newX,newY

def drawBrokenSquare(sideLength):
    global lastX
    global lastY
    global endProgram
    cSize = 5
    outOfBoundsCount=0
    xCoords = (1,1,-1,-1)
    yCoords = (1,-1,-1,1)
    # stroke(255) #lets go white
    stroke(204, 102, 0) #orange you glad
    for i in range(4):
        # print(lastX,lastY,xCoords[i]*sideLength,yCoords[i]*sideLength)
        newX = xCoords[i]*sideLength
        newY = yCoords[i]*sideLength
        newX, newY = RotatePoint(newX,newY,rotDeg)
        
        # #if outside circle
        # r = sqrt(newX**2+newY**2)
        # if r > tableRadius:
        #     # endProgram = True
        #     # theta = atan(newY/newX)
        #     outOfBoundsCount+=1
        #     if outOfBoundsCount==4:
        #         endProgram=True
        
        if i==3:
            newY+=sizeIncrementBy
        line(lastX,lastY,newX,newY)
        xys.append((newX,newY))
        # ellipse(newX,newY,cSize,cSize) #draw each Point
        lastX = newX
        lastY = newY
        # time.sleep(1)

        

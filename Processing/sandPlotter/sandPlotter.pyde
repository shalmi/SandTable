from RotatingArm import rotatingArm
from BallMount import Ball
from Controller import controller

keepGoing = False
global arm
global ball #= Ball()
global cont
PointByPoint = False
goToX = 0
goToY = 0
def setup():
    global arm
    global ball
    global cont
    size(500, 500)
    background(0)
    noStroke()
    print("hello")
    
    drawBoard()
    ball = Ball(width/2)
    arm = rotatingArm(width/2, height/2, width/2, ball)
    cont = controller(ball,arm,width/2,height/2)
    arm.drawArm()
    
    # arm.init(3,4)
    # print(arm.x)

def draw():
    background(0)
    drawBoard()
    global arm
    global keepGoing
    global PointByPoint
    arm.drawArm()
    
    if keyPressed:
        if key == 'w':
            arm.increaseDegree(-1)
            keepGoing=False
        elif key == 's':
            arm.increaseDegree(1)
            keepGoing=False
        elif key == 'a':
            ball.move(-1)
            keepGoing=False
        elif key == 'd':
            ball.move(1)
            keepGoing=False
        # elif key == 'g':
        #     print("Going To Point")
        #     cont.HeadTowardsPoint(goToX, goToY)
        #     keepGoing=False
        elif key == 'G':
            keepGoing = True
        elif key == 'c':
            Steppers = cont.calculateSubPoints(goToX,goToY)
            for step in Steppers:
                point(step[0],step[1])
        elif key == 'h':
            PointByPoint = cont.headToNextPoint()
            
    if keepGoing:
        cont.HeadTowardsPoint(goToX, goToY)
    if PointByPoint:
        PointByPoint = cont.headToNextPoint()
# draw a white circle on the frame
def drawBoard():
    # draw the Circular Frame
    fill(0,0) #black and seethrough
    stroke(255)
    ellipse(width/2, height/2, width, height)
    # draw center dot
    point(width/2, height/2)
    point(goToX, goToY) #draw the point we are trying to go to

def mousePressed():
    global goToX
    global goToY
    goToX = mouseX
    goToY = mouseY
    cont.calculateSubPoints(goToX,goToY)
    print("x: "+str(goToX)+", y: "+str(goToY))

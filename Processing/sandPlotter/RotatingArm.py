from BallMount import Ball
xCenter = 0
yCenter = 0
armLength = 0
degree = 0
incr = .25
global ball

class rotatingArm(object):

    def __init__(self,xcenter, ycenter, armlength, importedBall):
        global xCenter
        global yCenter
        global armLength
        global ball
        # self.position = 45 #ranges from 0-360 for each degree
        xCenter = xcenter
        yCenter = ycenter
        armLength = armlength
        ball = importedBall
        
    def update(self, invWidth, invHeight, fluidSolver):
        print("updating...")
    
    def increaseDegree(self,direction):
        global degree
        gain=1 #6-int(round(ball.getPosition()/50))
        if direction >0:
            degree += incr*gain
            if degree>360:
                degree-=360
        else:
            degree -= incr*gain
            if degree<0:
                degree=degree+360
    
    def drawArm(self,):
        fill(0,0)
        stroke(255)
        pushMatrix()
        translate(xCenter,yCenter)
        rotate(radians(degree))
        line(0, 0, armLength, 0)
        ball.drawBall()
        popMatrix()

    def getDegrees(self):
        return degree

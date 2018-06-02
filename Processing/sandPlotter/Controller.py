from BallMount import Ball
from RotatingArm import rotatingArm
global ball
global arm
global currentDegrees
global currentTravel
global centerX
global centerY
SteppingPoints = []
currentStep = 0
totalSteps = 0
degreeReady = False
rReady = False
degreeTolerance = .3

class controller(object):
    
    def __init__(self,importedBall,importedArm,centX,centY):
        global ball
        global arm
        global centerX
        global centerY
        ball = importedBall
        arm = importedArm
        centerX = centX
        centerY = centY

    def updateXY(self):
        # Write code here to get x and y
        print("no")
    
    def updatePolar(self):
        global currentDegrees
        global currentTravel
        currentDegrees = arm.getDegrees()
        currentTravel = ball.getPosition()
        #print("CURRENT: r: "+str(currentTravel)+",degrees: "+str(currentDegrees))
    
    def cartesianToPolar(self,x,y):
        origX = x
        origY = y
        x=float(x-centerX)
        y=float(y-centerY)
        r = sqrt(x**2+y**2)
        radian = atan(y/x)
        if x<0:
            radian = radian + PI
        elif y<0:
            radian = radian + 2*PI
        degree = degrees(radian)
        #print ("DESIRED: r: "+str(r)+", degree: "+str(degree))
        return degree, r
    
    def polarToCartesian(self, theta, r):
        theta = radians(theta)
        x = r*cos(theta)
        y = r*sin(theta)
        return x, y

    def calculateSubPoints(self, endX, endY):
        global SteppingPoints
        global currentStep
        global totalSteps
        global degreeReady
        global rReady
        degreeReady = False
        rReady = False
        self.updatePolar()
        currentX, currentY = self.polarToCartesian(currentDegrees,currentTravel)
        endXRel = endX-centerX
        endYRel = endY-centerY
        distance = sqrt((endXRel-currentX)**2+(endYRel-currentY)**2)
        print currentX,currentY
        print distance
        distINT = int(round(distance))
        SteppingPoints = []
        xIncr = (endXRel-currentX)/distINT #TODO: maybe make this a float...
        yIncr = (endYRel-currentY)/distINT
        for x in range(distINT):
            SteppingPoints.append([currentX+centerX+(xIncr*x+1),currentY+centerY+(yIncr*x+1)])
        currentStep = 0
        totalSteps = len(SteppingPoints)
        return SteppingPoints
        
    def HeadTowardsPoint(self,endX, endY):
        self.updatePolar()
        degreeEnd, rEnd = self.cartesianToPolar(endX,endY)
        if (degreeEnd-currentDegrees>.5 and degreeEnd-currentDegrees <180): #and if its not slower to go the opposite way
            arm.increaseDegree(1) #go clockwise
        elif(degreeEnd-currentDegrees>.5):
            arm.increaseDegree(-1) #go counter clockwise
        elif (degreeEnd-currentDegrees<-.5 and currentDegrees-degreeEnd <180):
            arm.increaseDegree(-1) #go counter clockwise
        elif (degreeEnd-currentDegrees<-.5):
            arm.increaseDegree(1) #go clockwise
        if (rEnd-currentTravel>.5):
            ball.move(1)
        elif (rEnd-currentTravel<-.5):
            ball.move(-1)
    
    ### returns False if all points have been found! True otherwise
    def headToNextPoint(self,):
        global degreeReady
        global rReady
        global currentStep
        self.updatePolar()
        if(degreeReady and rReady):
            currentStep+=1
            degreeReady = False
            rReady = False
        if currentStep >= totalSteps:
            return False
        
        endX = SteppingPoints[currentStep][0]
        endY = SteppingPoints[currentStep][1]
        
        #skip to next step if you are closer to the step after
        if currentStep!=totalSteps-1: #as long as this is not the last step
            nextEndX = SteppingPoints[currentStep+1][0]
            nextEndY = SteppingPoints[currentStep+1][1]
            currentX, currentY = self.polarToCartesian(currentDegrees,currentTravel)
            currentX += centerX
            currentY += centerY
            distPoint1 = sqrt((endX-currentX)**2+(endY-currentY)**2)
            distPoint2 = sqrt((nextEndX-currentX)**2+(nextEndY-currentY)**2)
            if distPoint1 >distPoint2:
                degreeReady = False
                rReady = False
                currentStep+=1
                print"~~~~~~~~~~~~~~~~~~~~~~~~"
                print currentX,currentY
                print endX, endY, distPoint1
                print nextEndX, nextEndY, distPoint2
                print distPoint1-distPoint2
                print"~~~~~~~~~~~~~~~~~~~~~~~~"
                # while(True):
                #     pass
                return True
        
        
        
        
        degreeEnd, rEnd = self.cartesianToPolar(endX,endY)
        # if degreeReady:
        #     pass
        if (degreeEnd-currentDegrees>degreeTolerance and degreeEnd-currentDegrees <180): #and if its not slower to go the opposite way
            arm.increaseDegree(1) #go clockwise
        elif(degreeEnd-currentDegrees>degreeTolerance):
            arm.increaseDegree(-1) #go counter clockwise
        elif (degreeEnd-currentDegrees<-1*degreeTolerance and currentDegrees-degreeEnd <180):
            arm.increaseDegree(-1) #go counter clockwise
        elif (degreeEnd-currentDegrees<-1*degreeTolerance):
            arm.increaseDegree(1) #go clockwise
        else:
            degreeReady = True
        # if rReady:
        #     pass
        if (rEnd-currentTravel>.5):
            ball.move(1)
        elif (rEnd-currentTravel<-.5):
            ball.move(-1)
        else:
            rReady = True
        return True
        
        

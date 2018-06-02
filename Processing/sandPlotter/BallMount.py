global maxPos
global currentPosition
ballSize = 7
class Ball(object):
    
    def __init__(self,maxTravel):
        global currentPosition
        global maxPos
        maxPos = maxTravel
        currentPosition = 10
    
    # print("HELLO IM A BALL")
    def drawBall(self):
        ellipse(currentPosition, 0, ballSize, ballSize)
    
    def move(self,direction):
        global currentPosition
        if direction >0:
            if (currentPosition < maxPos):
                currentPosition +=1
        elif direction <1:
            if (currentPosition > 0):
                currentPosition -=1

    def getPosition(self):
        return currentPosition

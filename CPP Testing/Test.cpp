#include <iostream>
using namespace std;
#include <math.h>       /* sqrt */

void printHello(){
    cout << "Hello, World!"<<endl;
}

float sq(float input){
    return input*input;
}


double distanceBetweenPoints(float x1,float y1,float x2, float y2){
    return sqrt( sq(y2-y1)+sq(x2-x1) );
}


void findArrayPointsBetweenPoints(float x1,float y1,float x2, float y2){
    float xDistance = x2-x1;
    float yDistance = y2-y1;
    // double totalDistance = distanceBetweenPoints(x1,y1,x2,y2);
    double totalDistance = sqrt( sq(yDistance)+sq(xDistance) );
    int numOfMinorPoints = totalDistance/50;
    float xDeltaBy = xDistance/numOfMinorPoints;
    float yDeltaBy = yDistance/numOfMinorPoints;
    cout << "xDeltaBy: "<<xDeltaBy<<endl;
    cout << "yDeltaBy: "<<yDeltaBy<<endl;
    float newX = x1;
    float newY = y1;
    for (int i = 0;i <=numOfMinorPoints;i++){
        cout <<newX<< ", "<<newY<<endl;
        newX+=xDeltaBy;
        newY+=yDeltaBy;
    }
}

int main() 
{
    printHello();

    findArrayPointsBetweenPoints(100.0,-100.0,0.0,100);
    // cout <<dist<<endl;
    return 0;
}


#include <iostream>
#include <wiringPi.h>
#include <atomic>
#include <thread>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <memory>
#include <signal.h>
#include <sys/types.h>
using namespace std;
#define MOTORX 0
#define MOTORY 1
#define MOTORZ 2
#define MOTORX2 3
#define motor1PinA 7
#define motor1PinB 0
#define motor1PinC 2
#define motor1PinD 3
#define motor2PinA 21
#define motor2PinB 22
#define motor2PinC 23
#define motor2PinD 24
#define motor3PinA 26
#define motor3PinB 27
#define motor3PinC 28
#define motor3PinD 29
#define motor4PinA 1
#define motor4PinB 4
#define motor4PinC 5
#define motor4PinD 6
#define touchCallbackPin 25
#define motorXStartPosition 300
#define motorYStartPosition 300


int pin[4][4] = {
        {motor1PinA,motor1PinB,motor1PinC,motor1PinD},
        {motor2PinA,motor2PinB,motor2PinC,motor2PinD},
        {motor3PinA,motor3PinB,motor3PinC,motor3PinD},
        {motor4PinA,motor4PinB,motor4PinC,motor4PinD}
};
int totalSteps=0;
static int isTouched=0;
useconds_t delayMotor = 1000;


void setStep(int w1,int w2,int w3,int w4,int motor);
void calculate();

void setupPins();
void releasePins();
int goZForwardUntilTouched();
void goForward(int stepCount,int motor);
void goBackward(int stepCount,int motor);
void sendCoord(int x,int y,int totalSteps);
void goForwardX(int stepCount);

void syncMotors();

void goBackwardX(int stepCount);
int calculateHeight();

void goUp(int stepCount);
void goDown(int stepCount);


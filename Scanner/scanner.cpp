#include "scanner.h"
#include "../communication/ScannerClient.h"

int heightMap[20][20];
int startHeight = 300;
int k=0;

using namespace CanTouchThis;
shared_ptr<ScannerClient> client;
shared_ptr<thread> mainThread;

int main(int argc,char** argv){
    Command command;
	client.reset(new ScannerClient (argv[1], PORT, command));
    startHeight = atoi(argv[2]);
    /*setupPins();
    goUp(100);
    fprintf(stderr,"Sync motors...");
    syncMotors();
    fprintf(stderr,"Starting to calculate...");
    calculate();
    releasePins();
    exit(0);
    
    return 0;*/
	
    
    pid_t cpd = 0;
    
    while (true) {
		client->listenToController();
        std::cout << "LISTENING\n";
		if (command == Command::START_SCAN){
			std::cout << "START\n";
            if((cpd=fork())==0){
                setupPins();
                syncMotors();
                calculate();
                releasePins();
                exit(0);
            }
        }
		else if (command == Command::STOP_SCAN){
			std::cout << "STOP\n";
            if(cpd!=0)kill(cpd,SIGKILL);
        }
        else if (command == Command::MOTOR_X_START){
			std::cout << "X1 START\n";
            if((cpd=fork())==0){
                setupPins();
                goForwardX(99999);
                releasePins();
                exit(0);
            }
        }
		else if (command == Command::MOTOR_X1_START){
			std::cout << "X1 START\n";
            if((cpd=fork())==0){
                setupPins();
                goForward(99999,MOTORX);
                releasePins();
                exit(0);
            }
        }
		else if (command == Command::MOTOR_X2_START){
			std::cout << "X2 START\n";
            if((cpd=fork())==0){
                setupPins();
                goForward(99999,MOTORX2);
                releasePins();
                exit(0);
            }
        }
		else if (command == Command::MOTOR_Y_START){
			std::cout << "Y START\n";
            if((cpd=fork())==0){
                setupPins();
                goForward(99999,MOTORY);
                releasePins();
                exit(0);
            }
        }
		else if (command == Command::MOTOR_Z_START){
			std::cout << "Z START\n";
            if((cpd=fork())==0){
                setupPins();
                goForward(99999,MOTORZ);
                releasePins();
                exit(0);
            }
        }else if (command == Command::MOTOR_X_STARTR){
			std::cout << "X1 START\n";
            if(cpd!=0)kill(cpd,SIGKILL);
            if((cpd=fork())==0){
                setupPins();
                goBackwardX(99999);
                releasePins();
                exit(0);
            }
        }
		else if (command == Command::MOTOR_X1_STARTR){
			std::cout << "X1 START\n";
            if(cpd!=0)kill(cpd,SIGKILL);
            if((cpd=fork())==0){
                setupPins();
                goBackward(99999,MOTORX);
                releasePins();
                exit(0);
            }
        }
		else if (command == Command::MOTOR_X2_STARTR){
			std::cout << "X2 START\n";
            if(cpd!=0)kill(cpd,SIGKILL);
            if((cpd=fork())==0){
                setupPins();
                goBackward(99999,MOTORX2);
                releasePins();
                exit(0);
            }
        }
		else if (command == Command::MOTOR_Y_STARTR){
			std::cout << "Y START\n";
            if(cpd!=0)kill(cpd,SIGKILL);
            if((cpd=fork())==0){
                setupPins();
                goBackward(99999,MOTORY);
                releasePins();
                exit(0);
            }
        }
		else if (command == Command::MOTOR_Z_STARTR){
			std::cout << "Z START\n";
            if(cpd!=0)kill(cpd,SIGKILL);
            if((cpd=fork())==0){
                setupPins();
                goBackward(99999,MOTORZ);
                releasePins();
                exit(0);
            }
        }
		else if (command == Command::MOTOR_X1_STOP){
			std::cout << "X1 STOP\n";
            if(cpd!=0)kill(cpd,SIGKILL);
        }
        else if (command == Command::MOTOR_X_STOP){
			std::cout << "X1 STOP\n";
            if(cpd!=0)kill(cpd,SIGKILL);
        }
		else if (command == Command::MOTOR_X2_STOP){
			std::cout << "X2 STOP\n";
            if(cpd!=0)kill(cpd,SIGKILL);
        }
		else if (command == Command::MOTOR_Y_STOP){
			std::cout << "Y STOP\n";
            if(cpd!=0)kill(cpd,SIGKILL);
        }
		else if (command == Command::MOTOR_Z_STOP){
			std::cout << "Z STOP\n";
            if(cpd!=0)kill(cpd,SIGKILL);
        }
	}

    
    
}


int i=0;
void touchCallback(){
    //printf("Touched %d\n",++i);
    //fflush(stdout);
    isTouched=1;
}

void sendCoord(int x,int y,int totalSteps){
            double height = (startHeight-totalSteps)/(startHeight*1.0);
            printf("X=%d Y=%d H = %f \n",x,y,height);
            
    /*thread thread1(([client,x,y,height](){
		
	}));*/
	client->sendLocationInfo(x, y, height);
	//thread1.detach();
			
}

int xPoint = 0;
int yPoint = 0;
void calculatePoint(int x,int y){
    heightMap[xPoint][yPoint] = calculateHeight();
    sendCoord(xPoint,yPoint,heightMap[xPoint][yPoint]);
    goUp(heightMap[xPoint][yPoint]);
    /*
    for(int i=0;i<5;i++){
        for(int y=0;y<5;y++){
            
            
            sendCoord(xPoint,yPoint,heightMap[xPoint][yPoint]);
        }
    }*/
   
}

void calculate(){
xPoint = 0;
yPoint = 0;
    while(1){
        calculatePoint(xPoint,yPoint);
        for(int y=1;y<20;y++){
            goForward(100,MOTORY);
            yPoint++;

            calculatePoint(xPoint,yPoint);
        }
        if(xPoint==19)break;

        goForwardX(100);
        xPoint++;

        calculatePoint(xPoint,yPoint);
        for(int y=19;y>0;y--){
            goBackward(100,MOTORY);
            yPoint--;
            calculatePoint(xPoint,yPoint);

        }
        //------------
        isTouched=0;
    while(!isTouched){
        goBackward(10,MOTORY);
    }
    
    goForward(motorYStartPosition,MOTORY);
    if(xPoint==19)break;
    //------------
        goForwardX(100);
        xPoint++;

    }
    
}



void setupPins(){
    if (wiringPiSetup() == -1) {
        cerr << "'Failed to setup wiringPi" << endl;
        exit (EXIT_FAILURE);
    }
    pinMode (pin[0][0], OUTPUT);
    pinMode (pin[0][1], OUTPUT);
    pinMode (pin[0][2], OUTPUT);
    pinMode (pin[0][3], OUTPUT);

    pinMode (pin[1][0], OUTPUT);
    pinMode (pin[1][1], OUTPUT);
    pinMode (pin[1][2], OUTPUT);
    pinMode (pin[1][3], OUTPUT);

    pinMode (pin[2][0], OUTPUT);
    pinMode (pin[2][1], OUTPUT);
    pinMode (pin[2][2], OUTPUT);
    pinMode (pin[2][3], OUTPUT);
    
    pinMode (pin[3][0], OUTPUT);
    pinMode (pin[3][1], OUTPUT);
    pinMode (pin[3][2], OUTPUT);
    pinMode (pin[3][3], OUTPUT);
    pinMode (touchCallbackPin, INPUT);
    wiringPiISR(touchCallbackPin,INT_EDGE_RISING,&touchCallback);

}

void releasePins(){
    setStep(0, 0, 0, 0,MOTORX);
    setStep(0, 0, 0, 0,MOTORY);
    setStep(0, 0, 0, 0,MOTORZ);
}


void setStep(int w1,int w2,int w3,int w4,int motor) {
    digitalWrite(pin[motor][0], w1);
    digitalWrite(pin[motor][1], w2);
    digitalWrite(pin[motor][2], w3);
    digitalWrite(pin[motor][3], w4);
}






void goForwardX(int stepCount) {
      
    thread thread1(([stepCount](){
		goForward(stepCount,MOTORX);
	}));
	
	
	
    thread thread2(([stepCount](){
		goBackward(stepCount,MOTORX2);
	}));
	thread1.join();
	thread2.join();
}


void goBackwardX(int stepCount) {
      thread thread1(([stepCount](){
		goBackward(stepCount,MOTORX);
	}));
	
    thread thread2(([stepCount](){
		goForward(stepCount,MOTORX2);
	}));
	
	thread1.join();
	thread2.join();
}



void goForward(int stepCount,int motor) {
       setStep(0, 0, 0, 0,MOTORX);
    setStep(0, 0, 0, 0,MOTORX2);
    setStep(0, 0, 0, 0,MOTORZ);
    setStep(0, 0, 0, 0,MOTORY);
    int stepsLeft=stepCount;
    while(stepsLeft>0){
        setStep(0, 0, 0, 1,motor);
        usleep(delayMotor);
        setStep(0, 0, 1, 1,motor);
        usleep(delayMotor);
        setStep(0, 0, 1, 0,motor);
        usleep(delayMotor);
        setStep(0, 1, 1, 0,motor);
        usleep(delayMotor);
        
        setStep(0, 1, 0, 0,motor);
        usleep(delayMotor);
        setStep(1, 1, 0, 0,motor);
        usleep(delayMotor);
        setStep(1, 0, 0, 0,motor);
        usleep(delayMotor);
        setStep(1, 0, 0, 0,motor);
        usleep(delayMotor);
        
        
        stepsLeft--;
    }
}



void goBackward(int stepCount,int motor) {
       setStep(0, 0, 0, 0,MOTORX);
    setStep(0, 0, 0, 0,MOTORX2);
    setStep(0, 0, 0, 0,MOTORZ);
    setStep(0, 0, 0, 0,MOTORY);
    int stepsLeft=stepCount;
    while(stepsLeft>0){
		setStep(1, 0, 0, 0,motor);
        usleep(delayMotor);
		setStep(1, 0, 0, 0,motor);
        usleep(delayMotor);
		setStep(1, 1, 0, 0,motor);
        usleep(delayMotor);
        setStep(0, 1, 0, 0,motor);
        usleep(delayMotor);
        
		setStep(0, 1, 1, 0,motor);
        usleep(delayMotor);
        setStep(0, 0, 1, 0,motor);
        usleep(delayMotor);
		setStep(0, 0, 1, 1,motor);
        usleep(delayMotor);
        setStep(0, 0, 0, 1,motor);
        usleep(delayMotor);
        
        
        stepsLeft--;
    }
}



void syncMotors(){
    isTouched=0;
    while(!isTouched){
        goBackward(10,MOTORY);
    }
    goForward(motorYStartPosition,MOTORY);
    
    isTouched=0;
    while(!isTouched){
        goBackwardX(10);
    }
    goForwardX(50);
    isTouched=0;
    while(!isTouched){
        goDown(10);
    }
    goUp(startHeight);
    goForwardX(motorXStartPosition-50);
}

int positive(int number){
if(number<0) return -number;
return number;    
}

int previusHeight=0;
int calculateHeight(){
    setStep(0, 0, 0, 0,MOTORX);
    setStep(0, 0, 0, 0,MOTORX2);
    setStep(0, 0, 0, 0,MOTORZ);
    setStep(0, 0, 0, 0,MOTORY);
    
    isTouched=0;
    int totalSteps = 0;
    while(!isTouched){
        setStep(1, 0, 0, 0,MOTORZ);
        usleep(delayMotor);
		setStep(1, 0, 0, 0,MOTORZ);
        usleep(delayMotor);
		setStep(1, 1, 0, 0,MOTORZ);
        usleep(delayMotor);
        setStep(0, 1, 0, 0,MOTORZ);
        usleep(delayMotor);
        
		setStep(0, 1, 1, 0,MOTORZ);
        usleep(delayMotor);
        setStep(0, 0, 1, 0,MOTORZ);
        usleep(delayMotor);
		setStep(0, 0, 1, 1,MOTORZ);
        usleep(delayMotor);
        setStep(0, 0, 0, 1,MOTORZ);
        usleep(delayMotor);
        totalSteps++;
    }
    
    if(totalSteps>startHeight){
        fprintf(stderr,"Maximum height reached, returning start height\n");
        return startHeight;
    }
    if(positive(totalSteps-previusHeight)<35){
        fprintf(stderr,"%d change is not considerable\n",positive(totalSteps-previusHeight));
        totalSteps = previusHeight;
    }else{
        previusHeight = totalSteps;
    }
    return totalSteps;
}

void goUp(int stepCount){
    goForward(stepCount,MOTORZ);
}

void goDown(int stepCount){
    goBackward(stepCount,MOTORZ);
}

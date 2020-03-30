#include <Servo.h>

#include "Wire.h"  // Arduino Wire library
#include "I2Cdev.h"  //Installer ces 2 librairies
#include "MPU6050.h"
#include "math.h"




///////////////////
// PIN used for Servos. Changed if you need to
///////////////////

#define PIN_RIGHT_FRONT_FOOT 12
#define PIN_LEFT_FRONT_FOOT 8
#define PIN_RIGHT_BACK_FOOT 4
#define PIN_LEFT_BACK_FOOT 6

#define PIN_RIGHT_FRONT_ARM 11
#define PIN_RIGHT_BACK_ARM 3
#define PIN_LEFT_FRONT_ARM 9
#define PIN_LEFT_BACK_ARM 5

#define PIN_RIGHT_FRONT_SHOULDER 10
#define PIN_RIGHT_BACK_SHOULDER 2
#define PIN_LEFT_FRONT_SHOULDER 7
#define PIN_LEFT_BACK_SHOULDER 13



///////////////////
// Rotation sens
///////////////////
#define ROTATION_RIGHT 0
#define ROTATION_LEFT 1


///////////////////
// Body part ID
///////////////////

#define RIGHT_FRONT_FOOT 0
#define LEFT_FRONT_FOOT 1
#define RIGHT_BACK_FOOT 2
#define LEFT_BACK_FOOT 3

#define RIGHT_FRONT_ARM 4
#define RIGHT_BACK_ARM 5
#define LEFT_FRONT_ARM 6
#define LEFT_BACK_ARM 7

#define RIGHT_FRONT_SHOULDER 8
#define RIGHT_BACK_SHOULDER 9
#define LEFT_FRONT_SHOULDER 10
#define LEFT_BACK_SHOULDER 11


///////////////////
// Servos initialisation
///////////////////

Servo ArriereGaucheEpaule;
Servo ArriereGaucheBras;
Servo ArriereGauchePied;

Servo ArriereDroitEpaule;
Servo ArriereDroitBras;
Servo ArriereDroitPied;

Servo AvantDroitEpaule;
Servo AvantDroitBras;
Servo AvantDroitPied;

Servo AvantGaucheEpaule;
Servo AvantGaucheBras;
Servo AvantGauchePied;


///////////////////
// Servos position correction
///////////////////

int CorrAvantDroitPied = -15; // -10
int CorrAvantGauchePied = 0;
int CorrArriereDroitPied = 5; // 5
int CorrArriereGauchePied = 0;
int CorrAvantDroitBras = -5; // -3
int CorrAvantGaucheBras = 5; // 10
int CorrArriereDroitBras = -5; // 5
int CorrArriereGaucheBras = 0; // 5
int CorrAvantDroitEpaule = -20;
int CorrAvantGaucheEpaule = 10;
int CorrArriereDroitEpaule = 2;
int CorrArriereGaucheEpaule = -10;


///////////////////
// Servos actual position
///////////////////

int PosAvantDroitPied = 0;
int PosAvantGauchePied = 0;
int PosArriereDroitPied = 0;
int PosArriereGauchePied = 0;
int PosAvantDroitBras = 0;
int PosAvantGaucheBras = 0;
int PosArriereDroitBras = 0;
int PosArriereGaucheBras = 0;
int PosAvantDroitEpaule = 0;
int PosAvantGaucheEpaule = 0;
int PosArriereDroitEpaule = 0;
int PosArriereGaucheEpaule = 0;


///////////////////
// MPU6050 vars
///////////////////

MPU6050 accelgyro;
 
int16_t ax, ay, az;
int16_t gx, gy, gz;
uint8_t Accel_range;
uint8_t Gyro_range;
float angleFront=0;
float angleSide=0;


///////////////////
// Components status
///////////////////

int servosStatus = 0;
int gyroStatus = 0;


///////////////////
// Message variables
///////////////////
String message = "";
String action = "";


////////////////////////////////////////////////////////////////////////////////////
//////////////                                                        //////////////
//////////////                 FUNCTIONS                              //////////////
//////////////                                                        //////////////
////////////////////////////////////////////////////////////////////////////////////


///////////////////
// MInstead of sleeping for long, update position every 5 ms
///////////////////
void myDelay(int time){

  for(int j=0; j<(time/50); j++){
    updatePosition();
    delay(50);
  }
  delay(time/50);

}


///////////////////
// Move an arm, shoulder or foot
///////////////////

void movearm(int part, int position){

    switch(part){
      case RIGHT_FRONT_FOOT:
        PosAvantDroitPied = position;
        AvantDroitPied.write(PosAvantDroitPied + CorrAvantDroitPied);
        break;
        
      case LEFT_FRONT_FOOT:
        PosAvantGauchePied = position;
        AvantGauchePied.write(180 - PosAvantGauchePied + CorrAvantGauchePied);
        break;
        
      case RIGHT_BACK_FOOT:
        PosArriereDroitPied = position;
        ArriereDroitPied.write(PosArriereDroitPied + CorrArriereDroitPied);
        break; 
        
      case LEFT_BACK_FOOT:
        PosArriereGauchePied = position;
        ArriereGauchePied.write(180 - PosArriereGauchePied + CorrArriereGauchePied);
        break;  

        
      case RIGHT_FRONT_ARM:
        PosAvantDroitBras = position;
        AvantDroitBras.write(180 - PosAvantDroitBras + CorrAvantDroitBras); 
        break;   
                
      case LEFT_FRONT_ARM:
        PosAvantGaucheBras = position;
        AvantGaucheBras.write(PosAvantGaucheBras + CorrAvantGaucheBras);
        break; 
                
      case RIGHT_BACK_ARM:
        PosArriereDroitBras = position;
        ArriereDroitBras.write(180 - PosArriereDroitBras + CorrArriereDroitBras);
        break; 
                
      case LEFT_BACK_ARM:
        PosArriereGaucheBras = position;
        ArriereGaucheBras.write(PosArriereGaucheBras + CorrArriereGaucheBras);
        break;   


      case RIGHT_FRONT_SHOULDER:
        PosAvantDroitEpaule = position ;
        AvantDroitEpaule.write(180 - PosAvantDroitEpaule+ CorrAvantDroitEpaule);
        break;   
                
      case LEFT_FRONT_SHOULDER:
        PosAvantGaucheEpaule = position;
        AvantGaucheEpaule.write(PosAvantGaucheEpaule + CorrAvantGaucheEpaule);
        break; 
                
      case RIGHT_BACK_SHOULDER:
        PosArriereDroitEpaule = position ;
        ArriereDroitEpaule.write(PosArriereDroitEpaule+ CorrArriereDroitEpaule);
        break; 
                
      case LEFT_BACK_SHOULDER:
        PosArriereGaucheEpaule = position;
        ArriereGaucheEpaule.write(180 - PosArriereGaucheEpaule + CorrArriereGaucheEpaule);
        break; 
  
    }

}

///////////////////
// Make Spot to move forward
///////////////////

void move(){

  movearm(RIGHT_FRONT_FOOT, 170);
  movearm(LEFT_BACK_FOOT, 170);
  myDelay(50);
  movearm(RIGHT_FRONT_ARM, 150);
  movearm(LEFT_BACK_ARM, 150); 
  myDelay(100);
  movearm(RIGHT_FRONT_FOOT, 150);
  movearm(LEFT_BACK_FOOT, 150);


  movearm(LEFT_BACK_FOOT, 130);
  movearm(LEFT_FRONT_FOOT, 120);
  movearm(RIGHT_BACK_FOOT, 120);
  movearm(RIGHT_FRONT_FOOT, 130);

  myDelay(200);

  movearm(LEFT_FRONT_FOOT, 170);
  movearm(RIGHT_BACK_FOOT, 170);
  myDelay(50);
  movearm(LEFT_FRONT_ARM, 140);
  movearm(RIGHT_BACK_ARM, 140); 
  myDelay(100);
  movearm(LEFT_FRONT_FOOT, 130);
  movearm(RIGHT_BACK_FOOT, 130);


  movearm(RIGHT_BACK_ARM, 170);
  movearm(LEFT_BACK_ARM, 170);
  movearm(RIGHT_BACK_FOOT, 140);
  movearm(LEFT_BACK_FOOT, 140);
  movearm(RIGHT_FRONT_ARM, 170);
  movearm(LEFT_FRONT_ARM, 170);
  movearm(RIGHT_FRONT_FOOT, 140);
  movearm(LEFT_FRONT_FOOT, 140);
  
  myDelay(100);

}


///////////////////
// Stop position
///////////////////

void stop(){

  movearm(RIGHT_BACK_ARM, 170);
  movearm(LEFT_BACK_ARM, 170);
  movearm(RIGHT_BACK_FOOT, 140);
  movearm(LEFT_BACK_FOOT, 140);
  movearm(RIGHT_FRONT_ARM, 170);
  movearm(LEFT_FRONT_ARM, 170);
  movearm(RIGHT_FRONT_FOOT, 140);
  movearm(LEFT_FRONT_FOOT, 140);  
  movearm(RIGHT_FRONT_SHOULDER, 90);
  movearm(LEFT_FRONT_SHOULDER, 90);
  movearm(RIGHT_BACK_SHOULDER, 90);
  movearm(LEFT_BACK_SHOULDER, 90);

}


void getup(){

  int ValPosAvantDroitBras = PosAvantDroitBras;
  int ValPosAvantGaucheBras = PosAvantGaucheBras;
  int ValPosArriereDroitBras = PosArriereDroitBras;
  int ValPosArriereGaucheBras = PosArriereGaucheBras;
  int ValPosArriereDroitPied = PosArriereDroitPied;
  int ValPosArriereGauchePied = PosArriereGauchePied;
  int ValPosAvantDroitPied = PosAvantDroitPied;
  int ValPosAvantGauchePied = PosAvantGauchePied;

  movearm(RIGHT_FRONT_SHOULDER, 90);
  movearm(LEFT_FRONT_SHOULDER, 90);
  movearm(RIGHT_BACK_SHOULDER, 90);
  movearm(LEFT_BACK_SHOULDER, 90);

  myDelay(100);
  

  for(int i=0; i<10; i++){

    movearm(RIGHT_BACK_ARM, PosArriereDroitBras-round((ValPosArriereDroitBras - 170)/15));
    movearm(LEFT_BACK_ARM, PosArriereGaucheBras-round((ValPosArriereGaucheBras - 170)/15));
    movearm(RIGHT_BACK_FOOT, PosArriereDroitPied-round((ValPosArriereDroitPied - 140)/15));
    movearm(LEFT_BACK_FOOT, PosArriereGauchePied-round((ValPosArriereGauchePied - 140)/15));
  
    myDelay(50);
  
  }

  myDelay(500);
  
  for(int i=0; i<5; i++){

    movearm(RIGHT_FRONT_ARM, PosAvantDroitBras-round((ValPosAvantDroitBras - 170)/5));
    movearm(LEFT_FRONT_ARM, PosAvantGaucheBras-round((ValPosAvantGaucheBras - 170)/5));
    
    movearm(RIGHT_BACK_ARM, PosArriereDroitBras-round((ValPosArriereDroitBras - 170)/15));
    movearm(LEFT_BACK_ARM, PosArriereGaucheBras-round((ValPosArriereGaucheBras - 170)/15));
    movearm(RIGHT_BACK_FOOT, PosArriereDroitPied-round((ValPosArriereDroitPied - 140)/15));
    movearm(LEFT_BACK_FOOT, PosArriereGauchePied-round((ValPosArriereGauchePied - 140)/15));

    movearm(RIGHT_FRONT_FOOT, PosAvantDroitPied-round((ValPosAvantDroitPied - 140)/5));
    movearm(LEFT_FRONT_FOOT, PosAvantGauchePied-round((ValPosAvantGauchePied - 140)/5));
  
    myDelay(50);
  
  }

}


///////////////////
// Lay down
///////////////////

void laydown(){

  int ValPosAvantDroitBras = PosAvantDroitBras;
  int ValPosAvantGaucheBras = PosAvantGaucheBras;
  int ValPosArriereDroitBras = PosArriereDroitBras;
  int ValPosArriereGaucheBras = PosArriereGaucheBras;
  int ValPosArriereDroitPied = PosArriereDroitPied;
  int ValPosArriereGauchePied = PosArriereGauchePied;
  int ValPosAvantDroitPied = PosAvantDroitPied;
  int ValPosAvantGauchePied = PosAvantGauchePied;
  

  for(int i=0; i<10; i++){

    movearm(RIGHT_FRONT_ARM, PosAvantDroitBras-round((ValPosAvantDroitBras - 140)/10));
    movearm(LEFT_FRONT_ARM, PosAvantGaucheBras-round((ValPosAvantGaucheBras - 140)/10));
    movearm(RIGHT_BACK_ARM, PosArriereDroitBras-round((ValPosArriereDroitBras - 140)/10));
    movearm(LEFT_BACK_ARM, PosArriereGaucheBras-round((ValPosArriereGaucheBras - 140)/10));

    movearm(RIGHT_BACK_FOOT, PosArriereDroitPied-round((ValPosArriereDroitPied - 210)/10));
    movearm(LEFT_BACK_FOOT, PosArriereGauchePied-round((ValPosArriereGauchePied - 210)/10));

    movearm(RIGHT_FRONT_FOOT, PosAvantDroitPied-round((ValPosAvantDroitPied - 210)/10));
    movearm(LEFT_FRONT_FOOT, PosAvantGauchePied-round((ValPosAvantGauchePied - 210)/10));
  
    myDelay(50);
  
  }

}


///////////////////
// Turn
///////////////////

void turn(bool sens){

  switch(sens){
  
    case ROTATION_RIGHT:
  
      movearm(RIGHT_BACK_ARM, 180);
      movearm(LEFT_BACK_ARM, 180);
    
      myDelay(200);
    
      movearm(RIGHT_FRONT_SHOULDER, 70);
      movearm(LEFT_FRONT_SHOULDER, 110);
      movearm(RIGHT_BACK_SHOULDER, 110);
      movearm(LEFT_BACK_SHOULDER, 70);
    
      myDelay(300);
    
      movearm(LEFT_BACK_FOOT, 150);
      myDelay(100);
      movearm(LEFT_BACK_SHOULDER, 120);
      
      myDelay(300);
      
    
      movearm(RIGHT_BACK_FOOT, 190);
      myDelay(50);
      movearm(RIGHT_BACK_SHOULDER, 60);
      myDelay(100);
      movearm(RIGHT_BACK_FOOT, 140);
      
      myDelay(300);
    
      movearm(RIGHT_FRONT_SHOULDER, 90);
      movearm(LEFT_FRONT_SHOULDER, 90);
      movearm(RIGHT_BACK_SHOULDER, 90);
      movearm(LEFT_BACK_SHOULDER, 90);  
    
      myDelay(200);
      break;


    case ROTATION_LEFT:
  
      movearm(RIGHT_BACK_ARM, 180);
      movearm(LEFT_BACK_ARM, 180);
    
      myDelay(200);
    
      movearm(RIGHT_FRONT_SHOULDER, 110);
      movearm(LEFT_FRONT_SHOULDER, 70);
      movearm(RIGHT_BACK_SHOULDER, 70);
      movearm(LEFT_BACK_SHOULDER, 110);
    
      myDelay(300);
    
      movearm(RIGHT_BACK_FOOT, 150);
      myDelay(100);
      movearm(RIGHT_BACK_SHOULDER, 120);
      
      myDelay(300);
      
    
      movearm(LEFT_BACK_FOOT, 190);
      myDelay(50);
      movearm(LEFT_BACK_SHOULDER, 60);
      myDelay(100);
      movearm(LEFT_BACK_FOOT, 140);
      
      myDelay(300);
    
      movearm(RIGHT_FRONT_SHOULDER, 90);
      movearm(LEFT_FRONT_SHOULDER, 90);
      movearm(RIGHT_BACK_SHOULDER, 90);
      movearm(LEFT_BACK_SHOULDER, 90);  
    
      myDelay(200);
      break;
      
  }

}


///////////////////
// Start MPU6050
///////////////////

void startGyro(){

  // Wait for the MPU6050 initialization
  for( int j = 0; j<60; j++){
    accelgyro.initialize();
    delay(100);
    if(accelgyro.getDeviceID() == 57){      // testconneciton() doesn't work because it's supposed to find a 52 getDeviceID !
      delay(1000);
      gyroStatus = 1;
      break;
    }
    delay(900);
  }  
}


///////////////////
// Update Front and Side position
///////////////////

void updatePosition(){
   
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  angleFront=0.80*(angleFront+float(gy)*0.01/131) + 0.20*atan2((double)ax,(double)az)*180/PI;
  angleSide=0.80*(angleSide+float(gx)*0.01/131) + 0.20*atan2((double)ay,(double)az)*180/PI;
  
}


///////////////////
// Send status in JSON
///////////////////

void spotStatus(){

  Serial.print("{\"servo\":");Serial.print(servosStatus);
  Serial.print(",\"gyro\":");Serial.print(gyroStatus);
  Serial.print(",\"front\":");Serial.print(angleFront);
  Serial.print(",\"side\":");Serial.print(angleSide);
  Serial.print(",\"frontrightfoot\":");Serial.print(PosAvantDroitPied);
  Serial.print(",\"frontleftfoot\":");Serial.print(PosAvantGauchePied);
  Serial.print(",\"backrightfoot\":");Serial.print(PosArriereDroitPied);
  Serial.print(",\"backleftfoot\":");Serial.print(PosArriereGauchePied);
  Serial.print(",\"frontrightarm\":");Serial.print(PosAvantDroitBras);
  Serial.print(",\"frontleftarm\":");Serial.print(PosAvantGaucheBras);
  Serial.print(",\"backrightarm\":");Serial.print(PosArriereDroitBras);
  Serial.print(",\"backleftarm\":");Serial.print(PosArriereGaucheBras);
  Serial.print(",\"frontrightshoulder\":");Serial.print(PosAvantDroitEpaule);
  Serial.print(",\"frontleftshoulder\":");Serial.print(PosAvantGaucheEpaule);
  Serial.print(",\"backrightshoulder\":");Serial.print(PosArriereDroitEpaule);
  Serial.print(",\"backleftshoulder\":");Serial.print(PosArriereGaucheEpaule);
  Serial.println("}");
  Serial.flush();
  
  myDelay(100);                                                                                                                                                                                                                                                                              
}



///////////////////
// Stay right
///////////////////

void adjust_front(float angle){

  // Go down the front
  if( angle > 2.5 ){
    CorrAvantDroitPied = CorrAvantDroitPied + abs(round(angle/2));
    CorrAvantGauchePied = CorrAvantGauchePied - abs(round(angle/2));
    CorrAvantDroitBras = CorrAvantDroitBras - abs(round(angle/2));
    CorrAvantGaucheBras = CorrAvantGaucheBras + abs(round(angle/2));
    stop();
  }

  // Go up the front
  if( angle < -2.5 ){
    CorrAvantDroitPied = CorrAvantDroitPied - abs(round(angle/2));
    CorrAvantGauchePied = CorrAvantGauchePied + abs(round(angle/2));
    CorrAvantDroitBras = CorrAvantDroitBras + abs(round(angle/2));
    CorrAvantGaucheBras = CorrAvantGaucheBras - abs(round(angle/2));
    stop();
  }
  
}

void adjust_side(float angle){

  // Go down left
  if( angle > 2.5 ){
    
    CorrAvantDroitPied = CorrAvantDroitPied - abs(round(angle/4));
    CorrAvantDroitBras = CorrAvantDroitBras + abs(round(angle/4));
    CorrArriereDroitPied = CorrArriereDroitPied - abs(round(angle/4));
    CorrArriereDroitBras = CorrArriereDroitBras + abs(round(angle/4));

    CorrAvantGauchePied = CorrAvantGauchePied - abs(round(angle/4));
    CorrAvantGaucheBras = CorrAvantGaucheBras + abs(round(angle/4));
    CorrArriereGauchePied = CorrArriereGauchePied - abs(round(angle/4));
    CorrArriereGaucheBras = CorrArriereGaucheBras + abs(round(angle/4));

    stop();
  }

 // Go down right
  if( angle < -2.5 ){

    CorrAvantDroitPied = CorrAvantDroitPied + abs(round(angle/2));
    CorrAvantDroitBras = CorrAvantDroitBras - abs(round(angle/2));
    CorrArriereDroitPied = CorrArriereDroitPied + abs(round(angle/2));
    CorrArriereDroitBras = CorrArriereDroitBras - abs(round(angle/2));

    CorrAvantGauchePied = CorrAvantGauchePied + abs(round(angle/2));
    CorrAvantGaucheBras = CorrAvantGaucheBras - abs(round(angle/2));
    CorrArriereGauchePied = CorrArriereGauchePied + abs(round(angle/2));
    CorrArriereGaucheBras = CorrArriereGaucheBras - abs(round(angle/2));

    stop();
  }
}


void stable(){
    stop();
    while (abs(angleFront) > 2.5 or abs(angleSide) > 2.5){
      adjust_front(angleFront);
      adjust_side(angleSide);
      myDelay(100);
    }
}


void returnOK(){
  Serial.println("{\"result\": 1}");
}

///////////////////
// Execute order received
///////////////////

void execOrder(){

  if (action == "move") {
    move();
    
  }
  else{
    if (action == "stop"){
      stop();
      action = "";
      returnOK();
    }
    else{
      if (action == "stable"){
        stable();
        action = "";
        returnOK();
      }
      else{
        if (action == "status"){
          spotStatus();
          action = "";
          returnOK();
        }
        else{
          if (action == "laydown"){
            laydown();
            action = "";
            returnOK();
          }
          else{
            if (action == "getup"){
              getup();
              action = "";
              returnOK();
            } 
            else{
              if (action == "right"){
                turn(ROTATION_RIGHT);
                action = "";
                returnOK();
              }
              else{
                if (action == "left"){
                  turn(ROTATION_LEFT);
                  action = "";
                  returnOK();
                } 
              }
            }
          }
        }
      }
    }
  }
  

  delay(20);
  
}


///////////////////
// Read order from Raspi
///////////////////

void serialEvent(){

  char inChar = (char)Serial.read();

  if (inChar == '\n') {
    message += '\0';
    action = message;
    message = "";
  }
  else{
    message += inChar;
  }
  
}


///////////////////
// Setup
///////////////////

void setup() {

  Serial.begin (115200);

  ///////////////////
  // Attach Servo
  ///////////////////
  
  ArriereGaucheEpaule.attach (PIN_LEFT_BACK_SHOULDER);
  ArriereGaucheBras.attach (PIN_LEFT_BACK_ARM);
  ArriereGauchePied.attach (PIN_LEFT_BACK_FOOT);

  ArriereDroitEpaule.attach (PIN_RIGHT_BACK_SHOULDER);
  ArriereDroitBras.attach (PIN_RIGHT_BACK_ARM);
  ArriereDroitPied.attach (PIN_RIGHT_BACK_FOOT);

  AvantDroitEpaule.attach (PIN_RIGHT_FRONT_SHOULDER);
  AvantDroitBras.attach (PIN_RIGHT_FRONT_ARM);
  AvantDroitPied.attach (PIN_RIGHT_FRONT_FOOT);
  
  AvantGaucheEpaule.attach (PIN_LEFT_FRONT_SHOULDER);
  AvantGaucheBras.attach (PIN_LEFT_FRONT_ARM);
  AvantGauchePied.attach (PIN_LEFT_FRONT_FOOT);


  servosStatus = 1;

  delay(100);

  // Reserver 200 bits for message String
  message.reserve(200);

  // Begin lay down, then get up
  laydown();
  myDelay(500);
  getup();
  myDelay(500);
  stop();
  

  // Start Gyro
  startGyro();

  delay(1000);
  
}


///////////////////
// Loop
///////////////////

void loop() {

  execOrder();

  updatePosition();
  
  myDelay(50);

}

#include <Servo.h>
#include <ArduinoJson.h>

#include "Wire.h"  // Arduino Wire library
#include "I2Cdev.h"  //Installer ces 2 librairies
#include "MPU6050.h"
#include "math.h"


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

int CorrAvantDroitPied = -5; // -10
int CorrAvantGauchePied = 0;
int CorrArriereDroitPied = 0; // 5
int CorrArriereGauchePied = 0;
int CorrAvantDroitBras = 0; // -3
int CorrAvantGaucheBras = 5; // 10
int CorrArriereDroitBras = 0; // 5
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
        PosAvantDroitPied = position + CorrAvantDroitPied;
        AvantDroitPied.write(PosAvantDroitPied);
        break;
        
      case LEFT_FRONT_FOOT:
        PosAvantGauchePied = 180 - position + CorrAvantGauchePied;
        AvantGauchePied.write(PosAvantGauchePied);
        break;
        
      case RIGHT_BACK_FOOT:
        PosArriereDroitPied = position + CorrArriereDroitPied;
        ArriereDroitPied.write(PosArriereDroitPied);
        break; 
        
      case LEFT_BACK_FOOT:
        PosArriereGauchePied = 180 - position + CorrArriereGauchePied;
        ArriereGauchePied.write(PosArriereGauchePied);
        break;  

        
      case RIGHT_FRONT_ARM:
        PosAvantDroitBras = 180 - position + CorrAvantDroitBras;
        AvantDroitBras.write(PosAvantDroitBras); 
        break;   
                
      case LEFT_FRONT_ARM:
        PosAvantGaucheBras = position + CorrAvantGaucheBras;
        AvantGaucheBras.write(PosAvantGaucheBras);
        break; 
                
      case RIGHT_BACK_ARM:
        PosArriereDroitBras = 180 - position + CorrArriereDroitBras;
        ArriereDroitBras.write(PosArriereDroitBras);
        break; 
                
      case LEFT_BACK_ARM:
        PosArriereGaucheBras = position + CorrArriereGaucheBras;
        ArriereGaucheBras.write(PosArriereGaucheBras);
        break;   


      case RIGHT_FRONT_SHOULDER:
        PosAvantDroitEpaule = 180 - position + CorrAvantDroitEpaule;
        AvantDroitEpaule.write(PosAvantDroitEpaule);
        break;   
                
      case LEFT_FRONT_SHOULDER:
        PosAvantGaucheEpaule = position + CorrAvantGaucheEpaule;
        AvantGaucheEpaule.write(PosAvantGaucheEpaule);
        break; 
                
      case RIGHT_BACK_SHOULDER:
        PosArriereDroitEpaule = position + CorrArriereDroitEpaule;
        ArriereDroitEpaule.write(PosArriereDroitEpaule);
        break; 
                
      case LEFT_BACK_SHOULDER:
        PosArriereGaucheEpaule = 180 - position + CorrArriereGaucheEpaule;
        ArriereGaucheEpaule.write(PosArriereGaucheEpaule);
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
  String message =  String("{\"servo\":" + String(servosStatus) + ",\"gyro\":" + String(gyroStatus) + ",\"front\":" + String(angleFront) + ",\"side\":" + String(angleSide) + ",\"frontrightfoot\":" + String(PosAvantDroitPied) + ",\"frontleftfoot\":" + String(PosAvantGauchePied) + ",\"backrightfoot\":" + String(PosArriereDroitPied) + ",\"backleftfoot\":" + String(PosArriereGauchePied) + ",\"frontrightarm\":" + String(PosAvantDroitBras) + ",\"frontleftarm\":" + String(PosAvantGaucheBras) + ",\"backrightarm\":" + String(PosArriereDroitBras) + ",\"backleftarm\":" + String(PosArriereGaucheBras) + ",\"frontrightshoulder\":" + String(PosAvantDroitEpaule) + ",\"frontleftshoulder\":" + String(PosAvantGaucheEpaule) + ",\"backrightshoulder\":" + String(PosArriereDroitEpaule) + ",\"backleftshoulder\":" + String(PosArriereGaucheEpaule) + "}"); 
  Serial.println(message);                                                                                                                                                                                                                                                                              
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


String serialEvent(){

  String message;
 
  if (Serial.available() > 0) {
    // read the incoming byte:
    message = Serial.readString();
  }
  else{
    message = "";
  }
  
  return(message);
  
}




///////////////////
// Setup
///////////////////

void setup() {

  Serial.begin (115200);

  ///////////////////
  // Attach Servo
  ///////////////////
  
  ArriereGaucheEpaule.attach (13);
  ArriereGaucheBras.attach (5);
  ArriereGauchePied.attach (6);

  ArriereDroitEpaule.attach (2);
  ArriereDroitBras.attach (3);
  ArriereDroitPied.attach (4);

  AvantDroitEpaule.attach (10);
  AvantDroitBras.attach (11);
  AvantDroitPied.attach (12);
  
  AvantGaucheEpaule.attach (7);
  AvantGaucheBras.attach (9);
  AvantGauchePied.attach (8);

  servosStatus = 1;

  delay(100);

  // Begin stand up  
  stop();


  // Start Gyro
  startGyro();

  delay(1000);
  
}


///////////////////
// Loop
///////////////////

int i = 0;
void loop() {

//  Serial.print( "Angle : ");
//  Serial.print(angleFront);
//  Serial.print("  ");
//  Serial.println(angleSide);Serial.flush();

  //spotStatus();

  String action = serialEvent();

  if (action == "move") {
    move();
  }
  else{
    if (action == "stop"){
      adjust_front(angleFront);
      adjust_side(angleSide);
      stop();
    }
    else{
      adjust_front(angleFront);
      adjust_side(angleSide);
      stop();
    }
  }

  updatePosition();
  
  myDelay(100);

}



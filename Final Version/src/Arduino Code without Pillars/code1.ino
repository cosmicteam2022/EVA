#include <Servo.h>
#include "Wire.h"
#include <MPU6050_light.h>
Servo Stearing ;
MPU6050 mpu(Wire);


// Gloable variable

int StopRobot=0;

int normalSpeed = 250;
int turnSpeed = 200;
int reverseSpeed = 100 ;

int mpuvalue =0;
int mpuoldvalue =0;
int mpuoldvalue0  =0;

float delta = 0; // used for tuning the vehicle by calculating the left and right distance 
int counter = 0 ; // to count the number of turns when reach 12 the vehicle stops


int trueAngle = 0;

String state ="9";


// Motors 

#define encodPinA1      2                       // Quadrature encoder A pin
#define encodPinB1      8                       // Quadrature encoder B pin
#define PWM             2
#define M1              3                       // PWM outputs to L298N H-Bridge motor driver module
#define M2              4
#define Motor 44
#define BTN 9

// distance 

#define TRIGGERF 52   //D6
#define ECHOF 53     //D7
#define TRIGGERL 48 //A0
#define ECHOL 49    //A1
#define TRIGGERR 46 //A2
#define ECHOR 47//A3
#define TRIGGERB 50 //A2
#define ECHOB 51



float distance;
float backDistance;
float currentDistance;
float lDistance;
float rDistance;
unsigned long period;

// --- end distance




//*************************************************





void setup() {
  // put your setup code here, to run once:


  Stearing.attach(Motor);

  pinMode(M1,OUTPUT);
  pinMode(M2,OUTPUT);
  pinMode(encodPinA1, INPUT_PULLUP);                  // quadrature encoder input A
  pinMode(encodPinB1, INPUT_PULLUP);                  // quadrature encoder input B
//  attachInterrupt(0, encoder, FALLING);               // update encoder position
  TCCR1B = TCCR1B & 0b11111000 | 1; 
 // set 31KHz PWM to prevent motor noise
  pinMode(BTN,INPUT);

  // distance 

  pinMode(ECHOL,INPUT);
  pinMode(TRIGGERL,OUTPUT);

  pinMode(ECHOB,INPUT);
  pinMode(TRIGGERB,OUTPUT);
  
  pinMode(ECHOF,INPUT);
  pinMode(TRIGGERF,OUTPUT);
  
  pinMode(ECHOR,INPUT);
  pinMode(TRIGGERR,OUTPUT);
  
  // -end distance

  Serial.begin (115200);  
  pinMode(Motor,OUTPUT);

  //mpu
  Wire.end(); // reset I2C 
  Wire.begin();
  mpu.begin();
  mpu.calcOffsets();
  Serial.print(F("MPU6050 status: "));

  resetStearingi() ;
 
 
  



}
//   Distance Fun , to read ultrasonic
 
float obstacleDist(int trig,int echo){
 digitalWrite(trig,LOW);
 delayMicroseconds(3);
 digitalWrite(trig,HIGH);
 delayMicroseconds(10);
 digitalWrite(trig,LOW);
 period=pulseIn(echo,HIGH);
 distance=float(period)/58.8;
  return distance;
  
  }


// End Distance Fun






// functions for dc motor , if output > 0 move forward , else move backward with speed out . 

void Run(int out) {                                // to H-Bridge board
  if (out > 0) {
    analogWrite(PWM,out);
    digitalWrite(M1, HIGH);                             // drive motor CW
    digitalWrite(M2, LOW);
  }
  else {
     analogWrite(PWM,out);
     digitalWrite(M1, LOW);                             // drive motor CW
     digitalWrite(M2, HIGH);                        // drive motor CCW
  }
  
}



void getDistance () // read ultrasonic value
{
  
  
currentDistance = obstacleDist(TRIGGERF,ECHOF); // front ultrasonic
delayMPU(0);
rDistance = obstacleDist(TRIGGERR,ECHOR); // right altrasonic
delayMPU(0);
lDistance = obstacleDist(TRIGGERL,ECHOL); // left ultrasonic
delayMPU(0);



}

void resetStearingi(){ // this is for reset stearing on start moving
  Stearing.write(50);
  delayMPU(10);
  Stearing.write(87);
}
void resetStearingl(){ // this is to reset stearing from left
   Stearing.write(89);
}
void resetStearingr(){ // this is to reset stearing from right 
   Stearing.write(90);
}
void resetStearing(){   // make angle of servo motor = 90 
   Stearing.write(90);
}


void delayMPU(int count) // delay with update mpu every 50 ms
{


for (int  i=0 ; i<count ;i++)
{
   
    mpu.update();
    int tempAngle = mpu.getAngleZ() ;
    if (tempAngle <0){tempAngle  = tempAngle *-1 ;}
    if (tempAngle -mpuoldvalue < 110){
      mpuvalue = tempAngle -mpuoldvalue;
      mpuoldvalue0=mpuoldvalue;
     // Serial.print("\tmpu: ");Serial.println(mpuvalue);
      delay(50);
      }
  
  }
    
}
void delayMPU20(int count) // delay with update mpu every 20 ms
{


for (int  i=0 ; i<count ;i++)
{
   
    mpu.update();
    int tempAngle = mpu.getAngleZ() ;

    if (tempAngle <0){tempAngle  = tempAngle *-1 ;}
 
    if (tempAngle -mpuoldvalue < 110){
      mpuvalue = tempAngle -mpuoldvalue;
      mpuoldvalue0=mpuoldvalue;

      delay(20);
      
     
      
      }
  
  }
    
}

void drive (int speedvar ){
  
  
  
  Run(speedvar);
  delayMPU(2);
  
  
  
  }


void trurnLeft () 
{

      Serial.print("left");
      counter ++;
      Stearing.write(60);
      
      
      while ( mpuvalue < 87   ) // turn until angle =  87
        {
           
         Run(turnSpeed);

         delayMPU(2);
         
        
        
        }
       trueAngle+=90;
       mpuoldvalue+=mpuvalue;
       mpuvalue=0;
       Run(0);
       delayMPU(2);
       resetStearingl();
       Run(normalSpeed);
       
        StopRobot=0;
       if (counter == 12){StopRobot=1;delayMPU(2); }
        


}

void turnRight(){
  
      Serial.print("Right");
      counter ++;
      Stearing.write(116);
      
      
      while ( mpuvalue < 85   ) // turn until angle = 85 which is in real 90 
        {
           
         Run(turnSpeed);

         delayMPU(2);
         
        
        
        }
       trueAngle+=90;
       mpuoldvalue+=mpuvalue;
       mpuvalue=0;
       Run(0);
       delayMPU(2);
       resetStearingr();
       Run(normalSpeed);
       delayMPU(2);
       StopRobot=0;

       if (counter == 12){StopRobot=1;delayMPU(2); }
       
  
  
  
  }


void tuning ()
{
  
  


if ((rDistance) <20  ){
  Run(0);
  Stearing.write(80);
  Run(normalSpeed);
  delayMPU(6);
  Run(0);
  Stearing.write(90);
  Run(normalSpeed);
    

  }
else if ((lDistance) <20 ){

  Run(0);
  Stearing.write(100);
  Run(normalSpeed);
  delayMPU(6);
  Run(0);
  Stearing.write(90);
  Run(normalSpeed);
  
  

  
  }
  
  
  }
  



void loop() {
  // put your main code here, to run repeatedly:



  while (StopRobot==0){
  
    
    
    drive(normalSpeed);
    getDistance();
    delayMPU(4);
    if (currentDistance <= 130.0 )

    {
      
     Run(turnSpeed);
        
    }
    else
    {
      Run(normalSpeed);
      
    }


  if (lDistance>120.0 || currentDistance <80 )
    { 
     trurnLeft();
     
    }
  else if (rDistance>120.0)
  {
    turnRight();
    
  }
    
tuning();


    
    
  }
      drive(0);

}

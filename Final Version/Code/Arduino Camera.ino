#include <Servo.h>
#include "Wire.h"
#include <MPU6050_light.h>
Servo Stearing ;
MPU6050 mpu(Wire);


// Gloable variable

int normalSpeed = 255;
int turnSpeed = 200;
int currentSpeed = 100;
int reverseSpeed = 100 ;
int mpuoldvalue0  =0;
int delayRev =20;
int delayleftturn  = 20;
int isturn=0;

int mpuvalue =0;
int mpuoldvalue =0;


volatile long encoderPos = 0;


int currentAngle = 90; // PWM
int leftAngle = 60;
int rightAngle = 120 ;
int forwardAngle = 90;
float rangle =90; // PWM
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




int x=0;
int counter = 0 ;
//*************************************************

float delta = 0;



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
  Wire.end();
  Wire.begin();
  mpu.begin();
  mpu.calcOffsets();
  Serial.print(F("MPU6050 status: "));

  resetStearingi() ;
 
 
  delay(5000);



}
//   Distance Fun

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






// functions for dc motor

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



void getDistance ()
{
  
  
currentDistance = obstacleDist(TRIGGERF,ECHOF);
rDistance = obstacleDist(TRIGGERR,ECHOR);
lDistance = obstacleDist(TRIGGERL,ECHOL);
backDistance = obstacleDist(TRIGGERB,ECHOB);
delayMPU(1);


}

void resetStearingi(){
  Stearing.write(50);
  delayMPU(10);
  Stearing.write(87);
}
void resetStearingl(){
   Stearing.write(89);
}
void resetStearingr(){
   Stearing.write(90);
}
void resetStearing(){
   Stearing.write(90);
}


void delayMPU(int count)
{


for (int  i=0 ; i<count ;i++)
{
   
    mpu.update();
    int xx= mpu.getAngleZ() ;

    if (xx<0){xx = xx*-1 ;}
 
    if (xx-mpuoldvalue < 110){
      mpuvalue = xx-mpuoldvalue;
      mpuoldvalue0=mpuoldvalue;
    //  Serial.print("\tZ: ");Serial.println(xx);
      
   //   Serial.print("\told: ");Serial.println(mpuoldvalue);
  //    Serial.print("\tmpu: ");Serial.println(mpuvalue);
     // Serial.print("\tmpu: ");Serial.println(mpuvalue);
      delay(50);
      
     
      
      }
  
  }
    
}
void delayMPU20(int count)
{


for (int  i=0 ; i<count ;i++)
{
   
    mpu.update();
    int xx= mpu.getAngleZ() ;

    if (xx<0){xx = xx*-1 ;}
 
    if (xx-mpuoldvalue < 110){
      mpuvalue = xx-mpuoldvalue;
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
      delayMPU(4);
      Stearing.write(60);
      int whilecount =0;
      int whilecount0 =0;

      while ( mpuvalue < 44   )
        {
           
         Run(turnSpeed);

         delayMPU(2);
         whilecount++;
        
        
        }
      Stearing.write(115);
     while ( mpuvalue < 88   )
        {
           
         Run(-1*reverseSpeed);

         delayMPU(2);
         whilecount++;
        
        
        }
       trueAngle+=90;
       mpuoldvalue+=mpuvalue;
       mpuvalue=0;
       Run(0);
       delayMPU(40);
       resetStearingl();
       readState();
       if(backDistance<20){delayMPU(10);}
    
       Run(normalSpeed);

       isturn=1;
        x=0;
        state="9";
        


}

void turnRight(){
  
      Serial.print("Right");
      counter ++;
      Stearing.write(116);
      int whilecount =0;
      
      while ( mpuvalue < 85   )
        {
           
         Run(turnSpeed);

         delayMPU(2);
         whilecount++;
        
        
        }
       trueAngle+=90;
       mpuoldvalue+=mpuvalue;
       mpuvalue=0;
       Run(0);
       delayMPU(2);
       resetStearingr();
       Run(normalSpeed);
       if(backDistance<20){delayMPU(10);}
       else{delayMPU(2);}
       
       x=0;
        
  
  
  
  
  
  
  }


void tuning ()
{
  
  


if ((rDistance) <35   ){
  Run(0);
  Stearing.write(80);
  Run(normalSpeed);
  delayMPU(6);
  Run(0);
  Stearing.write(87);
  Run(normalSpeed);
    

  }
else if ((rDistance) >48){

  Run(0);
  Stearing.write(100);
  Run(normalSpeed);
  delayMPU(6);
  Run(0);
  Stearing.write(87);
  Run(normalSpeed);
  
  

  
  }
  
  
  }
  void tuningr()
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
  
void readState()
{

 if (Serial.available() > 0 && isturn==0) {

    state = Serial.readStringUntil('\n');
      Serial.print("You  ");
      Serial.println(state);
      cases(state);
    //   isturn=1;
      state="9";
      Serial.end();
      Serial.begin(115200);
    
}
}



void loop() {
  // put your main code here, to run repeatedly:



  while (x==1){
  
    

    drive(normalSpeed);
    getDistance();
    
if (lDistance<120.0){tuning();}

  if (lDistance>120.0)
    { 
     if (isturn==0) {trurnLeft();}
     
   
     
    }
    
  else if (rDistance>120.0)
  {
         if (isturn==0) {    turnRight();}
    
  }
   if (backDistance > 150.0 && lDistance<120.0) {isturn=0;}
    



    
    
  }
      drive(0);

}





void cases (String state)
    {
  if (state == "1" ){

  delay(100);  
 
  Stearing.write(120);
  Run(turnSpeed);
  if(rDistance > 45){delayMPU(30);}
  else{delayMPU(20);}
  
  
  Run(0);
  Stearing.write(60);
  Run(turnSpeed);
  delayMPU(44);
  Run(0);
  resetStearingl();
      
      }/*
  else if (state == "2" ){

      
   Run(speed0);
  delay(2500); 
  servo_pwm(120,Motor,currentangle);
  Run(speed0);
  delay(1000);
  Run(0);
  servo_pwm(60,Motor,currentangle);
  Run(speed0);
  delay(1800);
  Run(0);
  servo_pwm(80,Motor,currentangle);
  Run(speed0);
    delay(1000);
 

      
      }
 else if (state == "3" ){

  delay(100);  
 
  servo_pwm(120,Motor,currentangle);
  Run(speed0);
  delay(800);
  Run(0);
  servo_pwm(60,Motor,currentangle);
  Run(speed0);
  delay(1700);
  Run(0);
  servo_pwm(80,Motor,currentangle);
  Run(speed0);
  delay(2500);
      
      }

  else if (state == "4" ){

  delay(100);  
 
  servo_pwm(60,Motor,currentangle);
  Run(speed0);
  delay(1000);
  Run(0);
  servo_pwm(120,Motor,currentangle);
  Run(speed0);
  delay(1700);
  Run(0);
  servo_pwm(85,Motor,currentangle);
  Run(speed0);
  delay(2500);
      
      }

  else if (state == "5" ){

  delay(100);  
 
  Run(speed0);
  delay(2500); 
  servo_pwm(60,Motor,currentangle);
  Run(speed0);
  delay(1300);
  Run(0);
  servo_pwm(120,Motor,currentangle);
  Run(speed0);
  delay(1700);
  Run(0);
  servo_pwm(85,Motor,currentangle);
  Run(speed0);
  delay(1000);
      
      }

 else if (state == "6" ){


 
  servo_pwm(60,Motor,currentangle);
  Run(speed0);
  delay(1200);
  Run(0);
  servo_pwm(120,Motor,currentangle);
  Run(speed0);
  delay(1700);
  Run(0);
  servo_pwm(82,Motor,currentangle);
  Run(speed0);
  delay(2500);
      
      }


 else if (state == "7" ){

  delay(100);  
 
  servo_pwm(130,Motor,currentangle);
  Run(speed0);
  delay(900);
  Run(0);
  servo_pwm(56,Motor,currentangle);
  Run(speed0);
  delay(3300);
  Run(0);
  servo_pwm(84,Motor,currentangle);
  Run(speed0);
  delay(1600);
  servo_pwm(135,Motor,currentangle);
  Run(speed0);
  delay(1000);
  Run(0);
  servo_pwm(80,Motor,currentangle);
  Run(speed0);
  delay(2000);
  Run(0);
  servo_pwm(84,Motor,currentangle);
  Run(speed0);
  
      
      }


 else if (state == "8" ){

  delay(100);  
 
   servo_pwm(56,Motor,currentangle);
  Run(speed0);
  delay(1100);
  Run(0);
  servo_pwm(120,Motor,currentangle);
  Run(speed0);
  delay(3000);
  Run(0);
  servo_pwm(84,Motor,currentangle);
  Run(speed0);
  delay(1600);
  servo_pwm(60,Motor,currentangle);
  Run(speed0);
  delay(1000);
  Run(0);
  servo_pwm(60,Motor,currentangle);
  Run(speed0);
  delay(1300);
  Run(0);
  servo_pwm(100,Motor,currentangle);
  Run(speed0);
      
      }



      */
    
    }

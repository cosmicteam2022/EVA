#include <Servo.h>
#include "Wire.h"
#include <MPU6050_light.h>
Servo Stearing ;
MPU6050 mpu(Wire);


// Gloable variable

int turnSection = 0 ;
int normalSpeed = 250;
int turnSpeed = 210;
int currentSpeed = 100;
int reverseSpeed = 100 ;
int mpuoldvalue0  =0;
int delayRev =20;
int delayleftturn  = 20;
int isturn=0;
int canturn=0;
int actionDone = 0 ;
int mpuvalue =0;
int mpuoldvalue =0;
int tempAngle=0;
int angState=0;
int totAng = 0;
int underTurn=0;
int underStraight=0;
int start=0;



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




int stopVehicle=0;
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



void getForward () // get the front distance
{
 
currentDistance = obstacleDist(TRIGGERF,ECHOF);



}

void getLeft ()  // get the left distance
{
  
  
lDistance = obstacleDist(TRIGGERL,ECHOL);




}

void getRight () // get the right distance
{
  
  
rDistance = obstacleDist(TRIGGERR,ECHOR);




}

void getBack () // get the back distance
{
  
  

backDistance = obstacleDist(TRIGGERB,ECHOB);



}

void resetStearingi(){
  Stearing.write(50);
  delayMPU(10);
  Stearing.write(87);
}
void resetStearingl(){
   Stearing.write(87);
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
    int temp= mpu.getAngleZ() ;

    if (temp<0){temp = temp*-1 ;}
 
    if (temp-mpuoldvalue < 110){
      mpuvalue = temp-mpuoldvalue;
      tempAngle=mpuvalue;
      if( tempAngle<0){tempAngle = -1* tempAngle;}
      mpuoldvalue0=mpuoldvalue;

   //   Serial.print("\tmpu: ");Serial.println(tempAngle);
      delay(50);
      
     
      
      }
  
  }
    
}
void delayMPU20(int count)
{


for (int  i=0 ; i<count ;i++)
{
   
    mpu.update();
    int temp= mpu.getAngleZ() ;

    if (temp<0){temp = temp*-1 ;}
 
    if (temp-mpuoldvalue < 110){
      mpuvalue = temp-mpuoldvalue;
      mpuoldvalue0=mpuoldvalue;

      delay(20);
      
     
      
      }
  
  }
    
}

void drive (int speedvar ){
  
  
  
  Run(speedvar);
  delayMPU(2);
  
  
  
  }




void straightBackRight() 
{

    //  Serial.print("left");
      counter ++;
      delayMPU(4);
      Stearing.write(60);
      int whilecount =0;
      int whilecount0 =0;
      

      Stearing.write(130);


       if (turnSection>4){
        turnSection = 0;
     while ( mpuvalue < 79   )
        {
           
         Run(-1*reverseSpeed);

         delayMPU(2);
         whilecount++;
        
        
        }

}
       else
       {     while ( mpuvalue < 84   )
        {
           
         Run(-1*reverseSpeed);

         delayMPU(2);
         whilecount++;
        
        
        }}

       trueAngle+=90;
       mpuoldvalue+=mpuvalue;
       mpuvalue=0;
       Run(0);
       
       resetStearingl();
       delayMPU(2);
       getBack();
    //   Serial.print(backDistance);
       if (backDistance > 35){Run(-reverseSpeed);delayMPU(4);Run(0);}
       
       if (backDistance <15.0 ) {Run(turnSpeed);delayMPU(4);Run(0);}
       else if (backDistance <30.0 ) {Run(turnSpeed);delayMPU(2);Run(0);}
     //  else if (backDistance <50.0 ) {Run(turnSpeed);delayMPU(10);Run(0);}
       delayMPU(50);
       readState();
      
    
       Run(normalSpeed);

       
        stopVehicle=0;
        state="9";
        


}

void straightBackLeft () 
{

    //  Serial.print("left");
      counter ++;
      delayMPU(4);
      Stearing.write(60);
      int whilecount =0;
      int whilecount0 =0;
      

      Stearing.write(130);


       if (turnSection>4){
        turnSection = 0;
     while ( mpuvalue < 79   )
        {
           
         Run(-1*reverseSpeed);

         delayMPU(2);
         whilecount++;
        
        
        }

}
       else
       {     while ( mpuvalue < 84   )
        {
           
         Run(-1*reverseSpeed);

         delayMPU(2);
         whilecount++;
        
        
        }}

       trueAngle+=90;
       mpuoldvalue+=mpuvalue;
       mpuvalue=0;
       Run(0);
       
       resetStearingl();
       delayMPU(2);
       getBack();
    //   Serial.print(backDistance);
       if (backDistance > 35){Run(-reverseSpeed);delayMPU(4);Run(0);}
       
       if (backDistance <15.0 ) {Run(turnSpeed);delayMPU(4);Run(0);}
       else if (backDistance <30.0 ) {Run(turnSpeed);delayMPU(2);Run(0);}
     //  else if (backDistance <50.0 ) {Run(turnSpeed);delayMPU(10);Run(0);}
       delayMPU(50);
       readState();
      
    
       Run(normalSpeed);

       
        stopVehicle=0;
        state="9";
        


}






void walkTurnLeft(int ang , int theta)
{
          Run(0);
        delayMPU(2);

        Stearing.write(theta);
 
        
        while ( tempAngle < ang   )
        {
           
         Run(turnSpeed);

         delayMPU(2);
         
        
        
        }
       mpuoldvalue+=mpuvalue;
       tempAngle=0;
       angState= -1;
       totAng += -1*ang ;
       

}



void walkTurnRight(int ang , int theta)
{

        Run(0);
        delayMPU(2);
        Stearing.write(theta);
        
        
        while ( tempAngle < ang   )
        {
           
         Run(turnSpeed);

         delayMPU(2);
        
        
        
        }
       mpuoldvalue+=mpuvalue;
       tempAngle=0;
       angState= 1;
       totAng += ang ;
       
        

}
void walkStright(int d)
{

  resetStearingl();
  Run(normalSpeed);
  delayMPU(d);
  angState= 0;

}



  
  

 
  
void readState()
{

 if (Serial.available() > 0 && isturn==0 ) {
      state = Serial.readStringUntil('\n');
      Serial.print(state);
      if(state =="1" || state =="5" ){cases(state);}
      
      
      isturn=1;
      state="9";
      
     
      Serial.end();
      Serial.begin(115200);
      
   
      

      
    
}
}

int isLessThanDelta = 0;
void loop() {
  // put your main code here, to run repeatedly:



     

  while (stopVehicle==0){
  

   


    drive(normalSpeed);
    getForward();
    if (currentDistance < 130){
      getLeft();
      if (lDistance<90.0 ){canturn=0;underTurn = 0;isturn=0;} 
         
      if (lDistance>100.0 && canturn==0)
          { 
            
         // left    counter-clockwise
         getRight();
         if (rDistance < 33){

          getForward();
         if (currentDistance < 100){
           turnSection +=1;
           if (turnSection>4){walkTurnLeft(79,30);turnSection = 0;}
           else{walkTurnLeft(86,40);}
           
           
           resetStearingl();
           Run(-reverseSpeed);
           delayMPU(24);
           Run(0);
           delayMPU(50);
           
           totAng+=90;
           readState();
      
           Run(0);
            getRight();
           
           isLessThanDelta = 0;
           canturn=1;
           underTurn = 1;
           underStraight=0;
       
           
         
     }
         }
     else
     {
          getForward();
         if (currentDistance < 25){

          turnSection +=1;
          straightBackLeft ();
          isLessThanDelta = 0;
          canturn=1;
        
     }
      lDistance=90;
      }
   
     
    }//endleft








    //Right    clockwise


          getRight();
      if (rDistance<90.0 ){canturn=0;underTurn = 0;isturn=0;} 
         
      if (rDistance>100.0 && canturn==0)
          { 
            

         getLeft();
         if (lDistance < 33){

          getForward();
         if (currentDistance < 100){
           turnSection +=1;
           if (turnSection>4){walkTurnRight(79,120);turnSection = 0;}
           else{walkTurnRight(86,110);}
           
           
           resetStearingr();
           Run(-reverseSpeed);
           delayMPU(24);
           Run(0);
           delayMPU(50);
           
           totAng+=90;
           readState();
      
           Run(0);
            getRight();
           
           isLessThanDelta = 0;
           canturn=1;
           underTurn = 1;
           underStraight=0;
       
           
         
     }
         }
     else
     {
          getForward();
         if (currentDistance < 25){

          turnSection +=1;
          straightBackRight ();
          isLessThanDelta = 0;
          canturn=1;
        
     }
      rDistance=90;
      }
   
     
    }//endRight
    
}

    
    
  }
   Run(0);

}


void cases (String state) // the action to take when see pillars
    {

  if (state == "1" ){  //   case for red pillars
 walkTurnRight(30,110);

  walkTurnLeft(30,60);
 walkTurnLeft(16,60);
 
 walkTurnRight(11,110);
 resetStearingl();

      
      }

 else if (state == "5") //  case for green pillars
{walkTurnRight(32,60);

  walkTurnLeft(32,110);
 walkTurnLeft(15,110);
    walkTurnRight(15,65);
    resetStearingl();
  
  }
    }

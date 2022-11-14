#include <Servo.h>
#include "Wire.h"
#include <MPU6050_light.h>
#include <math.h>
Servo Stearing ;
MPU6050 mpu(Wire);

// this is the last version of the code 
// Gloable variable


int normalSpeed = 250;
int turnSpeed = 220;
int currentSpeed = 100;
int reverseSpeed = 50 ;
int isturn=0;
int canTurn=0;
int counterClockWise = 0;  // 0 init  1 clock  2 counterclockwise
int needTurn=0;
int LastState=0;
int CodeMOD=0;

volatile long encoderPos = 0;
String state ="9";
float distance;

// Motors 

#define encodPinA1      2                       // Quadrature encoder A pin
#define encodPinB1      8                       // Quadrature encoder B pin
#define PWM             5
#define M1              3                       // PWM outputs to L298N H-Bridge motor driver module
#define M2              4
#define Motor 44
#define BTN 13
#define MOD 14
#define LEDG 12
#define LEDB 11
#define LEDR 10

// distance 

#define TRIGGERF 52   //D6
#define ECHOF 53      //D7
#define TRIGGERL 48 //A0
#define ECHOL 49    //A1
#define TRIGGERR 50 //A2
#define ECHOR 51    //A3


unsigned long period;

// --- end distance

int x=0;
int counter = 0 ;


void setup() {
  // put your setup code here, to run once:

  pinMode(LEDR,OUTPUT);
  pinMode(LEDG,OUTPUT);
  pinMode(LEDB,OUTPUT);
  pinMode(MOD,INPUT);
  pinMode(BTN,INPUT);



  Stearing.attach(Motor);

  pinMode(M1,OUTPUT);
  pinMode(M2,OUTPUT);
  pinMode(encodPinA1, INPUT_PULLUP);                  // quadrature encoder input A
  pinMode(encodPinB1, INPUT_PULLUP);                  // quadrature encoder input B
  attachInterrupt(0, encoder, FALLING);               // update encoder position
  TCCR1B = TCCR1B & 0b11111000 | 1; 
 // set 31KHz PWM to prevent motor noise
  pinMode(BTN,INPUT);

  // distance 

  pinMode(ECHOL,INPUT);
  pinMode(TRIGGERL,OUTPUT);

 
  
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
 // digitalWrite(LEDB,HIGH);

  resetStearings(); 
 
if (!digitalRead(MOD)){digitalWrite(LEDB,HIGH);CodeMOD=0;}
else{digitalWrite(LEDG,HIGH);CodeMOD=1;}



}
//   Distance Fun


void encoder()  {                                     // pulse and direction, direct port reading to save cycles
  if (PINB & 0b00000001)    encoderPos--;             // if(digitalRead(encodPinB1)==HIGH)   count ++;
  else                      encoderPos++;             // if(digitalRead(encodPinB1)==LOW)   count --;
}

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


// functions to read ultrasonic sensors 
int getForward ()
{


int i = 0; 
int f = 0;
int tmp = 0;
tmp = obstacleDist(TRIGGERF,ECHOF);
while (i < 2)
{
  i++;
tmp = (obstacleDist(TRIGGERF,ECHOF) + tmp)/2;
}
 if (tmp > 250 ) {
    tmp = 150 ;  
  }

return  tmp;
}

int getLeft ()
{

int i = 0; 
int L = 0 ;
int  tmp = obstacleDist(TRIGGERL,ECHOL);
while (i < 2)
{

  i++;
  L=obstacleDist(TRIGGERL,ECHOL);
  if (L>tmp) {tmp=L;}
  
 
}
 
return tmp;
}

int getRight ()
{

int i = 0; 
int tmp = 0;
int R=0;
tmp = obstacleDist(TRIGGERR,ECHOR);
while (i < 2)
{
  i++;
  R=obstacleDist(TRIGGERR,ECHOR);
  if (R>tmp) {tmp=R;}

}
 

return tmp;
}




// end ultrasonic functions






void resetStearingi(){
  Stearing.write(120);
  MyDelay(2);
  if (counterClockWise == 0 || counterClockWise == 1)
  {
  Stearing.write(85);
  }
  else if (counterClockWise ==2)
  {
  Stearing.write(90);
  }
}

void resetStearings(){
  Stearing.write(120);
  MyDelay(2);
   
  Stearing.write(90);
  
}





void straightBack (int delta) 
{

 
  if (LastState == 0 || LastState == 1 ){

      encoderWalkStraight1(50,999);
      encoderPos = 0;
      Stearing.write(45);
      encoderWalkStraight1(53,999);
      encoderTurnBack(67 +delta);  
      resetStearingi();
      encoderWalkStraight1(10,999);
      Run(0);
      MyDelay(2);
      
      }
      else if (LastState == 5 )
      {
        
      encoderWalkStraight1(50,999);
      encoderPos = 0;
      Stearing.write(45);
      encoderWalkStraight1(53,999);      
      encoderTurnBack(10 +delta);   
      resetStearingi();
      encoderWalkStraight1(15,999);
      Run(0);
      MyDelay(2);
      
        }
counter++;
   
}


void straightBackRight (int delta) 
{
  int deltaStrightDifferenceVar = deltaStrightDifference(10);
  if (LastState == 5 || LastState == 0 ){
    
      encoderWalkStraight1(80,999);
      encoderPos = 0;
      Stearing.write(120);
      encoderWalkStraight1(27,999);      
      encoderTurnBackRight(47 +delta);  
      resetStearingi();
      MyDelay(2);
      revEncoderSpeed(25,reverseSpeed);
      }
      else if (LastState == 1 )
      {

      encoderWalkStraight1(50,999);
      encoderPos = 0;
      Stearing.write(120);
      encoderWalkStraight1(27,999);     
      encoderTurnBackRight(35 +delta);
      resetStearingi();
      MyDelay(2);
      revEncoderSpeed(15,reverseSpeed);      
        }
counter++;
}

void turnLeft1(int delta)
{
     // L = .785 * 50 = 39.25   "   "
encoderPos = 0;
    Stearing.write(45);
    drvEncoderSpeed(60,turnSpeed);
    Stearing.write(120);
    double revEncoderVar = 40 + delta  ;
    revEncoderSpeed(revEncoderVar,reverseSpeed);
    resetStearingi();
    MyDelay(2);
    drvEncoderSpeed(20,turnSpeed);
    Run(0);
    counter ++ ;
}

void turnRight1(int delta){
    encoderPos = 0;
    Stearing.write(120);
    drvEncoderSpeed(55,turnSpeed);
    Stearing.write(45);
    double revEncoderVar = 50+ 2*delta ;
    revEncoderSpeed(revEncoderVar,reverseSpeed);
    resetStearingi();
    MyDelay(2);
    drvEncoderSpeed(20,turnSpeed);
    Run(0);
    counter ++ ;
  }

void readState()
{

 if (Serial.available() > 0  ) {
      state = Serial.readStringUntil('\n');
      if(state =="1" || state =="5" ){cases(state);}
      state="9";
      Serial.end();
      Serial.begin(115200);
}
else{cases("1");}
}


void encoderTurnBack(int angle)
{
  int dis = angle * 0.76;
  Stearing.write(120);
  revEncoderSpeed(dis,reverseSpeed);
}

void encoderTurnBackRight(int angle)
{
  int dis = angle * 1.35;
  Stearing.write(45);
  revEncoderSpeed(dis,reverseSpeed);
}

void encoderWalkStraight1(int distance,int side){
   if (counterClockWise == 1 ) {
   int getRightVar =0; 

   drvEncoderSpeed(distance /4,normalSpeed);
   if (LastState == 1 )   // from red pillars
   {
       drvEncoderSpeed(distance /4,normalSpeed);
       getRightVar = getRight();
          
       if (side !=999 )
       {
        
          if (getRightVar<15){Stearing.write(45);MyDelay(3);Stearing.write(120);MyDelay(1);resetStearingi();}
          else if (getRightVar>25){Stearing.write(110);MyDelay(2);Stearing.write(60);MyDelay(1);resetStearingi();}
       }
       
       drvEncoderSpeed(distance /2,normalSpeed);

       }
      else if (LastState==5)
       {     

        int  getLeftVar = getLeft();
        drvEncoderSpeed(distance /4,normalSpeed);
        getLeftVar = getLeft();  
        if (side !=999  )
            {      
             if (getLeftVar<17 ){  Stearing.write(120);MyDelay(1);Stearing.write(60);MyDelay(1);resetStearingi();}
            }
        drvEncoderSpeed(distance /2,normalSpeed);
        
        }
        else
        {    
         drvEncoderSpeed(distance, normalSpeed);
        }
   }



   //********************************************


   
   else if (counterClockWise == 2 ) {

    int getLeftVar =0; 

   drvEncoderSpeed(distance /4,normalSpeed);
   if (LastState == 5 )   // from red pillars
   {
       drvEncoderSpeed(distance /4,normalSpeed);
       getLeftVar = getLeft();
       if (side !=999 )
       {
        
          if (getLeftVar<15){  Stearing.write(120);MyDelay(2);Stearing.write(60);MyDelay(2);resetStearingi();}
          else if (getLeftVar>20){  Stearing.write(60);MyDelay(3);Stearing.write(120);MyDelay(2);resetStearingi();}
       }


       drvEncoderSpeed(distance /2,normalSpeed);

       }
      else if (LastState==1)
       {    
 
        int  getRightVar = getRight();
        drvEncoderSpeed(distance /2,normalSpeed);
        getRightVar = getRight();
        if (side !=999  )
            {      
              if (getRightVar<17 ){Stearing.write(60);MyDelay(3);Stearing.write(120);MyDelay(2);resetStearingi();}
              if (getRightVar>22 ){Stearing.write(120);MyDelay(3);Stearing.write(60);MyDelay(2);resetStearingi();}
             }
        drvEncoderSpeed(distance /2,normalSpeed);

        }
        else
        {    
         drvEncoderSpeed(distance ,normalSpeed);
        }
   }
  }

void encoderWalkStraight(int distance,int side, int speed0){
   if (counterClockWise == 1 ) {
  
  int getLeftVar; 
  int  getRightVar;
  int getFrontVar;
   
  drvEncoderSpeed(distance /2,speed0);
       
   
       
       getLeftVar = getLeft(); 
          
       if ( getLeftVar <20 && side != 999)
       {
   
         Stearing.write(110);MyDelay(2);Stearing.write(60);MyDelay(1);resetStearingi();
        
          
         
       }
       else {getRightVar = getRight();
        
        if (getRightVar <17 && side != 999 ){   
          
          Stearing.write(45);MyDelay(3);Stearing.write(120);MyDelay(1);resetStearingi();}
       }


       drvEncoderSpeed(distance /2,speed0);
       getLeftVar = getLeft(); 
          
     
       

   }
   //********************************************


   
   else if (counterClockWise == 2 ) {

    int getLeftVar; 
    int  getRightVar;
    int getFrontVar;
   
       drvEncoderSpeed(distance /2,speed0);
       
       
       getRightVar = getRight();
       if (getRightVar <25 && side != 999)
       {
         Stearing.write(45);MyDelay(3);Stearing.write(100);MyDelay(1);resetStearingi();
        
          
         
       }
       else {getLeftVar = getLeft();
       
        
        if (getLeftVar <13 && side != 999){ 
          
         
        Stearing.write(105);MyDelay(2);Stearing.write(60);MyDelay(1);resetStearingi();
       }
      
       }
      


       drvEncoderSpeed(distance /2,speed0);
     
       
       
   }
  }
int deltaStrightDifference(int edgeThreshold)    // to calculate distance between vehicle and outer edge
{

  int delta = getForward()-edgeThreshold ; 
  
    if (delta > 0)
    {

      return delta ; 
    }
    else 
    {
      return 0 ;
    }
}



int CalculateWalkDifferenceDegree ( double disSide1,double disSide2,double encoderDis)
{
  int result  = asin((disSide1 - disSide2)/encoderDis)*(180/3.14);
  

  return (result);
}

void turnLeft(int delta)
{
     
  
    double revEncoderVar = 84+ delta ;
    Stearing.write(30);
    drvEncoderSpeed(revEncoderVar,turnSpeed);
    resetStearingi();
    MyDelay(2);
    counter ++ ;
    encoderWalkStraight(50,999,normalSpeed);
  
}

void turnRight(int delta)


{
    double revEncoderVar = 82+ delta ;
    Stearing.write(120);
    drvEncoderSpeed(revEncoderVar,turnSpeed);
    resetStearingi();
    MyDelay(2);
    counter ++ ;
    encoderWalkStraight(50,999,normalSpeed);
  
  
  
  
  }

void MyDelay(int cct)
{
  for (int i=0;i<cct;i++)
  {
    delay(50);
    }
  }


void drvEncoderSpeed(int cm,int speed0)
{
  encoderPos=0;
if (cm>25){
float enc = (cm-20)/0.077;
 
while (encoderPos < int(enc))
{
  Run(speed0);
  }

}
else 
{
float enc = (cm)/0.23;
while (encoderPos < int(enc))
{
  Run(speed0);
  }
  
  
}
}

void revEncoderSpeed(int cm,int speed0)
{
 encoderPos=0;
if (cm>25){
float enc = (cm-20)/0.077;
 
while (encoderPos < int(enc))
{
  Run(-speed0);
  }

}
else 
{
float enc = (cm)/0.23;
while (encoderPos < int(enc))
{
  Run(-speed0);
  }
  
  
}
}



void drvEncoderSpeed1(int cm,int speed0)
{
  encoderPos=0;
if (cm>25){
float enc = 2.3*((cm-20)/0.077);
 
while (encoderPos < int(enc))
{
  Run(speed0);
  }

}
else 
{
float enc = 2.3*((cm)/0.23);
while (encoderPos < int(enc))
{
  Run(speed0);
  }
  
  
}
}

void revEncoderSpeed1(int cm,int speed0)
{
 encoderPos=0;
if (cm>25){
float enc = 2*((cm-20)/0.077);
 
while (encoderPos < int(enc))
{
  Run(-speed0);
  }

}
else 
{
float enc = 2*((cm)/0.23);
while (encoderPos < int(enc))
{
  Run(-speed0);
  }
  
  
}
}

double tempEdgeDistance0 = 0;
double tempEdgeDistance1 = 0;



void loop() {




  
  while(x==6){
    int getRightVar = getRight() ;
    Serial.println(getRightVar);
   // MyDelay(4);
    double d= 1.414 * getRightVar ;
    Stearing.write(120);
    drvEncoderSpeed1((d/4),turnSpeed);
    drvEncoderSpeed1((d/5),turnSpeed);
    resetStearingi();
    drvEncoderSpeed1((d/3),turnSpeed);
    drvEncoderSpeed1((d/10),turnSpeed);  
    Stearing.write(45);
    drvEncoderSpeed1((d/3),turnSpeed);
     drvEncoderSpeed1((d/4),turnSpeed);
    resetStearingi();
    drvEncoderSpeed1(10,turnSpeed);
    LastState = 1;
    x=17;
    
  }
  
  // put your main code here, to run repeatedly:

        int z=0;int rpt = 0;int g =0;

        while (x==10){
      //  drvEncoder (40);
        turnLeft(0)
        ;x=2;}

        
if (!digitalRead(BTN) && CodeMOD ==0){digitalWrite(LEDG,HIGH);
   MyDelay(20);
    normalSpeed = 250;
    turnSpeed = 220;
      
      while (x==0){


 

        
              while (counterClockWise == 0) {
              if (getLeft()>80.0) {turnLeft(10);counterClockWise = 1; tempEdgeDistance0 = getRight();tempEdgeDistance1 = getRight(); drvEncoderSpeed(5,turnSpeed); }
              else if (getRight()>80.0 ){turnRight(10);counterClockWise = 2;tempEdgeDistance0 = getLeft();tempEdgeDistance1 = getLeft();drvEncoderSpeed(5,turnSpeed);  }
              else if (getForward()<40){Run(0);encoderWalkStraight(2,999,turnSpeed);encoderPos = 0;}
              else {Run(turnSpeed);encoderPos = 0; }
              continue;
              }
        

            

                   
            if ((counterClockWise == 1))
                  {
                    
               if (g< 100){g= getLeft();}


               if (g>80.0)
                    {  g=0;
                     tempEdgeDistance1 = getRight();      
                     int t = CalculateWalkDifferenceDegree(tempEdgeDistance0,tempEdgeDistance1,100); 


                     turnLeft(t+10);
                     tempEdgeDistance0 = getRight();
 
                      }
                 else
                {
                  int getForwardVar =getForward();
                  if (getForwardVar<70){ encoderWalkStraight(4,999,turnSpeed);g= getLeft();MyDelay(1);}
                  else{  encoderWalkStraight(4,9999,normalSpeed); tempEdgeDistance1 = getRight(); }
                  
                  
                  
                  
                }
                if (counter > 11 ) 
                         {
                            int stopDistance = deltaStrightDifference(100);
                            drvEncoderSpeed((stopDistance/2),normalSpeed);
                            x=1;
                            break;
                          }
                 }
                 
                 //endleft




                 
                 //**********************
                 
            else if (counterClockWise == 2 )
            {
              
               if (g< 100){g= getRight();}


               if (g>80.0)
                    {  g=0;
                     tempEdgeDistance1 = getLeft();      
                     int t = CalculateWalkDifferenceDegree(tempEdgeDistance0,tempEdgeDistance1,100); 

                     Serial.println(t);
                     turnRight(t+10);
                     tempEdgeDistance0 = getLeft();
 
                      }
                 else
                {
                  int getForwardVar =getForward();
                  if (getForwardVar<70){ encoderWalkStraight(4,999,turnSpeed);g= getRight();MyDelay(1);}
                  else{  encoderWalkStraight(4,9999,normalSpeed);}
                  
                  
                  
                  
                }
                if (counter > 11 ) 
                         {
                            int stopDistance = deltaStrightDifference(100);
                            drvEncoderSpeed((stopDistance/2)+18,normalSpeed);
                            x=1;
                            break;
                          }
                 
                  
                  
  
              
            }
         





            
         
           

           
          }
          

}

//****************************** Stage 2 **************************************************/////////



if (!digitalRead(BTN) && CodeMOD == 1)
{
 normalSpeed = 190;
 turnSpeed = 170;
 reverseSpeed = 70 ;
 int g =0;
    MyDelay(20);
    resetStearingi();
       digitalWrite(LEDG,HIGH);
       while (x==0){
              while (counterClockWise == 0) {
              if (getLeft()>80.0) {turnLeft1(0);MyDelay(20);counterClockWise = 1; readState();tempEdgeDistance0 = getRight();tempEdgeDistance1 = getRight(); drvEncoderSpeed(5,turnSpeed); }
              else if (getRight()>80.0 ){turnRight(10);counterClockWise = 2;tempEdgeDistance0 = getLeft();tempEdgeDistance1 = getLeft();drvEncoderSpeed(5,turnSpeed);  }
              else if (getForward()<40){Run(0);encoderWalkStraight(2,999,turnSpeed);encoderPos = 0;}
              else {Run(turnSpeed);encoderPos = 0; }
              continue;
              }
        

            

                   
            if ((counterClockWise == 1))
                  {
                    
               if (g< 100){g= getLeft();}


               if (g>120.0)
                    {  g=0;
                     tempEdgeDistance1 = getRight();      
                     int t = CalculateWalkDifferenceDegree(tempEdgeDistance0,tempEdgeDistance1,100); 


                     
                      if (tempEdgeDistance1<40  &&  (LastState == 1 || LastState == 0) )
                      {
                       
                   //   drvEncoderSpeed(30,turnSpeed);
                      turnLeft1(0); 
                      }
                      else 
                      {
                        
                       straightBack(0);
                      }
                     tempEdgeDistance0 = getRight();
                     MyDelay(20);
                   //   cases("5");
                      readState();
 
                      }
                 else
                {
                  int getForwardVar =getForward();
                  if (getForwardVar<100){ Run(0); MyDelay(1);encoderWalkStraight1(2,999);g= getLeft();MyDelay(1);Serial.println("dd");}
                  else{  encoderWalkStraight1(4,9999); tempEdgeDistance1 = getRight(); }
                  
                  
                  
                  
                }
                if (counter > 11 ) 
                         {
                            int stopDistance = deltaStrightDifference(100);
                            drvEncoderSpeed((stopDistance/2),normalSpeed);
                            x=1;
                            break;
                          }
                 }
          
      }}


Run(0);
}



void cases (String state)
    {



  if (state == "5")
    {
      if (counterClockWise == 1 ) {
     int getRightVar = getRight() ;
     double d= 135- (1.414 * getRightVar );
     Stearing.write(45);
     drvEncoderSpeed(d/2,turnSpeed);
     resetStearingi();
     drvEncoderSpeed((d/4),turnSpeed);
     drvEncoderSpeed((d/14),turnSpeed);
     Stearing.write(125);
     drvEncoderSpeed((d/4),turnSpeed);
     drvEncoderSpeed((d/8),turnSpeed);
     LastState = 5 ;
     resetStearingi();
     drvEncoderSpeed(10,turnSpeed);
    }
    else if (counterClockWise == 2 ) {


     int getLeftVar = getLeft() ;
     double d=(1.414 * getLeftVar )-7;
     Stearing.write(45);
     drvEncoderSpeed((d/2),turnSpeed);
     drvEncoderSpeed((d/8),turnSpeed);
     resetStearingi();
     drvEncoderSpeed((d/2),turnSpeed);

     Stearing.write(135);
     drvEncoderSpeed((d/5),turnSpeed);
     drvEncoderSpeed((d/5),turnSpeed);
     LastState = 5 ;
     resetStearingi();
     drvEncoderSpeed(10,turnSpeed);
      
    }
    
    }
    
 // if (state == "1" )
 else
  {
    if (counterClockWise == 1 ){
     int getRightVar = getRight() ;
    double d= 1.414 * getRightVar -7;
    Stearing.write(120);
    drvEncoderSpeed1((d/4),turnSpeed);
    drvEncoderSpeed1((d/5),turnSpeed);
    resetStearingi();
    drvEncoderSpeed1((d/3),turnSpeed);
    drvEncoderSpeed1((d/10),turnSpeed);  
    Stearing.write(45);
    drvEncoderSpeed1((d/3),turnSpeed);
     drvEncoderSpeed1((d/5),turnSpeed);
    resetStearingi();
        drvEncoderSpeed(20,turnSpeed);
    LastState = 1;
    
   }


   //******
   else if (counterClockWise == 2 ){

    int getLeftVar = getLeft() ;
    double d= 127-(1.414 * getLeftVar );
    Stearing.write(135);
    drvEncoderSpeed((d/2),turnSpeed);
    resetStearingi();
    drvEncoderSpeed((d*2/5)-5,turnSpeed); 
    Stearing.write(45);
    drvEncoderSpeed((d*2/4)+2,turnSpeed);
    resetStearingi();
    drvEncoderSpeed(20,turnSpeed);
    LastState = 1;
    
   }
  }
    
} // END CASE

 

 





 

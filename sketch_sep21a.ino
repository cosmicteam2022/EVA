#define TRIGGERF 6   //D6
#define ECHOF 7      //D7
#define TRIGGERL 14 //A0
#define ECHOL 15    //A1
#define TRIGGERR 16 //A2
#define ECHOR 17    //A3


float distance;
unsigned long period;
void setup() {
  pinMode(ECHOL,INPUT);
  pinMode(TRIGGERL,OUTPUT);
  
  pinMode(ECHOF,INPUT);
  pinMode(TRIGGERF,OUTPUT);
  
  pinMode(ECHOR,INPUT);
  pinMode(TRIGGERR,OUTPUT);
  
  Serial.begin(115200);
}

void loop() {

Serial.print("F : ");Serial.print(obstacleDist(TRIGGERF,ECHOF));
Serial.print("   R : ");Serial.print(obstacleDist(TRIGGERR,ECHOR));
Serial.print("   L : ");Serial.println(obstacleDist(TRIGGERL,ECHOL));
delay (500);
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

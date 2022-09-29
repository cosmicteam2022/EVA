 /* Color Sensor Calibration
  color-sensor-calib.ino
  Calibrate RGB Color Sensor output Pulse Widths
  Uses values obtained for RGB Sensor Demo sketch 
 
  DroneBot Workshop 2020
  https://dronebotworkshop.com
*/
 
// Define color sensor pins
#define BTN 9
#define S2 11
#define S3 12
#define sensorOut 13
 
// Variables for Color Pulse Width Measurements
String COLOR[]={"UnKnown","White","Orange","Blue"} ;
int r = 0;
int g = 0;
int b = 0;
bool state=true;
byte Color=0;
void setup() {
 
  // Set S2 - S3 as outputs

  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(BTN,INPUT);
  
  // Set Sensor output as input
  pinMode(sensorOut, INPUT);
  

 
  
  // Setup Serial Monitor
  Serial.begin(115200);
}
 
void loop() {
  if(!digitalRead(BTN)){state=!state; delay(300);}
  if(state){     
  Color=readColor();
  if( Color==0){Serial.println("White");}
  else if (Color==1){Serial.println("Blue");}
  else if (Color==2){Serial.println("Orange");}
  }
  //delay(300);
  
}
 
 
// Function to read Red Pulse Widths
int getRedPW() {
  
  // Set sensor to read Red only
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
 
}
 
// Function to read Green Pulse Widths
int getGreenPW() {
 
  // Set sensor to read Green only
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
 
}
 
// Function to read Blue Pulse Widths
int getBluePW() {
 
  // Set sensor to read Blue only
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
 
}

byte readColor(){
  byte color=0;
  
   // Read Red Pulse Width
  r = getRedPW();
  // Delay to stabilize sensor
  delay(30);
  
  // Read Green Pur,int glse Width
  g = getGreenPW();
  // Delay to stabilize sensor
  delay(30);
  
  // Read Blue Pulse Width
  b = getBluePW();
  // Delay to stabilize sensor
  delay(30);
    // Print output to Serial Monitor
  
  Serial.print("Red PW = ");
  Serial.print(r);
  Serial.print(" - Green PW = ");
  Serial.print(g);
  Serial.print(" - Blue PW = ");
  Serial.print(b);
    Serial.print(" : ");
  
  if(r<30 && g<30  && b <30){color=0;}
  //if (r>70 && b>50 ){color=1;}
 else if (r>25 && g>30 && b>30 ){color=1;}
 // else if(g>60 && r<g){ color=2;}
   else if(r<30 && g>30 && b>30){ color=2;}




  return color;

  
  }

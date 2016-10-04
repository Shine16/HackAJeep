
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,3); // RX, TX

#define BRAKEVCC 0
#define CW   1
#define CCW  2
#define BRAKEGND 3
#define CS_THRESHOLD 100


//can be changed ######################

int sensitivity = 10; //greater number would be less sensitive, for value of 10 joy stick will not respond till beyond 10

int maximumspeed=1023;// 0 (min) to 1023(max) for max speed possible for both motors
int del=20;// same as 25ms delay of bluetooth app refresh
int accel=5; //more accel, faster it will reach 1023 speed
//can be changed ######################




int lowerend = 200-sensitivity;
int higherend = 200+sensitivity;

String inString = "";    // string to hold input
int  LR=0;
int  FB=0;
int L,R;

int inApin[2] = {7, 4};  // INA: Clockwise input
int inBpin[2] = {8, 9}; // INB: Counter-clockwise input
int pwmpin[2] = {5, 6}; // PWM input
int cspin[2] = {2, 3}; // CS: Current sense ANALOG input
int enpin[2] = {0, 1}; // EN: Status of switches output (Analog pin)

int statpin = 13;
int prevSpeedL=0;
int prevSpeedR=0;
int currSpeedL=0;
int currSpeedR=0;

void setup()
{
  Serial.begin(9600);
 
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  
  pinMode(statpin, OUTPUT);


  // Initialize digital pins as outputs
  for (int i=0; i<2; i++)
  {
    pinMode(inApin[i], OUTPUT); digitalWrite(inApin[i], LOW);  // Initialize braked
    pinMode(inBpin[i], OUTPUT); digitalWrite(inBpin[i], LOW); // Initialize braked
   // pinMode(pwmpin[i], OUTPUT);
  }


  
}

void loop()
{
  
 while (mySerial.available() > 0) {
    int inChar = mySerial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char
      // and add it to the string:
      //inString = "";
      inString += (char)inChar;
      if(inString.length()==6){
        
         
          LR=int(inString[0]-48)*100+int(inString[1]-48)*10+int(inString[2]-48);
          FB=int(inString[3]-48)*100+int(inString[4]-48)*10+int(inString[5]-48);
          
          
          
          Serial.print("Left/Right ");
          Serial.print(LR);          
          Serial.print(" Forward/back ");
          Serial.print(FB);
          Serial.print("    String: ");
          Serial.print(inString);         
          Serial.print("  String to int: ");         
          Serial.println(inString.toInt());    
          
          // clear the string for new input:
          inString = "";
          
          motordriveV2();//send values to motors
          while( mySerial.available()){ inChar = mySerial.read();}
      }
    }
    
    else{inString ="";}
  }
  
  
  

}


void motordriveV2(){
  
    if(FB<lowerend || FB>higherend || LR<lowerend || LR>higherend){
    Serial.print("Received ");Serial.print(LR);Serial.print(", ");Serial.println(FB);
    FB = map(FB,100,300,-maximumspeed,maximumspeed);
    LR = map(LR,100,300,-maximumspeed,maximumspeed); 
          
   
     L=FB+LR;  L = constrain(L, -maximumspeed,maximumspeed);
     R=FB-LR;  R = constrain(R, -maximumspeed,maximumspeed);  
     Serial.print("Target ");Serial.print(L);Serial.print(", ");Serial.println(R);  
     
     
       for(int i = 0 ; i< 5; i++){
          if(currSpeedL<L)       currSpeedL=currSpeedL+accel;
          else if(currSpeedL>L)  currSpeedL=currSpeedL-accel;
          if(currSpeedR<R)       currSpeedR=currSpeedR+accel;
          else if(currSpeedR>R)  currSpeedR=currSpeedR-accel;
          
           currSpeedL = constrain(currSpeedL, -maximumspeed,maximumspeed);
           currSpeedR = constrain(currSpeedR, -maximumspeed,maximumspeed);
          if(L>0)    motorGo(0, CW , currSpeedL); else motorGo(0, CCW , -currSpeedL); 
          if(R>0)    motorGo(1, CCW, currSpeedR); else motorGo(1, CW , -currSpeedR); 
          delay(3);
          Serial.print("Sent Motor ");Serial.print(currSpeedL);Serial.print(", ");Serial.println(currSpeedR); 
        }
    
   
    //L=R=0;
    
  }
  
  else  {
    motorOff();
    currSpeedR=currSpeedL=0;
  }
  
}


void motortest(){
    
  motorGo(0, CW, 1023);
  motorGo(1, CCW, 1023);
  delay(500);

  motorGo(0, CCW, 1023);
  motorGo(1, CW, 1023);
  delay(500);
  
  if ((analogRead(cspin[0]) < CS_THRESHOLD) && (analogRead(cspin[1]) < CS_THRESHOLD))
    digitalWrite(statpin, HIGH);
}

void motorOff()//int motor)
{
  // Initialize braked
  for (int i=0; i<2; i++)
  {
    digitalWrite(inApin[i], LOW);
    digitalWrite(inBpin[i], LOW);
    analogWrite(pwmpin[i], 0);
  }
  //analogWrite(pwmpin[motor], 0);

}



void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)
{
  if (motor <= 1)
  {
    if (direct <=4)
    {
      // Set inA[motor]
      if (direct <=1)
        digitalWrite(inApin[motor], HIGH);
      else
        digitalWrite(inApin[motor], LOW);

      // Set inB[motor]
      if ((direct==0)||(direct==2))
        digitalWrite(inBpin[motor], HIGH);
      else
        digitalWrite(inBpin[motor], LOW);

      analogWrite(pwmpin[motor], pwm);
    }
  }
}


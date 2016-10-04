
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10,11);


//for bluetooth
String inString = "";    // string to hold input
int  LR=0;
int  FB=0;
int del=50;

int tolerance=20;

void setup() {                

  Serial.begin(9600);
  mySerial.begin(9600);

  for(int i=2;i<=5;i++)
  pinMode(i, OUTPUT);     
  
  for(int i=6;i<=9;i++)
  {
    pinMode(i, INPUT);
    digitalWrite(i,HIGH); //pullup resistor
  } //6 to 9 are buttons 

//pin to direction Up, Down, Left, Right
// 2-U , 3-D , 4-L , 5-R


}

// the loop routine runs over and over again forever:
void loop() {

  
  //reset all low again
  for(int i=2;i<=5;i++)
  digitalWrite(i, LOW);   


ReadBLE();
if(LR>(100+tolerance)&&LR<(300-tolerance)&&FB>(100+tolerance)&&FB<(300-tolerance)){
   ButtonControl();
}
else{
  
  
  if(LR<(100+tolerance))//L
  {
    while(LR<(100+tolerance))//L
      {
        digitalWrite(5,HIGH);
        delay(del);
        ReadBLE();
      }
  }
  
  if(LR>(300-tolerance))//R
  {
    while(LR>(300-tolerance))//R
    {
        digitalWrite(4,HIGH);
        delay(del);
        ReadBLE();
    }
  }
  if(FB<(100+tolerance))//B
  {
    while(FB<(100+tolerance))//B
    {
        digitalWrite(3,HIGH);
        delay(del);
        ReadBLE();
    }
  }
  if(FB>(300-tolerance))//F
  {
    while(FB>(300-tolerance))//F
    {
        digitalWrite(2,HIGH);
        delay(del);
        ReadBLE();
    }
  }
}
  
}

void ReadBLE(){
  while (mySerial.available() > 0) {
    int inChar = mySerial.read();
    if (isDigit(inChar)) {
      
      inString += (char)inChar;
      if(inString.length()==6){
      
        LR=int(inString[0]-48)*100+int(inString[1]-48)*10+int(inString[2]-48);
          FB=int(inString[3]-48)*100+int(inString[4]-48)*10+int(inString[5]-48);
          
          /*
          Serial.print("Left/Right ");
          Serial.print(LR); //L is 100, R is 300 
          Serial.print(" Forward/back ");
          Serial.print(FB); //F is 300, B is 100
          Serial.println();
          */
          // clear the string for new input:
          inString = "";
          while( mySerial.available()){ inChar = mySerial.read();}
      }
    }
    
    else{inString ="";
    }
  }
 
}


void ButtonControl(){
  
 if(digitalRead(6)==LOW){
   while(digitalRead(6)==LOW){
    digitalWrite(2,HIGH);Serial.println("U");
   }}
 
 if(digitalRead(7)==LOW){
   while(digitalRead(7)==LOW){
    digitalWrite(3,HIGH);Serial.println("D");
   }}

 if(digitalRead(8)==LOW){
   while(digitalRead(8)==LOW){
    digitalWrite(4,HIGH);Serial.println("L");
   }}

 if(digitalRead(9)==LOW){
   while(digitalRead(9)==LOW){
    digitalWrite(5,HIGH);Serial.println("R");
   }}

}

void firstMotorTest(){
  
  for(int i=2;i<=5;i++)
  digitalWrite(i, LOW);    // turn the LED off by making the voltage LOW
  
  digitalWrite(2, HIGH);    // turn the LED off by making the voltage LOW
  digitalWrite(4, HIGH);    // turn the LED off by making the voltage LOW
  delay(4000);               // wait for a second

for(int i=2;i<=5;i++)
  digitalWrite(i, LOW);    // turn the LED off by making the voltage LOW
  
  digitalWrite(3, HIGH);    // turn the LED off by making the voltage LOW
  digitalWrite(5, HIGH);    // turn the LED off by making the voltage LOW
  delay(4000);               // wait for a second

}

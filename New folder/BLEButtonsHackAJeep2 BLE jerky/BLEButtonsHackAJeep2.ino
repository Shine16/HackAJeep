#include <TimerOne.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10,11);

//for bluetooth
String inString = "";    // string to hold input
int  LR=0;
int  FB=0;
int del=50;

//ble data for left/right , up/down
//neutral: 200
//forward: 300
//back: 100


int tolerance=20;// tolerance for reading of ble data
int receivedFlag=1;
int mode=0; //0 :buttons , 1: buttons and ble, 2: cutoff


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

  pinMode(12, INPUT); //switch for mode to auto/manual
  digitalWrite(12,HIGH); //pullup resistor
    
  //timer triggers every 300ms
  Timer1.initialize(300000); // set a timer microseconds
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here

}


void timerIsr()
{

     if(digitalRead(12)==HIGH)
       mode=0; //buttons
     else{
       mode=1; //buttons and ble
       if(receivedFlag==0)//check ble flag
          mode=2;//cutoff, shutoff motor due to ble connection loss
       }      
      
  
  receivedFlag=0;//reset received Flag
}


// the loop routine runs over and over again forever:
void loop() {

  ReadBLE();//needed in all 3, to trigger timer in button control and off motors, data only used in buttonBLE

  switch(mode){
  case 0:  //buttons
    buttonControl();
      break;
  case 1:  //buttons and ble
    buttonBLE();
    break;    
  case 2:  
    offMotors();
     break;
  }
    
}

void buttonBLE(){
  if(LR<(200+tolerance)&&LR>(200-tolerance)&&FB<(200+tolerance)&&FB>(200-tolerance)){
     BLEbuttonControl();
  }
  else{//priority to BLE
    offMotors();
    if(LR<(100+tolerance)){//L
        while(LR<(100+tolerance)){//L
          digitalWrite(5,HIGH);delay(del);
          ReadBLE();
    }}  
    if(LR>(300-tolerance)){//R
        while(LR>(300-tolerance)){//R
          digitalWrite(4,HIGH);delay(del);
          ReadBLE();
    }}
    if(FB<(100+tolerance)){//B
        while(FB<(100+tolerance)){//B
          digitalWrite(3,HIGH);delay(del);
          ReadBLE();
    }}
    if(FB>(300-tolerance)){//F
        while(FB>(300-tolerance)){//F
          digitalWrite(2,HIGH);delay(del);
          ReadBLE();
    }}
  }//close else
  
  
}

void offMotors(){
  //reset all low again
  for(int i=2;i<=5;i++)
  digitalWrite(i, LOW);  
}

void ReadBLE(){
  while (mySerial.available() > 0) {
    int inChar = mySerial.read();
    if (isDigit(inChar)) {
      inString += (char)inChar;
      if(inString.length()==6){
     
          LR=int(inString[0]-48)*100+int(inString[1]-48)*10+int(inString[2]-48);
          FB=int(inString[3]-48)*100+int(inString[4]-48)*10+int(inString[5]-48);
          /*Serial.print("Left/Right "); Serial.print(LR); //L is 100, R is 300 
          Serial.print(" Forward/back "); Serial.print(FB); Serial.println();//F is 300, B is 100
          */
          inString = "";// clear the string for new input:
          while( mySerial.available()){ inChar = mySerial.read();}
      }
    }
    
    else{inString ="";
    }
     receivedFlag=1;//turn on flag to tell data received
  }
    
}


void BLEbuttonControl(){ //button program with ble able to override

 if(digitalRead(6)==LOW){//&&LR<(200+tolerance)&&LR>(200-tolerance)&&FB<(200+tolerance)&&FB>(200-tolerance)){
   //while(digitalRead(6)==LOW &&LR<(200+tolerance)&&LR>(200-tolerance)&&FB<(200+tolerance)&&FB>(200-tolerance)){ //while button is held
    digitalWrite(2,HIGH);//Serial.println("U"); //remove serial prints to increase speed
   // delay(del);ReadBLE();
   }//}
 
 else if(digitalRead(7)==LOW){//&&LR<(200+tolerance)&&LR>(200-tolerance)&&FB<(200+tolerance)&&FB>(200-tolerance)){
   //while(digitalRead(7)==LOW &&LR<(200+tolerance)&&LR>(200-tolerance)&&FB<(200+tolerance)&&FB>(200-tolerance)){ //while button is held
    digitalWrite(3,HIGH);//Serial.println("D");
    //delay(del);ReadBLE();
   }//}

 else if(digitalRead(8)==LOW){//&&LR<(200+tolerance)&&LR>(200-tolerance)&&FB<(200+tolerance)&&FB>(200-tolerance)){
   //while(digitalRead(8)==LOW &&LR<(200+tolerance)&&LR>(200-tolerance)&&FB<(200+tolerance)&&FB>(200-tolerance)){ //while button is held
    digitalWrite(4,HIGH);//Serial.println("L");
   // delay(del);ReadBLE();
   }//}

 else if(digitalRead(9)==LOW){//&&LR<(200+tolerance)&&LR>(200-tolerance)&&FB<(200+tolerance)&&FB>(200-tolerance)){
 //  while(digitalRead(9)==LOW &&LR<(200+tolerance)&&LR>(200-tolerance)&&FB<(200+tolerance)&&FB>(200-tolerance)){ //while button is held
    digitalWrite(5,HIGH);//Serial.println("R");
 //   delay(del);ReadBLE();
   }//}
   
 else
   offMotors();
}


void buttonControl(){
  
  if(digitalRead(6)==LOW){
   while(digitalRead(6)==LOW){ //while button is held
    digitalWrite(2,HIGH);//Serial.println("U"); //remove serial prints to increase speed
   }}
 
 else if(digitalRead(7)==LOW){
   while(digitalRead(7)==LOW){
    digitalWrite(3,HIGH);//Serial.println("D");
   }}

 else if(digitalRead(8)==LOW){
   while(digitalRead(8)==LOW){
    digitalWrite(4,HIGH);//Serial.println("L");
   }}

 else if(digitalRead(9)==LOW){
   while(digitalRead(9)==LOW){
    digitalWrite(5,HIGH);//Serial.println("R");
   }}
 else
   offMotors();
}

void firstMotorTest(){
  
  for(int i=2;i<=5;i++)
  digitalWrite(i, LOW);     
  digitalWrite(2, HIGH);     
  digitalWrite(4, HIGH);     
  delay(4000);                

for(int i=2;i<=5;i++)
  digitalWrite(i, LOW);     
  
  digitalWrite(3, HIGH);   
  digitalWrite(5, HIGH);    
  delay(4000);              

}

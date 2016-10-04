



void setup() {                

  for(int i=2;i<=5;i++)
  pinMode(i, OUTPUT);     
  
  Serial.begin(9600);
  
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

  for(int i=2;i<=5;i++)
  digitalWrite(i, LOW);    // turn the LED off by making the voltage LOW
 
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

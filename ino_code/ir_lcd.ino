 /*
* IRremote Library - Copyright 2009 Ken Shirriff
* created by Rui Santos, http://randomnerdtutorials.wordpress.com
* Control LED's with a remote control
* 2013
*/

#include <IRremote.h>
#include <LiquidCrystal.h>
#include <Servo.h>
Servo myServo;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int IR_Recv = 6;   //IR Receiver Pin 6

#define INTRO_MSG "Intro tiempo:"
#define COUNT_MSG "Tiempo restante"
#define FINISH_MSG "Felicidades!!"
const int buttonLight = 7;  //input for turn on display
const long int LEFT  = 16720605;
const long int RIGHT  = 16761405;
const long int UP  = 16736925;
const long int DOWN  = 16754775;
const long int OK  = 16712445;
const long int EMERGENCY  = 16728765   ; //rec button
const int dimmer = 2; //dimmer for backlight in minutes 
int min=0;
int hour=0;
int day=0;
int day_array[3] = {0, 0, 0};
int position=0;
int backlight = 10; //digital output for controlling LCD backlight
int finished=0;

IRrecv irrecv(IR_Recv);
decode_results results;

void setup(){
  myServo.attach(13);
  Serial.begin(9600);  //starts serial communication
  irrecv.enableIRIn(); // Starts the receiver
  pinMode(backlight, OUTPUT);
  pinMode(buttonLight, INPUT);
  digitalWrite(backlight,HIGH);
  lcd.begin(16,2);
  lcd.print(INTRO_MSG);
  lcd.setCursor(0, 1);
  lcd.print("000:00:00");
  lcd.setCursor(position,1);
  lcd.blink();
}

void loop(){
  
  //decodes the infrared input
  while (results.value!=OK) {
    if (irrecv.decode(&results)){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(INTRO_MSG);
      long int decCode = results.value;
      Serial.println(decCode);
     //switch case to use the selected remote control button
     switch (results.value){
        case LEFT: //when you press the Left button
          if ((position==5) || (position==8))   {
            position=position-3; 
          }
          if ((position==2) || (position==1))   {
            position=position-1; 
          }
          break;
          
        case RIGHT: //when you press the Right button
          if ((position==2) || (position==5))  {
            position=position+3; 
          }
          if ((position==0) || (position==1))   {
            position=position+1; 
          }

          break;
          
        case UP: //when you press the Up button
          if (position==0){
              day_array[0]=day_array[0]+1;
              if (day_array[0]>9) {day_array[0]=0;}
          }
          if (position==1){
              day_array[1]=day_array[1]+1;
              if (day_array[1]>9) {day_array[1]=0;}
          }
          if (position==2){
              day_array[2]=day_array[2]+1;
              if (day_array[2]>9) {day_array[2]=0;}
          }
          if (position==5) {
              hour=hour+1;
              if (hour>24) {hour=0;}
          }
          if (position==8) {
            min=min+1;
              if (min>59) {min=0;}
          }        
          break;   
          
        case DOWN: //when you press the Down button
          if (position==0){
              day_array[0]=day_array[0]-1;
              if (day_array[0]<0) {day_array[0]=9;}
          }
          if (position==1){
              day_array[1]=day_array[1]-1;
              if (day_array[1]<0) {day_array[1]=9;}
          }
          if (position==2){
              day_array[2]=day_array[2]-1;
              if (day_array[2]<0) {day_array[2]=9;}
          }
          if (position==5) {
              hour=hour-1;
              if (hour<0) {hour=23;}
          }
          if (position==8) {
              min=min-1;
              if (min<0) {min=59;}
          }        
          break; 
      }
      String day_string = String(day_array[0])+String(day_array[1])+String(day_array[2]);
      day = day_string.toInt();
      printData(day,hour,min);      
      lcd.setCursor(position, 1);
      lcd.blink();
      delay(10);
      irrecv.resume(); // Receives the next value from the button you press
    }  
  }
  if ((results.value==OK)&&(finished==0)) {lockdoor();countdownFunction(day,hour,min);finished=1;}
}

void lockdoor(){
  int lock=5; //seconds to close the door
  while (lock>0)
  {
    lcd.setCursor(0,0);  
    lcd.clear();
    String lock_seg = String(lock);
    lcd.print("Closing in "+lock_seg+" sec");
    delay(1000);
    lock=lock-1;
  }
  myServo.write(0);
}
void countdownFunction(int day,int hour,int min){
  int dimmer_countdown = 0;
  int display_on = 0;
  int buttonState = 0;
  int sec=59;
  lcd.noBlink();
  lcd.setCursor(0,0);  
  lcd.clear();
  lcd.print(COUNT_MSG);
   while(day >= 0)
   {
     while (hour >= 0)
     {
        while (min >= 0)
        {
          printData(day,hour,min);
          if (dimmer_countdown==dimmer) { digitalWrite(backlight,LOW);}
          if ((day==0)&&(hour==0)&&(min==dimmer)) { digitalWrite(backlight,HIGH);}
          if ((day==0)&&(hour==0)&&(min==0)) { goto stopped; }
          while (sec >= 0)
          {
            lcd.setCursor(9,1);  
            String time_sec ="";
            if (sec<=9) {time_sec = ":0"+String(sec);} else {time_sec=":"+String(sec);}            
            lcd.print(time_sec);  
            buttonState = digitalRead(buttonLight);
            if ((buttonState==HIGH)&&(display_on<30)) {digitalWrite(backlight,HIGH);display_on=1;}
            if (display_on>0) {display_on++;}
            if ((display_on==30)&&(min>dimmer)) {digitalWrite(backlight,LOW);display_on=0;}
//            if (irrecv.decode(&results)) {
//              long int decCode = results.value;
//              switch (results.value){
//                case EMERGENCY:
//                  digitalWrite(backlight,HIGH);
//                  goto stopped;
//                  break;
//              }
//              irrecv.resume(); // Receive the next value
//            }
            sec--;
            delay(1000);  
          }
          sec=59;
          min--;
          dimmer_countdown++;
        }
        min=59;
        hour--;
     }
     hour=23;
     day--;
   }
   stopped:
   finish();
}

void finish(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(FINISH_MSG);
  lcd.setCursor(0,1);
  lcd.print("000:00:00");
  myServo.write(90);
  delay(60000);
}

void printData(int day,int hour,int min)
{
  String dia = String(day);
  if (day<=99) {dia = "0"+dia;}
  if (day<=9) {dia = "0"+dia;}
  String hora = String(hour);
  if (hour<=9) {hora = "0"+hora;}
  String minuto = String(min);
  if (min<=9) {minuto = "0"+minuto;}
  String time=dia+":"+hora+":"+minuto;
  lcd.setCursor(0, 1);
  lcd.print(time);
}



#include <LiquidCrystal.h>
#include <Servo.h>
Servo myServo;


const int left_pin = 6;
const int right_pin = 7;
const int up_pin = 8;
const int down_pin = 9;
const int enter_pin = 10;
const int position1 = 0; //servo position 1
const int position2 = 180; //servo position 2
//const int position1 = 15; //landing gear position 1
//const int position2 = 90; //landing gear position 2

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


#define INTRO_MSG "Intro tiempo:"
#define COUNT_MSG "Tiempo restante"
#define FINISH_MSG "Felicidades!!"
int bucle=0;
int min=0;
int hour=0;
int day=0;
int day_array[3] = {0, 0, 0};
int position=0;

void setup(){
  myServo.attach(13);
  myServo.write(position1);
  Serial.begin(9600);  //starts serial communication
  pinMode(left_pin, INPUT);
  pinMode(right_pin, INPUT);
  pinMode(up_pin, INPUT);
  pinMode(down_pin, INPUT);
//  pinMode(enter_pin, INPUT);
//  pinMode(reset_pin, INPUT);


  lcd.begin(16,2);
  lcd.print(INTRO_MSG);
  lcd.setCursor(0, 1);
  lcd.print("000:00:00");
  lcd.setCursor(position,1);
  lcd.blink();
}

void loop(){
if (bucle==0) {
  int enter=0;
  int up=0;
  int down=0;
  int left=0;
  int right=0;
 
  while (enter==0) {
    enter = digitalRead(enter_pin);
    up = digitalRead(up_pin);
    down = digitalRead(down_pin);
    left = digitalRead(left_pin);
    right = digitalRead(right_pin);
    if (left==HIGH) {  
          if ((position==5) || (position==8)) {
            position=position-3; 
          }
          if ((position==2) || (position==1)) {
            position=position-1; 
          }
    }
    if (right==HIGH) {
          if ((position==2) || (position==5)) {
            position=position+3; 
          }
          if ((position==0) || (position==1)) {
            position=position+1; 
          }
    }
    if (up==HIGH) {
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
    }
    if (down==HIGH) {
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
    }
      String day_string = String(day_array[0])+String(day_array[1])+String(day_array[2]);
      day = day_string.toInt();
      printData(day,hour,min);      
      lcd.setCursor(position, 1);
      lcd.blink();
      delay(200);
     
  }
  if (enter==HIGH) {
    lockdoor();countdownFunction(day,hour,min);
  }
}
else {}
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
  myServo.write(position2);
}
void countdownFunction(int day,int hour,int min){
  int display_on = 0;
  int sec=0;
  int reset=0;
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
          while (sec >= 0)
          {
            lcd.setCursor(9,1);  
            String time_sec ="";
            if (sec<=9) {time_sec = ":0"+String(sec);} else {time_sec=":"+String(sec);}            
            lcd.print(time_sec);  
            sec--;
            delay(1000);  
          }
          sec=59;
          min--;
        }
        min=59;
        hour--;
     }
     hour=23;
     day--;
   }
   finish();
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

void finish(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(FINISH_MSG);
  lcd.setCursor(0,1);
  lcd.print("000:00:00:00");
  myServo.write(position1);
  bucle=1;
}

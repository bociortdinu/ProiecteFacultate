/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
*/

#include <string.h>
#include <stdbool.h>
#include "Wire.h"
#include "LiquidCrystal.h"

LiquidCrystal lcd(0);

//Iesiri
int Ignition = 11;
int LOCK_driver_door_key = 7;
int DOOR = 8;
int Clacson = 12;
//Intrari
int FLASHER = 9;
int HORN = 10;

unsigned long t_init,t_fin;

void send_data(){
  Serial.println("----------");
  Serial.print("Flasher: ");
  Serial.println(digitalRead(FLASHER));
  Serial.print("Horn: ");
  Serial.println(digitalRead(HORN));
  Serial.println("----------\n");
}

void default_conditions(){
  digitalWrite(Ignition,HIGH);
  digitalWrite(DOOR,HIGH);
  digitalWrite(Clacson,HIGH);
}
void preconditions_1() {

  digitalWrite(Ignition,HIGH);
}
void preconditions_2() {

  digitalWrite(Ignition,LOW);
}
void preconditions_3() {

  digitalWrite(Ignition,HIGH);
  digitalWrite(LOCK_driver_door_key,HIGH);
}
void preconditions_4() {
  digitalWrite(Ignition,LOW);
  digitalWrite(LOCK_driver_door_key,HIGH);
}
void setup() {
  Serial.begin(9600);
  lcd.begin(16,2); //lcd.print for printing | lcd.setCursor(X,Y) | lcd.clear();
  lcd.setBacklight(1);

    pinMode(Ignition,OUTPUT);
    pinMode(LOCK_driver_door_key,OUTPUT);
    pinMode(DOOR,OUTPUT);
    pinMode(Clacson,OUTPUT);
    
    pinMode(FLASHER,INPUT);
    pinMode(HORN,INPUT);
}
/*
 * if(Serial.available()>0){
    s = Serial.read();
 */
bool flasher_test(int del){
  bool sw = false;
  t_init = t_fin = millis();
  while(t_fin - t_init < del && digitalRead(FLASHER) == HIGH){
        t_fin = millis();
        if(digitalRead(FLASHER) == LOW){
          sw = true;
          break;
        }
     }
   if(sw == false)
      return sw;
   else{
      t_init = t_fin = millis();
      sw = false;
      while(t_fin - t_init < del && digitalRead(FLASHER) == LOW){
        t_fin = millis();
        if(digitalRead(FLASHER) == HIGH){
          sw = true;
          break;
        }
       }
       return sw;
   }
}
int testing(char optiune){
  if(optiune == '1'){
    preconditions_1();
    digitalWrite(LOCK_driver_door_key, LOW); //procedure 1
    //Expected
    delay(200);
    if(flasher_test(1500) == false){
      return 1;
    }
    else{
      delay(100);
      default_conditions();
      digitalWrite(LOCK_driver_door_key, HIGH); //procedure 2
      if(flasher_test(1500) == false)
        return 2;
      else{
        delay(100);
        if(flasher_test(1500) == false)
          return 3;
        else
          return 0;
      }
    }
   }
   
   if(optiune == '2'){
      preconditions_2();
      digitalWrite(LOCK_driver_door_key, LOW);
      if(flasher_test(1500) == true)
        return 1;
      else{
        digitalWrite(LOCK_driver_door_key, HIGH);
        if(flasher_test(1500) == true)
          return 2;
        else
          return 0;
      }
   }
   
   if(optiune == '3'){
       preconditions_3();
       digitalWrite(LOCK_driver_door_key, LOW);
       delay(29000);
       digitalWrite(DOOR,LOW);
       delay(1);
       digitalWrite(DOOR,HIGH);
       if(flasher_test(1500) == true)
          return 1;
       else
          if(digitalRead(HORN) == LOW)
            return 2;
          else{
            delay(1000);
            digitalWrite(DOOR,LOW);
            delay(1);
            digitalWrite(DOOR,HIGH);
            if(flasher_test(1500) == false || digitalRead(HORN) == HIGH)
              return 3;
            else
              return 0;
          }
   }
   
   if(optiune == '4'){
       preconditions_4();
       digitalWrite(LOCK_driver_door_key, LOW);
       delay(29000);
       digitalWrite(DOOR,LOW);
       delay(1);
       digitalWrite(DOOR,HIGH);
       if( digitalRead(HORN) == LOW)
          return 1;
       else{
          delay(1000);
            digitalWrite(DOOR,LOW);
            delay(1);
            digitalWrite(DOOR,HIGH);
            if(digitalRead(HORN) == LOW)
                return 2;
            else
                return 0;
       }
   }
   
   if(optiune == '5'){
      digitalWrite(Ignition,LOW);
      digitalWrite(Clacson,LOW);
      delay(10000);
      if(digitalRead(HORN)==LOW)
        Serial.println("Hornul merge");
        else
          Serial.println("Hornul NU merge");
//      else{
//        digitalWrite(Clacson,HIGH);
//        digitalWrite(Ignition,LOW);
//        delay(2000);
//        digitalWrite(Clacson,LOW);
//        if(digitalRead(HORN)==LOW)
//          return 0;
//        else
//          return 2;
//         
//      }
   }
}
char optiune;
void loop() {
  do{
    Serial.println("1. Lock/Unlock - Ignition off");
    Serial.println("2. Lock/Unlock - Ignition on");
    Serial.println("3. Arming alarm - Ignition off");
    Serial.println("4. Arming alarm - Ignition on");
    Serial.println("5. Pressing the steering wheel switch - Ignition off/on");
    Serial.println("0. EXIT");
    

    while(Serial.available()<=0);
    optiune = Serial.read();
    while(Serial.available()>0)
      Serial.read();
    int debug;
    if(optiune < '0' && optiune > '4')
      Serial.println("Optiune eronata!");
    else{
        default_conditions();
        int debug = testing(optiune);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Test ");
        lcd.setCursor(6,0);
        lcd.print(optiune);
        lcd.setCursor(0,1);
        switch(debug){
          case 0:
                lcd.print("PASSED!"); break;
          default:
                lcd.print("FAILLED - Test ");
                lcd.setCursor(15,0);
                lcd.print(debug + '0');
                send_data();
                break;
        }
    }
  }while(optiune!='0');
}

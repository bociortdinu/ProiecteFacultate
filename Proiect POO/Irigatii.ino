#include <LiquidCrystal.h>

class Pin
{
protected:
int pin;
public:
  int Pin_Query()
{
  return pin;
}
};

class LED:public Pin
{
private:

public:
  LED(int x)
  {
    pin=x;
  }

};

class Senzor:public Pin
{
private:
int value;
public:
Senzor(int x)
{
  pin=x;
}
int State()
{
  value=analogRead(pin);
  return value;
}
};

class Buton:public Pin
{
private:
  int value;
public:
Buton(int x)
{
  pin=x;
}
int State()
{
  value=digitalRead(pin);
  return value;
}
};


LED galben(8); //initializam pin prin clasa LED
LED rosu(10);  //initializam pin prin clasa LED

Senzor soil_moisture(A0); //initializam pin prin clasa Senzor

Buton b1(12);  //initializam pin prin clasa Button

int Q[6]={7,6,5,4,3,2}; // RS E DB4 DB5 DB6 DB7

LiquidCrystal lcd(Q[0],Q[1],Q[2],Q[3],Q[4],Q[5]);  //declaram pinii pentru LCD

void setup() {
 
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(b1.Pin_Query(), INPUT);
  pinMode(galben.Pin_Query(), OUTPUT);
  pinMode(rosu.Pin_Query(), OUTPUT);

 lcd.print("Valoare: ");
}

int umiditate,prag=350;

void loop() {

 
 if(b1.State()==HIGH) //daca butonul este apasat atunci porneste sistemul
 {
  umiditate=soil_moisture.State();
     lcd.setCursor(9, 0); // se seteaza primul rand, col 9
     lcd.print(umiditate);
     
    if(umiditate>950) // daca este pamant uscat
    {
      lcd.setCursor(13, 0); 
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print("Pamant uscat!");
      digitalWrite(rosu.Pin_Query(), LOW);
      digitalWrite(galben.Pin_Query(), LOW);

       
    }
    else
    if(umiditate<950 && umiditate>=350) // pamant umed
    {
 //      lcd.clear();
       digitalWrite(rosu.Pin_Query(), LOW);
       digitalWrite(galben.Pin_Query(), HIGH);
       lcd.setCursor(12, 0); 
       lcd.print(" ");
       lcd.setCursor(0, 1);
       lcd.print("Irigam putin!");
    }
    else
    if(umiditate<prag) // daca nu este nevoie de irigatie, prag
    {

       digitalWrite(rosu.Pin_Query(), HIGH);
       digitalWrite(galben.Pin_Query(), LOW);
       lcd.setCursor(12, 0); 
       lcd.print(" ");
       lcd.setCursor(0, 1);
       lcd.print("Nu se iriga!");
    }

 }


}

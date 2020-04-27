int Factor_umplere=0,contor=0; // incrementare +25,5~26

 void setare_PWM0(int Factor_umplere)
  {
  OCR0A=Factor_umplere; //in reg OCR0A se seteaza val fact de umplere
  }


uint16_t read_adc(uint8_t channel)
{ 
  ADMUX &= 0xF0;//set input AO to A5
  ADMUX |= channel;//select chanel AO to A5
  ADCSRA |= (1<<ADSC);//start conversion
  while(ADCSRA & (1<<ADSC));//wait wile adc conversion are not updated 
  return ADC;//read and return voltage
}

float read_temp()
{
  float voltage=0;
  float temperatureCelsius=0; 
  int reading;
  reading=read_adc(0); 
  voltage=reading*5.0; 
  voltage/=1024.0;
  temperatureCelsius=(voltage*100);
  return temperatureCelsius;
}

void setup() {
  cli(); //clear interact
  
  TCCR0B=0;
  TCCR0A=0;
  
  TCCR0A|=(1<<WGM01)|(1<<WGM00)|(1<<COM0A1); //TIMER 0 ( NORMAL ) si fast pwm non-inverted
  TCCR0B|=(1<<CS02)|(1<<CS00); // PRESCALER 1024

  TCCR1A=0;
  TCCR1B=0;  
  TCCR1B|=(1<<WGM12)|(1<<CS12); // TIMER 1  ( CTC )
                                           // PRESCALER 256

  TIMSK1|=(1<<OCIE1A); //ACTIVAREA INTRERUPERII
  OCR1A=6245;

  DDRD|=0xFF;

  ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));//set division factor between 
  ADMUX |= (1<<REFS0);//AVcc with external capacitor at Aref pin 
  ADCSRA |= (1<<ADEN);//enable ADC
  ADCSRA |= (1<<ADSC);//ADC


  UBRR0H = 0x00;         
  UBRR0L = 0x67; //pt viteza de transimisie 9600 

  UCSR0B |= (1 << TXEN0) | (1 << RXEN0); //setam bitii pt validarea transmisiei si receptiei
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //setam bitii pt dimensiunea datelor din frame

  sei();
}

  ISR(TIMER1_COMPA_vect) 
  {  
    //pct 1
    contor++;//0.1sec in 0.1sec
    if(contor%10==0)
    PORTD^=1<<7; //toggle led pin 7 odata la 10* 0.1 secunde

    //pct2
  if (contor<10)
    Factor_umplere=Factor_umplere+25; // faza 1: contorul crescand cu 1 la fiecare 0.1 sec, intr-o secunda factorul de umplere creste treptat 
  else if(contor>=10 && contor<20)
    Factor_umplere=255; // faza 2: timp de o secunda sta aprins (factor de umplere maxim)
  else if(contor>=20 && contor<30)
    Factor_umplere=Factor_umplere-25; 
  else if(contor>=30 && contor<40) //faza 3: timp de o secunda scade treptat cu 25 factorul de umplere
    Factor_umplere=0;
  else if(contor==40)  //faza 4: timp de o secunda ledul este stins
    contor=0; //se reseteaza contorul si se reiau fazele 
  }

float temp=0;

void loop() {
  
  setare_PWM0(Factor_umplere);

   temp=read_temp();
  if(temp>35)
  {
    PORTD|=1<<4;
  }
  else if(temp<34) //avem histerezis de 1 grad 
  {
    PORTD&=~1<<4;
  }



 
 while( !( UCSR0A & (1<<UDRE0)) ) ;  
    byte x = UDR0; //preia datele
    if (x == 65) //A
    {
      PORTD |= 1 << 5;
    }
    if (x == 83) //S
    {
      PORTD = ~1 << 5;
    }
    
    Serial.println(temp);

} 

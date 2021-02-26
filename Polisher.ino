#include <Arduino.h>

#define POTIPIN A0
#define EN PD2
#define DIR PD5
#define CLK PD4
#define LEVEL PD3
#define MICROSTEPPING 0.25
#define INTERVALL 300*1000000       // in microsec.
#define INTERVALL_PAUSE 60*1000     // in millisec.
#define VAL_OFF 100000
#define BAUDRATE 115200

int  potiMap  = 0;
bool run  = false;
unsigned long intervall_timer = micros();
unsigned long micro = micros();
unsigned long watchOverflow = micros();

void setup() {
  pinMode(POTIPIN,  INPUT);
  pinMode(     EN, OUTPUT);
  pinMode(    DIR, OUTPUT);
  pinMode(    CLK, OUTPUT);
  pinMode(  LEVEL, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(   EN, LOW);
  digitalWrite(  DIR, LOW);
  digitalWrite(  CLK, LOW);
  digitalWrite(LEVEL, LOW);

  Serial.begin(BAUDRATE);
  Serial.println("Hello, im redy for rumble");
  Serial.print("potiMap = "); Serial.print(potiMap); Serial.print("; run = "); Serial.println(run);
  Serial.print("Baudrate = "); Serial.println(BAUDRATE);
  Serial.println("/////////////////////////////////////////////////////////////////////////");

  while (analogRead(POTIPIN) > 10)   // value between 0 - 1025 --> Potivalue must be lower then 10 for Start the control
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
// Stat control, set standard values.  
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(         EN, LOW);
  digitalWrite(        DIR, HIGH);
  digitalWrite(      LEVEL, HIGH);
  delay(500);
  watchOverflow = watchOverflow + 400000;
}

void loop() {
  potiMap = map(analogRead(POTIPIN),0,1023,(15000),(8000));
  // check if poti persistence in higher position
  if( watchOverflow < micros()){
    if (analogRead(POTIPIN) > 10){
      digitalWrite(EN, HIGH);
      // check if polisher is running
      if ( !run )
      {
        for (size_t i = 80; i > 10; i--)
        {
          if (analogRead(POTIPIN) > 10)
          {
            oneStep(CLK);
            delayMicroseconds(400*i*MICROSTEPPING);
            run = true;      
          }else{
            motorStop();
            break;
          }
        }
      }else if (micro + ( potiMap*MICROSTEPPING ) < micros() && run){
        oneStep(CLK);
        micro = micros(); 
      }   
    }else
    {
      motorStop();
    }
  }else
  {
    motorStop();
    watchOverflow = micros()+400000;
    delay(500);
    micro = micros();
  }
}

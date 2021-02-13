#include <Arduino.h>

#define POTIPIN A0
#define EN PD2
#define DIR PD5
#define CLK PD4
#define LEVEL PD3
#define MICROSTEPPING 0.25
#define INTERVALL 300*1000000       // in microsec.
#define INTERVALL_PAUSE 60*1000    // in millisec.
#define VAL_OFF 100000
#define BAUDRATE 115200

int  val  = 0;
bool run  = false;
unsigned long intervall_timer = micros();
unsigned long micro = micros();

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
  Serial.print("val = "); Serial.print(val); Serial.print("; run = "); Serial.println(run);
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
}

void loop() {
val = map(analogRead(POTIPIN),0,1023,(8888),(740));
/*
if (intervall_timer+INTERVALL < micros()){
  intervall_timer = micros();
  run == false;
  digitalWrite(CLK, LOW);
  digitalWrite(EN, LOW);
  //Serial.println("Intervall: Start");
  delay(INTERVALL_PAUSE);
  //Serial.println("Intervall stop");
  }
*/
  
//Serial.println(val);
// check if poti persistence in higher position
  if (analogRead(POTIPIN) > 10)
  {
    digitalWrite(EN, HIGH);
// check if polisher is running
    if (micro+(val*MICROSTEPPING) < micros() && run)
    {
      //Serial.print(micros() - micro); Serial.print(";  "); Serial.println(val);
      digitalWrite(CLK, HIGH);
      delayMicroseconds(4);
      digitalWrite(CLK, LOW);
      delayMicroseconds(4);

      micro = micros();

    }else if (run == false)
    {
      //Serial.println("Motor start");
      // if polisher starts slowly if it is nocht running
      for (size_t i = 100; i > 0; i--)
      {
        //Serial.print("Motor Start i = "); Serial.println(i);
        digitalWrite(CLK, HIGH);
        delayMicroseconds(4);
        digitalWrite(CLK, LOW);
        delayMicroseconds(50*i);
        delay(50);
        //Serial.println(i);
        
      }
        run = true;      
    }   
  }
// if poti persistence in lower poksition
  else  
  {
    //Serial.println("Motor aus");
    digitalWrite(CLK, LOW);
    digitalWrite(EN, LOW);
    micro = micros();
    run = false;
  }
}

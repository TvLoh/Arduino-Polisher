#include <Arduino.h>

#define POTIPIN A0
#define EN A4
#define DIR A1
#define CLK A2
#define LEVEL A3
#define MICROSTEKPPING 0.25
#define VAL_OFF 100000
#define BAUDRATE 115200

int  val  = 0;
bool run  = false;
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
//Serial.println(val);
// check if poti persistence in higher position
  if (analogRead(POTIPIN) > 10)
  {
    digitalWrite(EN, HIGH);
// check if polisher is running
    if (micro+(val*MICROSTEKPPING) < micros() && run)
    {
      Serial.print(micros() - micro); Serial.print(";  "); Serial.println(val);
      digitalWrite(CLK, HIGH);
      delayMicroseconds(4);
      digitalWrite(CLK, LOW);
      delayMicroseconds(4);

      micro = micros();

    }else if (run == false)
    {
      //Serial.println("Motor start");
// if polisher starts slowly if it is nocht running
      for (size_t i = 10; i >= 1; i--)
      {
        //Serial.print("Motor Start i = "); Serial.println(i);
        digitalWrite(CLK, HIGH);
        delayMicroseconds(4);
        digitalWrite(CLK, LOW);
        delayMicroseconds(8888000*i);
        run = true;
      }
      
    }   
  }
// if poti persistence in lower poksition
  else  
  {
    Serial.println("Motor aus");
    digitalWrite(CLK, LOW);
    digitalWrite(EN, LOW);
    micro = micros();
    run = false;
  }
}

// Stepper Driver detect the 4 Microsecond Puls as one step. As you kan see in the 
void oneStep(int pin){
      digitalWrite(pin, HIGH);
      delayMicroseconds(6);
      digitalWrite(pin, LOW);
      delayMicroseconds(4);
  }

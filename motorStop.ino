void motorStop(){
    digitalWrite(CLK, LOW);
    digitalWrite(EN, LOW);
    micro = micros();
    run = false;
  }

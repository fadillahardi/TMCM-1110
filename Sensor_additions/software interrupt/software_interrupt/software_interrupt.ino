ISR (PCINT1_vect)
 {
 // handle pin change interrupt for A0 to A5 here
   float val = analogRead(A2);
    

 }  // end of PCINT1_vect

void setup ()
  { 
  // pin change interrupt (example for A2)
    PCMSK1 |= bit (PCINT10);  // want pin A2
    PCIFR  |= bit (PCIF1);
    PCICR  |= bit (PCIE1);

  //enabling pin
    pinMode(A2, INPUT);
    //digitalWrite(A2,HIGH);
    Serial.begin(38400);
  }

  void loop()
  {
    //float volt = val*5/1024;
    //Serial.println(volt);
  }

ISR (PCINT1_vect)
 {
 // handle pin change interrupt for A0 to A5 here
 	digitalWrite(13, digitalRead(A2));

 }  // end of PCINT1_vect

void setup ()
  { 
  // pin change interrupt (example for A2)
  	PCMSK1 |= bit (PCINT10);  // want pin A2
  	PCIFR  |= bit (PCIF1);   // clear any outstanding interrupts
  	PCICR  |= bit (PCIE1);   // enable pin change interrupts for D8 to D1

  //enabling pin
  	pinMode(A2, INPUT);
  }

  void loop()
  {

  }
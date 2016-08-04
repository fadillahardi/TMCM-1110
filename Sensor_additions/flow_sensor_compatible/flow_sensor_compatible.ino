int interval = 1000;
int sensorPin = A2;
int sign, change;
float volt, rate;
unsigned long previousMillis = 0;
unsigned long pulse =0;

void setup() {
  Serial.begin(38400);
  pinMode(sensorPin,INPUT);
  
}

void readSign() {
  // put your main code here, to run repeatedly:
  //millis();
  float val;
  val = analogRead(sensorPin);
  volt = (val/1024)*5;
  if(volt>0 && volt< 2.5)
  {
    sign = 0;
  }
  else if(volt>2.5 && volt < 5)
  {
    sign = 1;
  }
 
  if(sign != change)
  {
    pulse++;
    change=sign; 
  }
}

void loop()
{
  unsigned long currentMillis = millis();
  readSign();
 
  if((currentMillis - previousMillis)>=interval)//show every one second
  {
    float s = pulse;
    rate = (s*60)/36000;
    Serial.print(pulse);
    //Serial.print("  ");
    Serial.println("  waves");
    Serial.print(rate);
    Serial.println("  L/min");
    Serial.println("one second elapsed");
    pulse = 0;
    previousMillis = currentMillis;
  }
}


byte exti = 0;
byte sensorPin = 2;
int K_factor =3600;

volatile byte pulseCount;
float flowRate;
unsigned long oldTime;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin,INPUT);
  digitalWrite(sensorPin,HIGH);
 
  pulseCount        = 0;
  flowRate          = 0.0;
  oldTime           = 0;
  
  attachInterrupt(exti,pulseCounter,FALLING);

}

void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}

void loop() {
  // put your main code here, to run repeatedly:
  if((millis() - oldTime) > 1000)
  {
    detachInterrupt(exti);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / K_factor;
    oldTime = millis();
    Serial.print("Flow rate: ");
    Serial.println(int(flowRate));
    pulseCount = 0;
    attachInterrupt(exti,pulseCounter,FALLING);
    
  }

}

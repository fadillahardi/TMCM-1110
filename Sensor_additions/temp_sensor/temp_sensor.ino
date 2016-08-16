//#include <math.h>
int sensor1 = A3;
int sensor2 = A2;
int motor = A1;
int cobapin=13;
//const float R_pullup = 10000;
//const float res_def = 10000;
//const float temp_def = 25;
//const float B_coef = 3950;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(motor,OUTPUT);
  pinMode(cobapin,OUTPUT);

}

/*void steinhart()
{
  float value = analogRead(sensor1);
  Serial.print("sensor read = ");
  Serial.println(value);
  
  float convert = (1023/value)-1;
  float R_read = 10000/(convert);
  Serial.print("resistance = ");
  Serial.println(R_read);
  
  float a = 1.1279;
  float b = 0.23429;
  float c = 0.000087298;
  float x = log(R_read);

  float temp = 1000/(a + (b*x) + (c*x*x*x));
  float inCelcius = 273.15-temp;

  Serial.println(temp);
  Serial.print("in celcius = ");
  Serial.println(inCelcius);
  Serial.println(" ");
  
  delay(800);
}*/

void TempSensor()
{
  float value = analogRead(sensor1);
  Serial.print("sensor read = ");
  Serial.println(value);
  
  float convert = (1023/value)-1;
  float R_read = 1000/(convert);
  Serial.print("resistance = ");
  Serial.println(R_read);
  
  float Bxlog = log(R_read/10000);
  //Serial.print("hasil log = ");
  //Serial.println(Bxlog);
  
  float steinhart = 298.15*4268.2146/(4268.2146+(298.15*Bxlog));
  //float steinhart = 298.15*3950/(3950+(298.15*Bxlog));
  //Serial.print("steinhart T = ");
  //Serial.println(steinhart);
  
  float inCelcius = steinhart-273.15;
  Serial.print("in celcius = ");
  Serial.println(inCelcius);
  Serial.println(" ");
  delay(600);
}

void FlowSensor()
{
  float flowRAW = analogRead(sensor2);
  //Serial.println(flowRAW);
  float conv = (flowRAW/1023)*5;
  int pulse;
  int sign = 0;
  int newpls;
  //Serial.println(conv);
  if(flowRAW<500)
  {
    pulse = 0;
  }
  else pulse = 1;
  newpls = pulse;
  if(newpls =! pulse)
  {
    sign = sign + 1;
  }
  Serial.print(pulse);
  Serial.print(sign);
  delay(100);
  
}

void motorFull(){
  analogWrite(motor,255);
  delay(100);
  analogWrite(motor,0);
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  TempSensor();
  //motorFull();
  //delay(100);
  analogWrite(motor,100);
  delay(2000);
  analogWrite(motor,150);
  delay(2000);
  analogWrite(motor,200);
  delay(2000);
  analogWrite(motor,150);
  delay(2000);
  analogWrite(motor,100);
  delay(2000);
  
  
}

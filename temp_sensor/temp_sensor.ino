//#include <math.h>
int sensor1 = A3;
const float R_pullup = 10000;
const float res_def = 10000;
const float temp_def = 25;
const float B_coef = 3950;
//float value;
//float R_read;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensor1, INPUT);

}

/*void ReadSensor()
{
  float value = analogRead(sensor1);
  Serial.println(value);
  float convert = (1023/value)-1;
  float R_read = (R_pullup)/(convert);
  Serial.println(R_read);
  float steinhart;
  float Bxlog = (1 / 3950);//*(log(R_read/10000));
  steinhart = (1/(25.0+273.15)) + Bxlog;
  Serial.print("steinhart 1/T = ");
  Serial.println(steinhart);
  Serial.print("Bxlog = ");
  Serial.println(Bxlog);
  float hart = 1.0/steinhart;
  float inCelcius = 273.15-hart;
  Serial.println(inCelcius);
  
  delay(800);
}*/

void loop() {
  // put your main code here, to run repeatedly:
  //ReadSensor();
  float value = analogRead(sensor1);
  Serial.print("sensor read = ");
  Serial.println(value);
  
  float convert = (1023/value)-1;
  float R_read = 10000/(convert);
  Serial.print("resistance = ");
  Serial.println(R_read);
  
  float Bxlog = log(R_read/10000);
  Serial.print("hasil log = ");
  Serial.println(Bxlog);
  
  float steinhart = 298.15*3950/(3950+(298.15*Bxlog));
  Serial.print("steinhart T = ");
  Serial.println(steinhart);
  
  float inCelcius = steinhart-237.15;
  Serial.print("in celcius = ");
  Serial.println(inCelcius);
  Serial.println(" ");
  
  delay(800);
}

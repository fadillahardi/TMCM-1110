/**********************************      ABSOLUTELY TESTED AND WORKING     ***********************************/


byte curStep = 0;
volatile long long distance = 0;
byte coils[] = {0, 1, 3 ,4};

/*  ATTINY85 PINOUT  */
#define stepPin 0       //pin7 - PORTB 2 (interrupt 0)              --Used as Interrupt (so 0)
#define dirPin 0        //pin1 - PORTB 5 (analog 0) - Reset pin    --

#define del 140         //delay between 2 steps.  Maybe 300 is a good value... Test it with your motor plugged in.
                        // depends on Amperage too. If you feed motor max amps go for less delay
                        //
/*
 *    The threshold that identifies if a signal is high or low. It is inside [0, 1023].
 *      More likely to be higher than the middle-interval value.
 *
 *    For Attiny 85 this has to be the reset pin connected to a Transistor and resistors. It has to read the "low"
 *      but not Reset your microcontroller...
 */
#define highThreshold  829

/*
 *    Stepper motor coil states.
 *      States with more than one HIGHs are halfsteps.
 *
 *    Dead state is the state that no current is sent to the motor.
 */
int steps[8][4] ={
{LOW, LOW, LOW, HIGH},
{LOW, LOW, HIGH, HIGH},
{LOW, LOW, HIGH, LOW},
{LOW, HIGH, HIGH, LOW},
{LOW, HIGH, LOW, LOW},
{HIGH, HIGH, LOW, LOW},
{HIGH, LOW, LOW, LOW},
{HIGH, LOW, LOW, HIGH},
};
int dead[4] = {LOW, LOW, LOW, LOW};

void setup() {

    pinMode(coils[0], OUTPUT);
    pinMode(coils[1], OUTPUT);
    pinMode(coils[2], OUTPUT);
    pinMode(coils[3], OUTPUT);

/*  Triggers the count() function whenever a step signal is received  */
  attachInterrupt(0, count, RISING);
}

void loop() {

  if (distance > 0){
    curStep = (++curStep) % 8;
    drive(steps[curStep]);
    --distance;
  }
  if (distance < 0){
    curStep = (--curStep) % 8;
    drive(steps[curStep]);
    ++distance;
  }
  
/*
   //  makes the driver stop sending current to the motor without reason.  Absolutely prevents overheating when not moving
  if (!distance)
      drive(dead);
//*/
}

void count(){

  ///*
  if (analogRead(dirPin) > highThreshold)
    distance--;
  else
 //*/
    distance++;
  }

/*  drives the motor coils with the correct combination for every step
 *
 *    needs to run fast so it is inline
 */
inline void drive(int state[4]){

    digitalWrite(coils[0], state[0]);
    delayMicroseconds(del);
    digitalWrite(coils[1], state[1]);
    delayMicroseconds(del);
    digitalWrite(coils[2], state[2]);
    delayMicroseconds(del);
    digitalWrite(coils[3], state[3]);
    delayMicroseconds(del);
//    delayMicroseconds(del*4); 
}

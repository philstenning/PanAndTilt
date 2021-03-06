#include "Arduino.h"
#include <Servo.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
const int PAN_PIN = 3;
const int TILT_PIN = 2;
const int MIN_SERVO_VALUE = 5;
const int MAX_SERVO_VALUE = 179;
Servo panServo;
Servo tiltServo;

// void pan(Servo servo, int speed)
// {
//   int pos;
//
//   for (pos = 5; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees
//   {                                   // in steps of 1 degree
//     servo.write(pos);                 // tell servo to go to position in variable 'pos'
//     delay(speed);                     // waits 15ms for the servo to reach the position
//     Serial.println(pos);
//   }
//   for (pos = 180; pos >= MIN_SERVO_VALUE; pos -= 1) // goes from 180 degrees to 0 degrees
//   {
//     servo.write(pos); // tell servo to go to position in variable 'pos'
//     delay(speed);
//     Serial.println(pos); // waits 15ms for the servo to reach the position
//   }
// }

void increment(Servo servo)
{
  int pos = servo.read() + 1;
  // Serial.println("increment()");
  servo.write(pos);
}

void decrement(Servo servo)
{
  int pos = servo.read();
  pos -= 1;
  if (pos >= MIN_SERVO_VALUE)
  {
    servo.write(pos);
  }
}

//this is the main movement function
void gotoposition(int pan, int tilt, int speed)
{
  // the servo dosn't like going below 5
  // so do a chech..
  // this can be removed if the servo dosn't behave badly.
  if (pan < MIN_SERVO_VALUE) { pan = MIN_SERVO_VALUE;}
  if (tilt < MIN_SERVO_VALUE) { tilt = MIN_SERVO_VALUE;}

  int panPosition = panServo.read();
  int tiltPosition = tiltServo.read();

// first we move the pan 1 step
// then we move the tilt one step
// this will give you a smooth diagnol
// rather than do all pan then tilt.
  while (pan != panPosition)
  {
    if (panPosition < pan)
    {
      //Serial.println(panPosition);
      increment(panServo);
    }
    else
    {
      decrement(panServo);
    //  Serial.println(panPosition);
    }

    if (tiltPosition < tilt)
    {
      increment(tiltServo);
    }
    else
    {
      decrement(tiltServo);
    }

    delay(speed);
    panPosition = panServo.read();
    tiltPosition = tiltServo.read();
  }

 // if there is any tilt not compleated in
 // the previous while we do it here..
 // yes it is the same as the code above and should be refactored 8-(
  while (tilt != tiltPosition)
  {
    if (tiltPosition < tilt)
    {
      increment(tiltServo);
    }
    else
    {
      decrement(tiltServo);
    }
    tiltPosition = tiltServo.read();
    delay(speed);
  }
}

void findDistance(){
  VL53L0X_RangingMeasurementData_t measure;

  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
}

void setup()
{
  panServo.attach(PAN_PIN); // attaches the servo on GIO2 to the servo object
  tiltServo.attach(TILT_PIN);
  Serial.begin(9600);

  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
}

void test()
{
  gotoposition(0, 0, 15);
  gotoposition(0, 180, 15);
  gotoposition(180, 180, 15);
  //  gotoposition( 180, 180, 15);
}
void nod(int speed){
  gotoposition(90, 90, speed);
  gotoposition(90, 180, speed);
  gotoposition(90, 0, speed);
  gotoposition(90, 90, speed);
  delay(1000);
}

void map_area(int speed){
gotoposition(85, 90, speed);
for (int i = 85; i < 120; i++) {
  gotoposition(i, 90, speed);
  findDistance();
}
for (int i = 120; i > 85; i--) {
  gotoposition(i, 90, speed);
  findDistance();
}

//  delay(1000);
}

void loop()
{
//pan
// gotoposition(11,0, 10);
// delay(1000);
// gotoposition(180, 180, 10);
//


map_area(300);

  //test();


}

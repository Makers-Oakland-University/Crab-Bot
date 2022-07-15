// Omnidirectional Robot for Maker Faire 2017
// Code by Steven Stewart

#include <XBOXRECV.h>
#include <Servo.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
XBOXRECV Xbox(&Usb);

Servo LeftServo;
Servo RightServo;
int LeftServoPin = 14;
int RightServoPin = 15;

int yPin = 22;
int bPin = 24;
int xPin = 26;
int aPin = 28;

// Motor A (left front)
int dirPinA = 2;
int speedPinA = 3; // Needs to be a PWM pin to be able to control motor speed
int speedA = 0;
int directionA = 0;

// Motor B (left rear)
int dirPinB = 4;
int speedPinB = 5; // Needs to be a PWM pin to be able to control motor speed
int speedB = 0;
int directionB = 0;

// Motor C (right front)
int dirPinC = 7;
int speedPinC = 6; // Needs to be a PWM pin to be able to control motor speed
int speedC = 0;
int directionC = 0;

// Motor D (right rear)
int dirPinD = 8;
int speedPinD = 9; // Needs to be a PWM pin to be able to control motor speed
int speedD = 0;
int directionD = 0;

double vel_x;
double vel_y;
double vel_w;

double vel_a;
double vel_b;
double vel_c;
double vel_d;

double modifiedMap(double x, double in_min, double in_max, double out_min, double out_max)
{
  double temp = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  temp = (long) (255 * temp + .5);
  return (double) temp / 255;
}

void setup() {
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }

  pinMode(dirPinA, OUTPUT);
  pinMode(speedPinA, OUTPUT);
  pinMode(dirPinB, OUTPUT);
  pinMode(speedPinB, OUTPUT);
  pinMode(dirPinC, OUTPUT);
  pinMode(speedPinC, OUTPUT);
  pinMode(dirPinD, OUTPUT);
  pinMode(speedPinD, OUTPUT);
  pinMode(aPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  pinMode(yPin, OUTPUT);
  pinMode(xPin, OUTPUT);

  LeftServo.attach(LeftServoPin);
  RightServo.attach(RightServoPin);
}

void loop() {
  Usb.Task();
  if (Xbox.XboxReceiverConnected) {
    if (Xbox.Xbox360Connected[0]) {
      int inX = Xbox.getAnalogHat(LeftHatY, 0);
      inX = constrain(inX, -32767, 32767);  // prevents overflow when abs() is called

      int inY = Xbox.getAnalogHat(LeftHatX, 0);
      inY = constrain(inY, -32767, 32767); // prevents overflow when abs() is called
      inY = -inY;

      int inW = Xbox.getAnalogHat(RightHatX, 0);
      inW = constrain(inW, -32767, 32767); // prevents overflow when abs() is called
      inW = -inW;

      if (inX > 7500 || inX < -7500 || inY > 7500 || inY < -7500 || inW > 7500 || inW < -7500) {
        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Map inputs to velocities

        if (inX < -7500) {
          vel_x = modifiedMap(inX, -32767, -7500, -1, 0);
        }
        else if (inX > 7500) {
          vel_x = modifiedMap(inX, 7500, 32767, 0, 1);
        }
        else {
          vel_x = 0;
        }
        vel_x = constrain(vel_x, -1, 1);
        ///////////////////////////////////////////////////
        if (inY < -7500) {
          vel_y = modifiedMap(inY, -32767, -7500, -1, 0);
        }
        else if (inY > 7500) {
          vel_y = modifiedMap(inY, 7500, 32767, 0, 1);
        }
        else {
          vel_y = 0;
        }
        vel_y = constrain(vel_y, -1, 1);
        ///////////////////////////////////////////////////
        if (inW < -7500) {
          vel_w = modifiedMap(inW, -32767, -7500, -1, 0);
        }
        else if (inW > 7500) {
          vel_w = modifiedMap(inW, 7500, 32767, 0, 1);
        }
        else {
          vel_w = 0;
        }
        vel_w = constrain(vel_w, -1, 1);

        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Translate command velocities into wheel velocities

        vel_a = vel_x - vel_y - vel_w;

        vel_b = vel_x + vel_y - vel_w;

        vel_c = vel_x + vel_y + vel_w;

        vel_d = vel_x - vel_y + vel_w;

        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Translate wheel velocities into speed and direction commands

        if (vel_a < 0) {
          directionA = 2;
          vel_a = abs(vel_a);
        }
        else {
          directionA = 1;
        }
        ///////////////////////
        if (vel_b < 0) {
          directionB = 1;
          vel_b = abs(vel_b);
        }
        else {
          directionB = 2;
        }
        ///////////////////////
        if (vel_c < 0) {
          directionC = 2;
          vel_c = abs(vel_c);
        }
        else {
          directionC = 1;
        }
        ///////////////////////
        if (vel_d < 0) {
          directionD = 2;
          vel_d = abs(vel_d);
        }
        else {
          directionD = 1;
        }

        if (!vel_w || (!vel_x && !vel_y) ) {
          speedA = modifiedMap(vel_a, 0, 1.8, 0, 255);
          speedA = constrain(speedA, 0, 255);

          speedB = modifiedMap(vel_b, 0, 1.8, 0, 255);
          speedB = constrain(speedB, 0, 255);

          speedC = modifiedMap(vel_c, 0, 1.8, 0, 255);
          speedC = constrain(speedC, 0, 255);

          speedD = modifiedMap(vel_d, 0, 1.8, 0, 255);
          speedD = constrain(speedD, 0, 255);
        }
        else {
          speedA = modifiedMap(vel_a, 0, 2.8, 0, 255);
          speedA = constrain(speedA, 0, 255);

          speedB = modifiedMap(vel_b, 0, 2.8, 0, 255);
          speedB = constrain(speedB, 0, 255);

          speedC = modifiedMap(vel_c, 0, 2.8, 0, 255);
          speedC = constrain(speedC, 0, 255);

          speedD = modifiedMap(vel_d, 0, 2.8, 0, 255);
          speedD = constrain(speedD, 0, 255);
        }
      }
      else {  // if input is inside deadband send a stop command to motors
        directionA = 0;
        directionB = 0;
        directionC = 0;
        directionD = 0;

        vel_x = 0;
        vel_y = 0;
        vel_w = 0;
      }

      //////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Read servo trigger inputs

      int LeftTrigger = Xbox.getButtonPress(L2, 0);
      int LeftServoAngle = map(LeftTrigger, 0, 255, 0, 180);
      LeftServoAngle = constrain(LeftServoAngle, 0, 180);
      LeftServo.write(LeftServoAngle);

      int RightTrigger = Xbox.getButtonPress(R2, 0);
      int RightServoAngle = map(RightTrigger, 255, 0, 0, 180);
      RightServoAngle = constrain(RightServoAngle, 0, 180);
      RightServo.write(RightServoAngle);

      //////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Read voice commands

      bool aButton = Xbox.getButtonPress(A, 0);
      bool bButton = Xbox.getButtonPress(B, 0);
      bool xButton = Xbox.getButtonPress(X, 0);
      bool yButton = Xbox.getButtonPress(Y, 0);
      
      if (aButton) { //A is pressed
        digitalWrite(aPin, LOW);
      }
      else
      {
        digitalWrite(aPin, HIGH);
      }
      
      if (bButton) { //B is pressed
        digitalWrite(bPin, LOW);
      }
      else
      {
        digitalWrite(bPin, HIGH);
      }
       
      if (yButton) { //Y is pressed
        digitalWrite(yPin, LOW);
      }
      else
      {
        digitalWrite(yPin, HIGH);
      }
      
      if (xButton) { //X is pressed
        digitalWrite(xPin, LOW);
      }
      else
      {
        digitalWrite(xPin, HIGH);
      }

      //////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Send commands to motors

      //______________Motor A______________
      // left front

      if (directionA == 1) { // Motor A Forward
        analogWrite(speedPinA, speedA); //Sets speed variable via PWM
        digitalWrite(dirPinA, HIGH);
      }
      else if (directionA == 2) { // Motor A Reverse
        analogWrite(speedPinA, speedA);
        digitalWrite(dirPinA, LOW);
      }
      else {
        // turn off motor A
        digitalWrite(dirPinA, LOW);
        digitalWrite(speedPinA, LOW);
      }

      //______________Motor B______________
      // left rear

      if (directionB == 1) { // Motor B Forward
        analogWrite(speedPinB, speedB);//Sets speed variable via PWM
        digitalWrite(dirPinB, LOW);
      }
      else if (directionB == 2) { // Motor B Reverse
        analogWrite(speedPinB, speedB);
        digitalWrite(dirPinB, HIGH);
      }
      else { // turn all the connections off if no joystick input:
        // turn off motor B
        digitalWrite(dirPinB, LOW);
        digitalWrite(speedPinB, LOW);
      }

      //______________Motor C______________
      // right front

      if (directionC == 1) { // Motor C Forward
        analogWrite(speedPinC, speedC);//Sets speed variable via PWM
        digitalWrite(dirPinC, LOW);
      }
      else if (directionC == 2) { // Motor C Reverse
        analogWrite(speedPinC, speedC);
        digitalWrite(dirPinC, HIGH);
      }
      else { // turn all the connections off if no joystick input:
        // turn off motor C
        digitalWrite(dirPinC, LOW);
        digitalWrite(speedPinC, LOW);
      }

      //______________Motor D______________
      // right rear

      if (directionD == 1) { // Motor D Forward
        analogWrite(speedPinD, speedD);//Sets speed variable via PWM
        digitalWrite(dirPinD, LOW);
      }
      else if (directionD == 2) { // Motor D Reverse
        analogWrite(speedPinD, speedD);
        digitalWrite(dirPinD, HIGH);
      }
      else { // turn all the connections off if no joystick input:
        // turn off motor D
        digitalWrite(dirPinD, LOW);
        digitalWrite(speedPinD, LOW);
      }
    }
  }
}


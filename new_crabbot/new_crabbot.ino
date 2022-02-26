#include "declarations.h" //important info in this file. if you can't compile look here

/* Web control, sets up crab-bot as a wifi access point that
   serves a webpage on 192.168.4.1
   This is a temporary step to allow for control of crab bot in the mean-time
   until a proper controller is created.

   Simply power up crab bot, connect to the wifi access point "Crab-Bot Web Interface"
   and type "192.168.4.1" into your browser. At which point you will be able to
   control crab-bot using two joysticks (buttons for sounds coming eventually)
*/

//motor driver connections
#define FL_DIR 13
#define FL_PWM 14
#define BL_DIR 18
#define BL_PWM 17
#define FR_DIR 25
#define FR_PWM 23
#define BR_DIR 22
#define BR_PWM 21

//PWM Channels
#define FL_CHANNEL 0
#define BL_CHANNEL 1
#define FR_CHANNEL 2
#define BR_CHANNEL 3


void setup()
{
  Serial.begin(115200);
  initMotors();
  motor_test();

  initWebControl();
}


void loop()
{
  //only thing in the loop, nothing actually
  //goes past this point
  //If you want to do anything then it'll have to happen inside of onNewUserInput()
  //or create another thread.
  getWebInput();
}


void onNewUserInput() {
  Serial.printf("Control Values x:%d y:%d yaw:%d \n", x_input, y_input, yaw_input);
}


void initMotors() {
  pinMode(FL_DIR, OUTPUT);
  pinMode(FL_PWM, OUTPUT);
  pinMode(BL_DIR, OUTPUT);
  pinMode(BL_PWM, OUTPUT);
  pinMode(FR_DIR, OUTPUT);
  pinMode(FR_PWM, OUTPUT);
  pinMode(BR_DIR, OUTPUT);
  pinMode(BR_PWM, OUTPUT);


  //init PWM on the PWM pins for the motor controllers
  ledcSetup(FL_CHANNEL, 1000, 8);
  ledcAttachPin(FL_PWM, FL_CHANNEL);

  ledcSetup(BL_CHANNEL, 1000, 8);
  ledcAttachPin(BL_PWM, BL_CHANNEL);

  ledcSetup(FR_CHANNEL, 1000, 8);
  ledcAttachPin(FR_PWM, FR_CHANNEL);

  ledcSetup(BR_CHANNEL, 1000, 8);
  ledcAttachPin(BR_PWM, BR_CHANNEL);

  //init PWM on motor controller to 0
  ledcWrite(FL_CHANNEL, 0);
  ledcWrite(BL_CHANNEL, 0);
  ledcWrite(FR_CHANNEL, 0);
  ledcWrite(BR_CHANNEL, 0);

}

void motor_test() {
  Serial.println("Testing Front Left Motor");
  for (float a = -1.0; a < 1.0; a += 0.1) {
    driveMotors(a, 0, 0, 0);
    delay(100);
  }

  Serial.println("Testing Back Left Motor");
  for (float a = -1.0; a < 1.0; a += 0.1) {
    driveMotors(0, a, 0, 0);
    delay(100);
  }


  Serial.println("Testing Back Right Motor");
  for (float a = -1.0; a < 1.0; a += 0.1) {
    driveMotors(0, 0, a, 0);
    delay(100);
  }

  Serial.println("Testing Front Right Motor");
  for (float a = -1.0; a < 1.0; a += 0.1) {
    driveMotors(0, 0, 0, a);
    delay(100);
  }

  driveMotors(0, 0, 0, 0);
}

//sets motor values between -1 and 1 for front left, back left, back right, and front right respectively
void driveMotors(float fl, float bl, float br, float fr) {

  //set direction and PWM for each motor
  //set PWM based on value from -1 to 1
  ledcWrite(FL_CHANNEL, (int)abs(255.0 * fl));

  //if input is greater than 0 then write high, otherwise write low.
  digitalWrite(FL_DIR, fl > 0);

  //repeat for the rest

  ledcWrite(BL_CHANNEL, (int)abs(255.0 * bl));
  digitalWrite(BL_DIR, bl > 0);

  ledcWrite(BR_CHANNEL, (int)abs(255.0 * br));
  digitalWrite(BR_DIR, br < 0); //the right side motors are inverted 

  ledcWrite(FR_CHANNEL, (int)abs(255.0 * fr));
  digitalWrite(FR_DIR, fr < 0);
}

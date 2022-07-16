#include "declarations.h" //important info in this file. if you can't compile look here

/* Web control, sets up crab-bot as a wifi access point that
   serves a webpage on 192.168.4.1
   This is a temporary step to allow for control of crab bot in the mean-time
   until a proper controller is created.

   Simply power up crab bot, connect to the wifi access point "Crab-Bot Web Interface"
   and type "192.168.4.1" into your browser. At which point you will be able to
   control crab-bot using two joysticks (buttons for sounds coming eventually)
*/

void setup()
{
  Serial.begin(115200);
  initMotors();
  //  motor_test();
  initESPNOW();
  init_neopixels();

}

void loop()
{
  //only thing in the loop, nothing actually
  //goes past this point
  //If you want to do anything then it'll have to happen inside of onNewUserInput()
  //or create another thread.
  //  getWebInput();
  //  base_animation();
  spin_animation();
  delay(20);
}


void handleController(makers_controller_message controller_data) {
  double yaw_rate = 0;
  if (controller_data.left_trig)
    yaw_rate = 1.0;
  else if (controller_data.right_trig)
    yaw_rate = -1.0;

  double x_vel = ((controller_data.right_joy_x - 2048) / 4096.0);
  double y_vel = ((controller_data.right_joy_y - 2048) / 4096.0);

  if (abs(x_vel) < 0.05)
    x_vel = 0;
  if (abs(y_vel) < 0.05)
    y_vel = 0;

  inverseKinematics( x_vel, y_vel, yaw_rate);
}


void onNewUserInput() {
  Serial.printf("Control Values x:%d y:%d yaw:%d \n", x_input, y_input, yaw_input);
  //  inverseKinematics(x_input, y_input, yaw_input);

  double x = x_input / 100.0;
  double y = y_input / 100.0;
  double yaw = yaw_input / 100.0;

  inverseKinematics(x, y, yaw);
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
  double step_delay = 100.0;
  Serial.println("Testing Front Left Motor");
  for (float a = -1.0; a < 1.0; a += 0.1) {
    driveMotors(a, 0, 0, 0);
    delay(step_delay);
  }

  Serial.println("Testing Back Left Motor");
  for (float a = -1.0; a < 1.0; a += 0.1) {
    driveMotors(0, a, 0, 0);
    delay(step_delay);
  }


  Serial.println("Testing Back Right Motor");
  for (float a = -1.0; a < 1.0; a += 0.1) {
    driveMotors(0, 0, a, 0);
    delay(step_delay);
  }

  Serial.println("Testing Front Right Motor");
  for (float a = -1.0; a < 1.0; a += 0.1) {
    driveMotors(0, 0, 0, a);
    delay(step_delay);
  }

  driveMotors(0, 0, 0, 0);

  double kinematic_delay = 800;

  Serial.println("Moving Right");
  inverseKinematics(1.0, 0, 0);
  delay(kinematic_delay);
  Serial.println("Moving Left");
  inverseKinematics(-1.0, 0, 0);
  delay(kinematic_delay);

  Serial.println("Moving Forward");
  inverseKinematics(0, 1.0, 0);
  delay(kinematic_delay);

  Serial.println("Moving Backward");
  inverseKinematics(0, -1.0, 0);
  delay(kinematic_delay);


  Serial.println("Rotating clockwise");
  inverseKinematics(0, 0, 1.0);
  delay(kinematic_delay);

  Serial.println("Rotating conterclockwise");
  inverseKinematics(0, 0, -1.0);
  delay(kinematic_delay);

  inverseKinematics(0, 0, 0);
}

//convert x,y, and angular velocity to the values needed in order to move crab bot
void inverseKinematics(double x, double y, double yaw) {

  //if you're wondering why these are reversed, I have no idea.
  double x_vel = y / 10.0 ;
  double y_vel = (x / 10.0);
  double psi = (yaw / 10.0);

  //  double w1 =  A_CONST * x_vel - B_CONST * y_vel - C_CONST * psi;
  //  double w3 = A_CONST * x_vel + B_CONST * y_vel + C_CONST * psi;
  //  double w4 = A_CONST * x_vel + B_CONST * y_vel - C_CONST * psi;
  //  double w2 =  A_CONST * x_vel - B_CONST * y_vel + C_CONST * psi;

  double w1 = A_CONST * x_vel - B_CONST * y_vel - C_CONST * psi;
  double w2 = A_CONST * x_vel + B_CONST * y_vel - C_CONST * psi;
  double w3 = A_CONST * x_vel - B_CONST * y_vel + C_CONST * psi;
  double w4 = A_CONST * x_vel + B_CONST * y_vel + C_CONST * psi;

  Serial.printf("Driving motors (%f, %f, %f, %f)\n", w1, w2, w3, w4);

  driveMotors( w1, w2, w3, w4);
}

//sets motor values between -1 and 1 for front left, back left, back right, and front right respectively
void driveMotors(float front_left, float back_left, float back_right, float front_right) {

  //constrain the motor values across their normal range.
  float fl = constrain(front_left, -1.0, 1.0);
  float bl = constrain(back_left, -1.0, 1.0);
  float br = constrain(back_right, -1.0, 1.0);
  float fr = constrain(front_right, -1.0, 1.0);

  Serial.printf("Driving motors (front-left %f, back-left %f, back-right %f, front-right %f)\n", fl, bl, br, fr);

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

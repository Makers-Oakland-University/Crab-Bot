#define CRAB_BOT_LEFT_EYE_SERVO_BOARD_PIN 0
#define CRAB_BOT_RIGHT_EYE_SERVO_BOARD_PIN 15

#define CRAB_BOT_RIGHT_CLAW_ARM 13
#define CRAB_BOT_RIGHT_CLAW_PINCHER 14
#define CRAB_BOT_LEFT_CLAW_ARM 2
#define CRAB_BOT_LEFT_CLAW_PINCHER 1

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

#define NUM_SERVOS 16
float servo_positions[NUM_SERVOS];

Adafruit_PWMServoDriver PCA9685 = Adafruit_PWMServoDriver(0x40);

void updateServos() {
  for (int a = 0; a < NUM_SERVOS; a++)
    PCA9685.setPWM(a, 0, angleToPulse(servo_positions[a]));
}

void init_pwm_board() {
  Wire.begin(I2C_SDA, I2C_SCL);
  PCA9685.begin();
  PCA9685.setPWMFreq(60);
  setLeftEye(90);
  setRightEye(90);
  for (int a = 0; a < 16; a++)
    setServo(a, 90);
}

void setServo(int servo_num, double position) {
  
  //we cant set any servo we don't have, if someone tries just return
  if(servo_num >= NUM_SERVOS)
    return; 

  //no negative indexes either
  if(servo_num < 0)
    return; 

  servo_positions[servo_num] = position;    
    
}

int angleToPulse(int ang) {
  int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX); // map angle of 0 to 180 to Servo min and Servo max
  return pulse;
}



/* Interface function for the left and right eye
    these eyes must be kept in a specific region and these functions implement
    that restriction. driving the servos manually from outside this function is
    NOT RECOMMENDED.
*/
double left_eye_position = 90.0;
double right_eye_position = 90.0;
double left_eye_desired_position = 90.0;
double right_eye_desired_position = 90.0;

void updateEyes() {
  left_eye_position += 0.024 * (left_eye_desired_position - left_eye_position);
  setServo(
    CRAB_BOT_LEFT_EYE_SERVO_BOARD_PIN,
    constrain(left_eye_position, 90 - 70, 90 + 70));

  right_eye_position += 0.024 * (right_eye_desired_position - right_eye_position);
  setServo(
    CRAB_BOT_RIGHT_EYE_SERVO_BOARD_PIN,
    constrain(right_eye_position, 90 - 70, 90 + 70));
}

void setLeftEye(double position) {
  left_eye_desired_position = position;
}

void setRightEye(double position) {
  right_eye_desired_position = position;
}


/* Interface functions for the claws and pinchers */

void setLeftClawArm(double position) {
  setServo(CRAB_BOT_LEFT_CLAW_ARM, position);
}

void setRightClawArm(double position) {
  setServo(CRAB_BOT_RIGHT_CLAW_ARM, position);
}

void setLeftClawPincher(double position) {
  setServo(CRAB_BOT_LEFT_CLAW_PINCHER, position);
}

void setRightClawPincher(double position) {
  setServo(CRAB_BOT_RIGHT_CLAW_PINCHER, position);
}

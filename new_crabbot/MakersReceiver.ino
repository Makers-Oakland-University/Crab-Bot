/* Makers Receiver for crab-bot
    this file containes all the initalization and callback functions for crab-bot.
    The goal with this is to make it easy to expand crab bot later.
    The callback functions can be enabled below in the initalization function.
    simply uncomment the needed "receiver.registerButtonCallback(..." line
    and the corrosponding callback function

*/


void joystick_callback(float left_x, float left_y, float right_x, float right_y) {
  float clipped_left_x = (abs(left_x) > 0.14) ? left_x : 0.0;
  float clipped_left_y = (abs(left_y) > 0.14) ? left_y : 0.0;
  float clipped_right_x = (abs(right_x) > 0.14) ? right_x : 0.0;

  inverseKinematics(clipped_left_x, clipped_left_y, clipped_right_x);
}


//Switch 1 callback, enable below by uncommenting register button callback
void sw1_callback(int state) {
  playTrack(1);
}

//Switch 2 callback, enable below by uncommenting register button callback
void sw2_callback(int state) {
  playTrack(2);
}

//Switch 3 callback, enable below by uncommenting register button callback
void sw3_callback(int state) {
  playTrack(3);
}

//Switch 4 callback, enable below by uncommenting register button callback
void sw4_callback(int state) {
  crab_rave_mode = state;
  if (!state) {
    stopTrack();
    inverseKinematics(0, 0, 0);
  }
}

//Switch 5 callback, enable below by unco/mmenting register button callback
void sw5_callback(int state) {
  if (state)
    nextAnimation();
}

//Switch 6 callback, enable below by uncommenting register button callback
//void sw6_callback(int state) {
//  Serial.println("Switch 6 state changed");
//}

//Switch 7 callback, enable below by uncommenting register button callback
//void sw7_callback(int state) {
//  Serial.println("Switch 7 state changed");
//}

//Switch 8 callback, enable below by uncommenting register button callback
//void sw8_callback(int state) {
//  Serial.println("Switch 8 state changed");
//}

//Left joystick button, enable below by uncommenting register button callback
//void left_joystick_button_callback(int state) {
//  Serial.println("Switch on left joystick state changed");
//}

//Right joystick button, enable below by uncommenting register button callback
//void right_joystick_button_callback(int state) {
//  Serial.println("Switch on right joystick state changed");
//}


//left trigger callback, enable below by uncommenting register button callback
//void left_trigger_callback(int state) {
//  Serial.println("left trigger state changed");
//}

//left trigger callback, enable below by uncommenting register button callback
//void right_trigger_callback(int state) {
//  Serial.println("right trigger state changed");
//}

void initMakersReceiver() {
  receiver.startReceiver();

  // register joystick
  receiver.registerJoystickCallback(joystick_callback);
  receiver.setJoystickCallbackThreshold(0.03);


  /* Register the callbacks for the buttons.
      If you want to use one of the buttons on the controller for a function on crab bot uncomment the corrosponding
      buttton below (switch 1, 2, 3, etc) and the function above that it calls.
      When the button value changes on the controller that function will then be called with the
      current state of the button.

  */

  receiver.registerButtonCallback(MAKERS_CONTROLLER_SW1, sw1_callback); //Switch 1
  receiver.registerButtonCallback(MAKERS_CONTROLLER_SW2, sw2_callback); //Switch 2
  receiver.registerButtonCallback(MAKERS_CONTROLLER_SW3, sw3_callback); //Switch 3
  receiver.registerButtonCallback(MAKERS_CONTROLLER_SW4, sw4_callback); //Switch 4
  receiver.registerButtonCallback(MAKERS_CONTROLLER_SW5, sw5_callback); //Switch 5
  //  receiver.registerButtonCallback(MAKERS_CONTROLLER_SW6, sw6_callback); //Switch 6
  //  receiver.registerButtonCallback(MAKERS_CONTROLLER_SW7, sw7_callback); //Switch 7
  //  receiver.registerButtonCallback(MAKERS_CONTROLLER_SW8, sw8_callback); //Switch 8
  //  receiver.registerButtonCallback(MAKERS_CONTROLLER_SW_L_JOY, left_joystick_button_callback); //Button on left joystick
  //  receiver.registerButtonCallback(MAKERS_CONTROLLER_SW_R_JOY, right_joystick_button_callback); //Button on right joystick
  //  receiver.registerButtonCallback(MAKERS_CONTROLLER_SW_L_TRIG, left_trigger_callback); //Button on right joystick
  //  receiver.registerButtonCallback(MAKERS_CONTROLLER_SW_R_TRIG, right_trigger_callback); //Button on right joystick


}

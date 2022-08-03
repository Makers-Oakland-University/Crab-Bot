#include "declarations.h" //important info in this file. if you can't compile look here


void setup()
{
  Serial.begin(115200);

  initMotors();

  //if you're working on the motors and want to have each one activate individually
  //to verify that everything's connected correty then use this function
  //  motor_test();

  initMakersReceiver();
  init_neopixels();
  initSound();
  init_pwm_board();

  setAnimation(CRABBOT_STANDARD_ANIMATION);

}

void loop()
{
  delay(10);

  if (crab_rave_mode)
    crab_rave();
  else
    play_animation();

//  for(int a = 0; a < 10; a++){
//    Serial.println("Playing Track #" + String(a));
//    playTrack(a); 
//    delay(2000); 
//  }


  //call checkmotors() this will cause crabbot to come to a full stop in
  //the event that it doesn't receive controller commands within a second.
  checkMotors();
}

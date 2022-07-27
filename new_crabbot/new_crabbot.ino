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

  setAnimation(CRABBOT_STANDARD_ANIMATION);

}

void loop()
{

  play_animation();
  delay(10);

  if (crab_rave_mode)
    crab_rave();
}

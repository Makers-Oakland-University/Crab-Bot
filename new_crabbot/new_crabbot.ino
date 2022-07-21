#include "declarations.h" //important info in this file. if you can't compile look here

void setup()
{
  Serial.begin(115200);

  initMotors();
  //  motor_test();

  initMakersReceiver();
  init_neopixels();

}

void loop()
{
  //neopixel animations
  //  base_animation();
  spin_animation();
  //  quick_pattern();
  delay(20);
}

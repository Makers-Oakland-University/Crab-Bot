
#define CRAB_RAVE_DELAY 3560
#define CRAB_RAVE_ANIMATION_DELAY 100
#define CRAB_RAVE_LEFT_RIGHT_TIME 500
unsigned long last_crab_rave_song_play = 0;
unsigned long last_crab_rave_animation_launched = 0;


void crab_rave() {
  //animate neopixel
  if ((last_crab_rave_animation_launched + CRAB_RAVE_ANIMATION_DELAY) < millis()) {
    crab_rave_animation();
    last_crab_rave_animation_launched = millis();
  }


  //play crab_rave
  if ((last_crab_rave_song_play + CRAB_RAVE_DELAY) < millis()) {
    playTrack(4);
    last_crab_rave_song_play = millis();
  }


  /* MOTOR DRIVE EQUATION NEEDS IMPROVEMENT
      AT THE MOMENT IT IS NOT A SMOOTH TRANSITION  */

  float motor_drive = (((float)(millis() % (CRAB_RAVE_LEFT_RIGHT_TIME * 2)) - CRAB_RAVE_LEFT_RIGHT_TIME) / ((float)CRAB_RAVE_LEFT_RIGHT_TIME)) * 0.65;
  Serial.println(motor_drive);
  inverseKinematics(
    motor_drive,
    0,
    0);

}

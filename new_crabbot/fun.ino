
#define CRAB_RAVE_DELAY 3500
#define CRAB_RAVE_ANIMATION_DELAY 100
#define CRAB_RAVE_LEFT_RIGHT_TIME 1000
unsigned long last_crab_rave_song_play = 0;
unsigned long last_crab_rave_animation_launched = 0;


void crab_rave() {
  //animate neopixel
  if ((last_crab_rave_animation_launched + CRAB_RAVE_ANIMATION_DELAY) < millis()) {
    crab_rave_animation();
    last_crab_rave_animation_launched = millis();
  }
 
  Serial2.println("#2");

  //play crab_rave

  float motor_drive = (millis() % CRAB_RAVE_LEFT_RIGHT_TIME) / ((float) CRAB_RAVE_LEFT_RIGHT_TIME) ;
  float drive_sin = sin(motor_drive * 2 * PI);
  drive_sin *= 0.35;

  Serial.println(drive_sin);
  inverseKinematics(
    drive_sin,
    0,
    0);

}

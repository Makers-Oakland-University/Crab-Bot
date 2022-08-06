//constants and declarations in declarations.h

/* This tab is intended to contain all of the NEOPIXEL
    animations that are created to be shown on crab-bot.
    The animations created here can be called by the play_animation()


    ANIMATION IDENTIFIERS DEFINED IN DECLARATIONS.H
*/

int current_animation = 0;

void setAnimation(int value) {
  current_animation = value;
}

void nextAnimation() {
  current_animation++;
}

void play_animation() {
  switch (current_animation) {
    case CRABBOT_STANDARD_ANIMATION:
      standard_animation();
      break;
    case CRABBOT_SPLIT_ANIMATION:
      split_animation();
      break;
    case CRABBOT_CONTROLS_ANIMATION:
      controller_input_animtion();
      break;
    default:
      Serial.printf("No animation available for value %d\n", current_animation);
      current_animation = 0;
      break;
  }
}

void init_neopixels() {
  Serial.print("Starting Neopixels...");
  pixels.begin(); // This initializes the NeoPixel library.
  Serial.println("Done");
}

//value range 0 - 1
void setPixel(int index, float red, float blue, float green) {
  pixels.setPixelColor(index, pixels.Color(red * 255, blue * 255, green * 255));
}



//for animations please create a function that increments the animation every time it's called
//this way the animation can be triggerred without locking the thread.
/********************************
      STANDARD ANIMATION
 ********************************/
int standard_animation_count = 0;
int standard_animation_delta = 1;
void standard_animation() {
  for (int a = 0; a < NUM_NEOPIXEL; a++)
    if (a == standard_animation_count)
      setPixel(a, 1.0, 0, 0);
    else if (abs(a - standard_animation_count) < 4)
      setPixel(a, 0.04, 0, 0);
    else
      setPixel(a, 0.05, 0.01, 0.0);
  standard_animation_count += standard_animation_delta;
  pixels.show();

  if (standard_animation_count > NUM_NEOPIXEL || standard_animation_count < 0)
    standard_animation_delta *= -1;
}

/********************************
      Split ANIMATION
 ********************************/
int split_animation_count = 0;
void split_animation() {
  for (int a = 0; a < NUM_NEOPIXEL; a++) {
    setPixel(a, 0, 0, 0);
  }

  setPixel(NUM_NEOPIXEL / 2 - split_animation_count, 1, 0, 0);
  setPixel(NUM_NEOPIXEL / 2 + split_animation_count, 1, 0, 0);

  if (split_animation_count > NUM_NEOPIXEL / 2)
    split_animation_count = 0;
  split_animation_count++;
  pixels.show();
}


/********************************
    CONTROLLER INPUT ANIMTION
 ********************************/

void controller_input_animtion() {
  for (int a = 0; a < NUM_NEOPIXEL; a++) {
    setPixel(a, abs(receiver.readLeftJoystickX()), abs(receiver.readLeftJoystickY()), abs(receiver.readRightJoystickX() ));
  }
  pixels.show();
}

/********************************
      CRAB RAVE ANIMATION
 ********************************/
int crab_rave_counter = 0;

void crab_rave_animation() {
  for (int a = 0; a < NUM_NEOPIXEL; a++) {
    setPixel((a + crab_rave_counter) % NUM_NEOPIXEL, ( a & 0x04) >> 2 , 0, (a & 0x01));
  }

  Serial.printf("crab_rave_counter %d\n", crab_rave_counter);
  crab_rave_counter++;
  pixels.show();
}

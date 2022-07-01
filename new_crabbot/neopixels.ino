
//constants and declarations in declarations.h

void init_neopixels() {
  pixels.begin(); // This initializes the NeoPixel library.
}

void quick_pattern() {
  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        for (int l = 0; l < NUM_NEOPIXEL; l++) {

          pixels.setPixelColor(l, pixels.Color(i * 255, j * 255, k * 255)); // Moderately bright green color.

          pixels.show(); // This sends the updated pixel color to the hardware.
          //        delay(1); // Delay for a period of time (in milliseconds).

        }
        delay(100);
      }
    }
  }
}

//value range 0 - 1
void setPixel(int index, float red, float blue, float green) {
  pixels.setPixelColor(index, pixels.Color(red * 255, blue * 255, green * 255));
}

int current_special_index = 0;
int animation_direction = -1;

void base_animation() {
  for (int a = 0; a < NUM_NEOPIXEL; a++) {
    if (a > current_special_index)
      setPixel(a, 1.0, 0.1, 0);
    //setPixel(a, 0, 0, 1.0);
    else
      setPixel(a, 1.0, 0, 0);
  }
  pixels.show();

  current_special_index += animation_direction;
  if (current_special_index > NUM_NEOPIXEL)
    animation_direction *= -1;
  if (current_special_index < 0)
    animation_direction *= -1;
}

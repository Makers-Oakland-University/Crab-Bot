#include <Adafruit_NeoPixel.h>

#define PIN 18
#define NUM_LED 300

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LED, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop()
{

  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        for (int l = 0; l < NUM_LED; l += 5) {

          pixels.setPixelColor(l, pixels.Color(i * 255, j * 255, k * 255)); // Moderately bright green color.
          pixels.setPixelColor(l + 1, pixels.Color(i * 255, j * 255, k * 255)); // Moderately bright green color.
          pixels.setPixelColor(l + 2, pixels.Color(i * 255, j * 255, k * 255)); // Moderately bright green color.
          pixels.setPixelColor(l + 3, pixels.Color(i * 255, j * 255, k * 255)); // Moderately bright green color.
          pixels.setPixelColor(l + 4, pixels.Color(i * 255, j * 255, k * 255)); // Moderately bright green color.


          pixels.show(); // This sends the updated pixel color to the hardware.
          //        delay(1); // Delay for a period of time (in milliseconds).
        }
      }
    }
  }
}

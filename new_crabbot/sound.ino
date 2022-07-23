void initSound() {
  Serial.println("Starting Adafruit Soundboard");
  Serial2.begin(9600);

  if (!sfx.reset()) {
    Serial.println("Not found, sound is not available. please check connections");
    return;
  }

  Serial.println("Found Adafruit Sound Shield");
}


//todo make song triggered by name instead of song number (incase we add more songs)
void playTrack(int num) {
  Serial.printf("Playing track #%d\n", num);

  boolean success = sfx.playTrack((uint8_t)num);

  if(!success)
    Serial.println("Failed to play track, check connections to soundboard");

}

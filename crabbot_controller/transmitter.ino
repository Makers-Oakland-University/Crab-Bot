//screen size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

//button defines (probably going to want to copy these to any other script for the controller)
#define LEFT_JOY_Y 34
#define LEFT_JOY_X 35
#define LEFT_JOY_SW 4

#define RIGHT_JOY_X 32
#define RIGHT_JOY_Y 33
#define RIGHT_JOY_SW 4

#define SW1 17
#define SW2 16
#define SW3 19
#define SW4 18
#define SW5 25
#define SW6 13
#define SW7 36
#define SW8 26
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.*/

#define LEFT_TRIG 23
#define RIGHT_TRIG 27

#define BUTTON_DISPLAY_GRID_SIZE SCREEN_WIDTH / 6

//array for simplification of button reading code.
int buttons[] = {
  SW1,
  SW2,
  SW3,
  SW4,
  SW5,
  SW6,
  SW7,
  SW8,
  LEFT_TRIG,
  RIGHT_TRIG,
  LEFT_JOY_SW,
  RIGHT_JOY_SW
};

//init display.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, & Wire, -1);

void initESPNOW() {
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void initIO() {
  pinMode(LEFT_JOY_Y, INPUT);
  pinMode(LEFT_JOY_X, INPUT);
  pinMode(LEFT_JOY_SW, INPUT);

  pinMode(RIGHT_JOY_Y, INPUT);
  pinMode(RIGHT_JOY_X, INPUT);
  pinMode(RIGHT_JOY_SW, INPUT);

  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);
  pinMode(SW5, INPUT);
  pinMode(SW6, INPUT);
  pinMode(SW7, INPUT);
  pinMode(SW8, INPUT);

  pinMode(LEFT_TRIG, INPUT);
  pinMode(RIGHT_TRIG, INPUT);

  //wait for display.
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  delay(500);
  display.clearDisplay();

}


void drawTestData() {

  //clear display
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  //makers :)
  display.println("Crab-Bot");

  //loop through each button in the array, arrange them onto a 6x2 grid
  for (int a = 0; a < 12; a++) {

    //move to grid position
    display.setCursor((a % 6) * BUTTON_DISPLAY_GRID_SIZE, 10 + (10 * (a / 6)));

    //if button is pressed display white, otherwise display black.
    if (digitalRead(buttons[a])) {
      display.setTextColor(WHITE);
    } else {
      display.setTextColor(BLACK);
    }

    //print button with above formatting and location.
    display.print("S" + String(a + 1));

  }

  //read joysticks.
  display.setTextColor(WHITE);

  //left joystick
  display.setCursor(0, 30);
  display.print("Left x:" + String(analogRead(LEFT_JOY_X)) + " y:" + String(analogRead(LEFT_JOY_Y)));

  //right joystick
  display.setCursor(0, 40);
  display.print("Right x:" + String(analogRead(RIGHT_JOY_X)) + " y:" + String(analogRead(RIGHT_JOY_Y)));

  //push to display.
  display.display();

}


void readAndSendControllerData() {
  drawTestData();

  myData.left_joy_y = analogRead(LEFT_JOY_Y);
  myData.left_joy_x = analogRead(LEFT_JOY_X);

  myData.right_joy_y = analogRead(RIGHT_JOY_Y);
  myData.right_joy_x = analogRead(RIGHT_JOY_X);

  myData.sw1 = digitalRead(SW1);
  myData.sw2 = digitalRead(SW2);
  myData.sw3 = digitalRead(SW3);
  myData.sw4 = digitalRead(SW4);
  myData.sw5 = digitalRead(SW5);
  myData.sw6 = digitalRead(SW6);
  myData.sw7 = digitalRead(SW7);
  myData.sw8 = digitalRead(SW8);

  myData.left_trig = digitalRead(LEFT_TRIG);
  myData.right_trig = digitalRead(RIGHT_TRIG);
  myData.left_joy_sw = digitalRead(LEFT_JOY_SW);
  myData.right_joy_sw = digitalRead(RIGHT_JOY_SW);

  Serial.printf("Controller inputs\n");
  Serial.printf("Right Joy (%u, %u) Left Joy (%u, %u)\n", myData.right_joy_x, myData.right_joy_y, myData.left_joy_x, myData.left_joy_y);
  Serial.printf("Switches 1:%u, 2:%u, 3:%u, 4:%u, 5:%u, 6:%u, 7:%u, 8:%u \n", myData.sw1, myData.sw2, myData.sw3, myData.sw4, myData.sw5, myData.sw6, myData.sw7, myData.sw8);
  Serial.printf("right trig %u  left trig %u\n", myData.left_trig, myData.right_trig);

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }

}

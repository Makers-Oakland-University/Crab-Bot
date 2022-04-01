/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Left Joy x-%d y-%d sw-%d\n", myData.left_joy_x, myData.left_joy_y, myData.left_joy_sw);
  Serial.printf("Right Joy x-%d y-%d sw-%d\n", myData.right_joy_x, myData.right_joy_y, myData.right_joy_sw);
  Serial.printf("Buttons: \n   sw1-%d sw2-%d sw3-%d sw4-%d\n   sw5-%d sw6-%d sw7-%d sw8-%d\n   left_trig-%d right_trig-%d\n",
                myData.sw1, myData.sw2, myData.sw3, myData.sw4, myData.sw5, myData.sw6, myData.sw7, myData.sw8, myData.left_trig, myData.right_trig);

  double yaw_rate = 0;
  if (myData.left_trig)
    yaw_rate = 1.0;
  else if (myData.right_trig)
    yaw_rate = -1.0;

  double x_vel =((myData.right_joy_x - 2048) / 4096.0);
  double y_vel = ((myData.right_joy_y - 2048) / 4096.0);

  if (abs(x_vel) < 0.05)
    x_vel = 0;
  if (abs(y_vel) < 0.05)
    y_vel = 0;

  inverseKinematics( x_vel, y_vel, yaw_rate);


}

void initESPNOW() {
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  Serial.print("Device MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

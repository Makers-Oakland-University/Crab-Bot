#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct makers_controller_message {
  int left_joy_y;
  int left_joy_x;
  int right_joy_y;
  int right_joy_x;
  int sw1;
  int sw2;
  int sw3;
  int sw4;
  int sw5;
  int sw6;
  int sw7;
  int sw8;
  int left_trig;
  int right_trig;
  int left_joy_sw;
  int right_joy_sw;
} makers_controller_message;


// Create a struct_message called myData
makers_controller_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  initIO();
  initESPNOW();
}

void loop() {

  readAndSendControllerData();
 delay(25);

}

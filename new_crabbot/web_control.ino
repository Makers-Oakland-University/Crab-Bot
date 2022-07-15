
#ifdef USE_WEB_CONTROL
// handle http messages
//this converts the data taken from the user on the web page and parses it into
//values that we are able to use in the rest of the code.
void handle_message(WebsocketsMessage msg) {
  //  Serial.println(msg.data());
  int first_item = msg.data().indexOf(',');
  x_input = msg.data().substring(0, first_item).toInt();

  int second_item = msg.data().indexOf(',', first_item + 1);
  y_input = msg.data().substring(first_item + 1, second_item).toInt();

  int third_item = msg.data().indexOf(',', second_item + 1);
  yaw_input = msg.data().substring(second_item + 1, third_item).toInt();

  //here for in the future if we want to use it
  int fourth_item = msg.data().indexOf(',', third_item + 1);
  int right_stick_y = msg.data().substring(third_item + 1).toInt();

  onNewUserInput();
  //debug
  //Serial.printf("Data Rec x:%d y:%d yaw:%d ry:%d\n", x, y, yaw, right_stick_y);

    
}

void initWebControl() {
  // Create AP
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // HTTP handler assignment
  webserver.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });

  // start server
  webserver.begin();
  server.listen(82);
  if (server.available())
    Serial.println("Server is active!");
  else
    Serial.println("Server is unable to start");

}


void getWebInput() {
  auto client = server.accept();
  client.onMessage(handle_message);
  while (client.available()) {
    client.poll();
  }
}
#endif 

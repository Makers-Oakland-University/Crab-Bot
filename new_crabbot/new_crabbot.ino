#include "declarations.h" //important info in this file. if you can't compile look here

/* Web control, sets up crab-bot as a wifi access point that 
 * serves a webpage on 192.168.4.1
 * This is a temporary step to allow for control of crab bot in the mean-time 
 * until a proper controller is created. 
 * 
 * Simply power up crab bot, connect to the wifi access point "Crab-Bot Web Interface"
 * and type "192.168.4.1" into your browser. At which point you will be able to 
 * control crab-bot using two joysticks (buttons for sounds coming eventually) 
 */


void setup()
{
  Serial.begin(115200);
  initWebControl();
}


void loop()
{
  //only thing in the loop, nothing actually 
  //goes past this point 
  //If you want to do anything then it'll have to happen inside of onNewUserInput() 
  //or create another thread. 
  getWebInput();
}


void onNewUserInput(){
    Serial.printf("Control Values x:%d y:%d yaw:%d \n", x_input, y_input, yaw_input); 
}

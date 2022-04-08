#include <Arduino.h>
#include "Arduino.h"
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <esp_now.h>
#include <WiFi.h>

//note requires the following libaries to be installed:
// https://github.com/me-no-dev/ESPAsyncWebServer
// https://github.com/me-no-dev/AsyncTCP
// https://github.com/gilmaimon/ArduinoWebsockets
// follows the example presented here: https://github.com/nkmakes/SMARS-esp32 & https://nkmakes.github.io/2020/09/02/esp32-tank-robot-joystick-http-web-control/
// with elements of https://iotespresso.com/create-captive-portal-using-esp32/
// used a simple javascript joystick created bo Roberto D'Amico https://www.cssscript.com/onscreen-joystick/

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

const char* ssid = "Crab-Bot Web Interface"; //Enter SSID
const char* password = ""; //Enter Password

using namespace websockets;
WebsocketsServer server;
AsyncWebServer webserver(80);

//global variables that control crab bot 
int x_input = 0; 
int y_input = 0; 
int yaw_input = 0; 

// Create a struct_message called myData
makers_controller_message myData;

//motor driver connections
#define FL_DIR 14
#define FL_PWM 13
#define BL_DIR 18
#define BL_PWM 17
#define FR_DIR 25
#define FR_PWM 23
#define BR_DIR 22
#define BR_PWM 21

//PWM Channels
#define FL_CHANNEL 0
#define BL_CHANNEL 1
#define FR_CHANNEL 2
#define BR_CHANNEL 3

//drive constants for inverse kinematics
#define WHEEL_RADIUS 0.05
#define BOT_WIDTH 0.410
#define BOT_LENGTH 0.3

#define A_CONST (1 / WHEEL_RADIUS)
#define B_CONST (A_CONST)
#define C_CONST (0.5 * ((BOT_WIDTH + BOT_LENGTH) / WHEEL_RADIUS))


















































/*******************************************
 * The web page that is served to the connected device. 
 * This contains javascript which gives the webpage the ability to interact with our stuff here on the ESP32
 * This is derived from some of content posted in the links above.  
 */

 const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Crab-bot WiFi Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script>
let StickStatus={xPosition:0,yPosition:0,x:0,y:0,cardinalDirection:"C"};var JoyStick=function(t,e,i){var o=void 0===(e=e||{}).title?"joystick":e.title,n=void 0===e.width?0:e.width,a=void 0===e.height?0:e.height,r=void 0===e.internalFillColor?"#00AA00":e.internalFillColor,c=void 0===e.internalLineWidth?2:e.internalLineWidth,s=void 0===e.internalStrokeColor?"#003300":e.internalStrokeColor,d=void 0===e.externalLineWidth?2:e.externalLineWidth,u=void 0===e.externalStrokeColor?"#008000":e.externalStrokeColor,h=void 0===e.autoReturnToCenter||e.autoReturnToCenter;i=i||function(t){};t=document.getElementById(t);t.style.touchAction="none";var S=document.createElement("canvas");S.id=o,0===n&&(n=t.clientWidth),0===a&&(a=t.clientHeight),S.width=n,S.height=a,t.appendChild(S);var l=S.getContext("2d"),f=0,k=2*Math.PI,g=(S.width-(S.width/2+10))/2,x=g+5,v=g+30,P=S.width/2,m=S.height/2,C=S.width/10,p=-1*C,y=S.height/10,w=-1*y,L=P,F=m;function E(){l.beginPath(),l.arc(P,m,v,0,k,!1),l.lineWidth=d,l.strokeStyle=u,l.stroke()}function W(){l.beginPath(),L<g&&(L=x),L+g>S.width&&(L=S.width-x),F<g&&(F=x),F+g>S.height&&(F=S.height-x),l.arc(L,F,g,0,k,!1);var t=l.createRadialGradient(P,m,5,P,m,200);t.addColorStop(0,r),t.addColorStop(1,s),l.fillStyle=t,l.fill(),l.lineWidth=c,l.strokeStyle=s,l.stroke()}function T(){let t="";var e=L-P,i=F-m;return w<=i&&i<=y&&(t="C"),i<w&&(t="N"),y<i&&(t="S"),e<p&&("C"===t?t="W":t+="W"),C<e&&("C"===t?t="E":t+="E"),t}"ontouchstart"in document.documentElement?(S.addEventListener("touchstart",function(t){f=1},!1),document.addEventListener("touchmove",function(t){1===f&&t.targetTouches[0].target===S&&(L=t.targetTouches[0].pageX,F=t.targetTouches[0].pageY,"BODY"===S.offsetParent.tagName.toUpperCase()?(L-=S.offsetLeft,F-=S.offsetTop):(L-=S.offsetParent.offsetLeft,F-=S.offsetParent.offsetTop),l.clearRect(0,0,S.width,S.height),E(),W(),StickStatus.xPosition=L,StickStatus.yPosition=F,StickStatus.x=((L-P)/x*100).toFixed(),StickStatus.y=((F-m)/x*100*-1).toFixed(),StickStatus.cardinalDirection=T(),i(StickStatus))},!1),document.addEventListener("touchend",function(t){f=0,h&&(L=P,F=m);l.clearRect(0,0,S.width,S.height),E(),W(),StickStatus.xPosition=L,StickStatus.yPosition=F,StickStatus.x=((L-P)/x*100).toFixed(),StickStatus.y=((F-m)/x*100*-1).toFixed(),StickStatus.cardinalDirection=T(),i(StickStatus)},!1)):(S.addEventListener("mousedown",function(t){f=1},!1),document.addEventListener("mousemove",function(t){1===f&&(L=t.pageX,F=t.pageY,"BODY"===S.offsetParent.tagName.toUpperCase()?(L-=S.offsetLeft,F-=S.offsetTop):(L-=S.offsetParent.offsetLeft,F-=S.offsetParent.offsetTop),l.clearRect(0,0,S.width,S.height),E(),W(),StickStatus.xPosition=L,StickStatus.yPosition=F,StickStatus.x=((L-P)/x*100).toFixed(),StickStatus.y=((F-m)/x*100*-1).toFixed(),StickStatus.cardinalDirection=T(),i(StickStatus))},!1),document.addEventListener("mouseup",function(t){f=0,h&&(L=P,F=m);l.clearRect(0,0,S.width,S.height),E(),W(),StickStatus.xPosition=L,StickStatus.yPosition=F,StickStatus.x=((L-P)/x*100).toFixed(),StickStatus.y=((F-m)/x*100*-1).toFixed(),StickStatus.cardinalDirection=T(),i(StickStatus)},!1)),E(),W(),this.GetWidth=function(){return S.width},this.GetHeight=function(){return S.height},this.GetPosX=function(){return L},this.GetPosY=function(){return F},this.GetX=function(){return((L-P)/x*100).toFixed()},this.GetY=function(){return((F-m)/x*100*-1).toFixed()},this.GetDir=T};
  </script>
  </head><body>
  <h3>Crab-bot Simple Control</h3>
<div style="width:100%;">
<div id="joyDiv" style="width:49%;display:inline-block;align:center;"></div>
<div id="joyDiv2" style="width:49%;display:inline-block;align:center;"></div>
</div>


<script>

  //this websocket code is borrowed from an online example that provides the necessary functionality
  //see original code here https://github.com/nkmakes/SMARS-esp32
        const view = document.getElementById('stream');
        const WS_URL = "ws://" + window.location.host + ":82";
        const ws = new WebSocket(WS_URL);

        ws.onmessage = message => {
            if (message.data instanceof Blob) {
                var urlObject = URL.createObjectURL(message.data);
                view.src = urlObject;
            }
        };
        var lastText, lastSend, sendTimeout;
        // limit sending to one message every 30 ms
        // https://github.com/neonious/lowjs_esp32_examples/blob/master/neonious_one/cellphone_controlled_rc_car/www/index.html
        function send(txt) {
            var now = new Date().getTime();
            if (lastSend === undefined || now - lastSend >= 30) {
                try {
                    ws.send(txt);
                    lastSend = new Date().getTime();
                    return;
                } catch (e) {
                    console.log(e);
                }
            }
            lastText = txt;
            if (!sendTimeout) {
                var ms = lastSend !== undefined ? 30 - (now - lastSend) : 30;
                if (ms < 0)
                    ms = 0;
                sendTimeout = setTimeout(() => {
                    sendTimeout = null;
                    send(lastText);
                }, ms);
            }
        }



dims = document.getElementById('joyDiv').offsetWidth 
joy1x = 0; 
joy1y = 0; 
joy2x = 0; 
joy2y = 0; 


var joy1 = new JoyStick('joyDiv',{
    // The ID of canvas element
    title: 'joystick',
    // width/height
    width: dims,
    height: dims,
    // Internal color of Stick
    internalFillColor: '#330000',
    // Border width of Stick
    internalLineWidth: 2,
    // Border color of Stick
    internalStrokeColor: '#FF0000',
    // External reference circonference width
    externalLineWidth: 2,
    //External reference circonference color
    externalStrokeColor: '#000000',
    // Sets the behavior of the stick
    autoReturnToCenter: true
    
},
function(stickData) {
    joy1x = stickData.x; 
    joy1y = stickData.y; 
});

var joy2 = new JoyStick('joyDiv2',{
    // The ID of canvas element
    title: 'joystick',
    // width/height
    width: dims,
    height: dims,
    // Internal color of Stick
    internalFillColor: '#000033',
    // Border width of Stick
    internalLineWidth: 2,
    // Border color of Stick
    internalStrokeColor: '#0000FF',
    // External reference circonference width
    externalLineWidth: 2,
    //External reference circonference color
    externalStrokeColor: '#000000',
    // Sets the behavior of the stick
    autoReturnToCenter: true
    
},
function(stickData) {
    joy2x = stickData.x; 
    joy2y = stickData.y; 
});

//periodic sending of controls information to crab bot through the websocket (10Hz)
setInterval(function () { 
send(joy1x + ',' + joy1y + ',' + joy2x + ',' + joy2y); 
}, 100);


</script>
</body></html>

)rawliteral";

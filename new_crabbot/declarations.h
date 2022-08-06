#include <Arduino.h>
#include "Arduino.h"
#include <WiFi.h>
#include <esp_now.h>
#include <Adafruit_NeoPixel.h>
#include <makers_controller.h>
#include "Adafruit_Soundboard.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

/*********************************************
 *           MAKERS CONTROLLER 
 *********************************************/
//create receiver object that is going to be our way to get data
MakersController receiver = MakersController();

//soundboard connection (Serial2) 
//soundboard TX -> ESP32 16
//soundboard RX -> ESP32 17
#define SOUND_BOARD_RST 26

Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial2, NULL, 26);

//motor driver connections
#define FL_DIR 14
#define FL_PWM 13
#define BL_DIR 18
#define BL_PWM 15
#define FR_DIR 25
#define FR_PWM 23
#define BR_DIR 19
#define BR_PWM 12

/***********************************
 *         PWM SERVO BOARD
 ***********************************/

#define I2C_SDA 21
#define I2C_SCL 22

/***********************************
 *    MOTORS and DRIVE CONSTANTS
 ***********************************/
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

unsigned long last_motor_update = 0; 

boolean crab_rave_mode = false;


/*********************************************
 *                NEOPIXEL 
 *********************************************/
//for each animation define a constant that refers to that animation in the play_animation() function
#define CRABBOT_STANDARD_ANIMATION 0
#define CRABBOT_SPLIT_ANIMATION 1
#define CRABBOT_CONTROLS_ANIMATION 2
 
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
#define NEOPIXEL_PIN 27
#define NUM_NEOPIXEL 60
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_NEOPIXEL, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

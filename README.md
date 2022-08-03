# Crab-Bot - Mascot of Makers @ OU
![crab bot](https://github.com/Makers-Oakland-University/readme-images/blob/main/crabbot7.15.2022.jpg?raw=true)

Crab-bot is a fun interactive robot running on 4 motors and mecanum wheels. He waves his hands wherever he goes and brings joy to everyone around him.

# Crab-bot Version 2
We're currently working to rebuild and re-imagine crab-bot with the ESP32 at his core to make room for additional features. New crab-bot will include all the features of the original crab-bot with the addition of: more sounds, neopixel lights, and pre-programmed maneuvers/dance. All with a focus on leaving space for additional hardware and features in the future that anyone in makers can add!

This repo contains all the source code/files used in crab-bot

## Installation

In order to compile the arduino code for Crab-Bot you'll need to install the ESP32 Arduino Core, a guide for installation can be found [Here](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/).

A number of additional libraries are also required and can be found at the following links: 
* [Makers Unified Controller Arduino Library](https://github.com/Makers-Oakland-University/Makers-Unified-Controller-Arduino)
* [Adafruit Neopixel Library](https://github.com/adafruit/Adafruit_NeoPixel)
* [Adafruit Soundboard](https://github.com/adafruit/Adafruit_Soundboard_library)
* [Adafruit PWM Servo Driver](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)


## Contributing
Makers at OU welcomes all OU students to join to work on fun and interactive projects. Contact us on [grizzorgs](https://oaklandu.campuslabs.com/engage/organization/makers-at-oakland-university)

# Crab-bot Version 1 
Crab-bot is in the process of being rebuilt, the previous version files are present in this repo. Crab-bot V1 was built around the Arduino Mega which interfaces to 4 dedicated motor drivers, one for each wheel. An xbox controller was used as user input to drive crab bot. Two servos run crab-bot's claws. 
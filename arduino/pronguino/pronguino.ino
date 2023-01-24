/**
  pronguino.ino - Main sketch

  Parts required:
  - 2 x 10k Ohm potentiometers
  - 2 x Switches
  - 2 x 10k Ohm resistors
  - 2 x Yellow LEDs
  - 2 x 220 Ohm resistor
  - 1 x Piezo

  @author  Neil Deighan

*/

// Include the Controller library
#include "Pronguino.h"

// Define constants
const int CONTROLLER_COUNT = 2;
const int CONTROLLER_ONE = 0;
const int CONTROLLER_TWO = 1;

// Define analog pins
const int CONTROLLER_ONE_POT_PIN = A1;
const int CONTROLLER_TWO_POT_PIN = A2;

// Define digital pins for controllers
const int CONTROLER_ONE_BUTTON_PIN = 2;
const int CONTROLER_TWO_BUTTON_PIN = 3;
const int CONTROLER_ONE_LED_PIN = 4;
const int CONTROLER_TWO_LED_PIN = 5;

// Define digital pin for speaker
const int SPEAKER_PIN = 10;

// Special constant to turn on / off testing mode
const bool TESTING = false;

// Declare Game Console
GameConsole gameConsole;

// Declare controllers
Controller* controllers;

// Declare Controller Values
int controllerValue1;
int controllerValue2;
int previousControllerValue1;
int previousControllerValue2;

// Declare Button States
int buttonState1;
int buttonState2;
int previousButtonState1;
int previousButtonState2;

// Declare data
byte data;


/**
  Setup the serial communications and controllers
*/
void setup() {

  // Create Game Console
  gameConsole = GameConsole(SPEAKER_PIN);

  // Add controllers to console
  gameConsole.addController(CONTROLLER_ONE_POT_PIN, CONTROLER_ONE_BUTTON_PIN, CONTROLER_ONE_LED_PIN);
  gameConsole.addController(CONTROLLER_TWO_POT_PIN, CONTROLER_TWO_BUTTON_PIN, CONTROLER_TWO_LED_PIN);

  // Get Controllers
  controllers = gameConsole.controllers();

  // Initialize serial communication
  Serial.begin(9600);

  // Print debug headers
  if (TESTING) {
    resultsHeader();
  }

  // Read Initial Values
  readValues();

  // Create Initial Data
  data = createData(controllerValue1, controllerValue2, buttonState1, buttonState2);

  // Send Initial Data
  if (!TESTING) {
    // Write the data to serial port
    Serial.write(data);
  }

}

/**
  Continuously read/write controller values
*/
void loop() {

  readValues();

  // If either of the controller/button values have changed ...
  if (valuesChanged() == true) {

    // Create data byte to send
    data = createData(controllerValue1, controllerValue2, buttonState1, buttonState2);

    if (!TESTING) {
      // Write the data to serial port
      Serial.write(data);
    }

    // Set Previous Values
    previousControllerValue1 = controllerValue1;
    previousControllerValue2 = controllerValue2;

    // Set Previous Button States
    previousButtonState1 = buttonState1;
    previousButtonState2 = buttonState2;

    if (TESTING) {
      // Check the LED's working, by hitting a button
      controllers[CONTROLLER_ONE].writeLedState(buttonState1);
      controllers[CONTROLLER_TWO].writeLedState(buttonState2);

      // Check piezo .. when left button ..
      if(buttonState1 == HIGH) {
        gameConsole.speaker().wallSound();
      }

      // ... right button
      if(buttonState2 == HIGH) {
        gameConsole.speaker().paddleSound();
      }
      
      // ... both buttons pressed
      if(buttonState1 == HIGH && buttonState2 == HIGH) {
        gameConsole.speaker().pointSound();
      }

    }


    // Small Delay
    delay(100);

  }
}

/**
  Called when data received from serial port
*/
void serialEvent() {

  while (Serial.available()) {
    // Read Data
    byte dataIn = (byte)Serial.read();

    // Get State
    int state = bitRead(dataIn, 0);   // Add constants

    // Get Index
    int index = bitRead(dataIn, 1);

    // Set LED State of required controller
    controllers[index].writeLedState(state);

    // Check if wall sound requested
    if(bitRead(dataIn,2) == 1) {
      gameConsole.speaker().wallSound();
    }

    // Check if paddle sound requested
    if(bitRead(dataIn,3) == 1) {
      gameConsole.speaker().paddleSound();
    }

    // Check if point sound requested
    if(bitRead(dataIn,4) == 1) {
      gameConsole.speaker().pointSound();
    }


    
  }
}

/**
  Read values from controllers
*/
void readValues() {

  // Read the values from each controller
  controllerValue1 = controllers[CONTROLLER_ONE].readValue();
  controllerValue2 = controllers[CONTROLLER_TWO].readValue();

  // Read the state from buttons
  buttonState1 = controllers[CONTROLLER_ONE].readButtonState();
  buttonState2 = controllers[CONTROLLER_TWO].readButtonState();

}

/**
  Checks if any values have changed

  @return true if any values have changed
*/
bool valuesChanged() {
  return (controllerValue1 != previousControllerValue1 || controllerValue2 != previousControllerValue2 ||
          buttonState1 != previousButtonState1 || buttonState2 != previousButtonState2);
}

/**
  Create the data to be written to serial port

  @param  controllerValue1  
  @param  controllerValue2  
  @param  buttonState1
  @param  buttonState2
  
  @return data 
*/
byte createData(int controllerValue1, int controllerValue2, int buttonState1, int buttonState2)
{

  // Convert to nibbles
  byte highNibble = (byte) (controllerValue1 | buttonState1 << 3) << 4;
  byte lowNibble  = (byte)  controllerValue2 | buttonState2 << 3;

  // Combine the two nibbles to make a byte
  byte data = highNibble | lowNibble;

  if (TESTING) {
    // Print results detail
    resultsDetail(controllerValue1, buttonState1, highNibble, controllerValue2, buttonState2, lowNibble, data);
  }

  return data;

}

/**
   Print Results Header to Serial Monitor
*/
void resultsHeader() {

  // Controller #1
  Serial.print("controllerValue1\t");
  Serial.print("buttonState1\t");
  Serial.print("highNibble\t");

  // Controller #2
  Serial.print("controllerValue2\t");
  Serial.print("buttonState2\t");
  Serial.print("lowNibble\t");

  Serial.println("data");

}

/**
   Print Results Detail to Serial Monitor

  @param  controllerValue1  
  @param  buttonState1
  @param  highNibble
  @param  controllerValue2  
  @param  buttonState2
  @param  lowNibble
  @param  data
*/
void resultsDetail(int controllerValue1, int buttonState1, byte highNibble, int controllerValue2, int buttonState2, byte lowNibble, byte data) {

  // Controller #1
  Serial.print(controllerValue1); Serial.print("\t\t\t");
  Serial.print(buttonState1); Serial.print("\t\t");
  Serial.print(highNibble); Serial.print("\t\t");

  // Controller #2
  Serial.print(controllerValue2); Serial.print("\t\t\t");
  Serial.print(buttonState2); Serial.print("\t\t");
  Serial.print(lowNibble); Serial.print("\t\t");

  Serial.println(data);

}

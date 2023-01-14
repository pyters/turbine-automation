/*
  General comments here
*/

// === LIBRARIES
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === PIN MAPPING
#define speedSensorPin 2
#define speedReferencePin 54
#define startButtonPin 19
#define stopButtonPin 18

// === CONSTANTS
#define displayAddress 0x27
#define displayCol 2
#define displayLin 16

// === GLOBAL VARIABLES

// == Speed sensor calculation
unsigned long speedSensorCount = 0;          // to count the pulses of the speed sensor
unsigned long speedSensorCountLastTime = 0;  // to be used as delta pulse count for speed calculationio
unsigned long speedSensorLastTime = 0;       // to be used as delta time for speed calculation
int dTime = 0;          // used in getActualSpeed function                
int dPulse = 0;         // used in getActualSpeed function
int speedLastValue = 0; // used in getActualSpeed function

// == LCD display
LiquidCrystal_I2C display(displayAddress, displayCol, displayLin);  // LCD display instance declaration
char line0[16];                              // buffer char vector for the line0 of the display
char line1[16];                              // buffer char vector for the line1 of the display

// == START and STOP push buttons
bool startButtonStatus = 0;
bool stopButtonStatus  = 0;

// == Swithes variables (NOT push button types)


// === SETUP, code executed one time since uC started
void setup() {

  Serial.begin(9600);  //if needs serial

  initSensors();    // sensors initialization
  initActuators();  // actuators initialization
}


// === LOOP, code executed in loop after the SETUP part
void loop() {

  setDisplay(getSpeedReference(), getActualSpeed(), 1);  // * SHALL BE INSIDE writeOutputs() after debugging...
  if (startButtonStatus){
    Serial.println("botao START pressionado");
    startButtonStatus = 0;
  }
  if (stopButtonStatus) {
    Serial.println("botao STOP pressionado");
    stopButtonStatus = 0;
  }   
  delay(500);
}

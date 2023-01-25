/*
  General comments here
*/

// === LIBRARIES
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === PIN MAPPING
#define speedSensorPin 2
#define speedReferencePin 61
#define startButtonPin 19
#define stopButtonPin 18
#define phaseFaultPin 17
#define controlModePin 16
#define finPosStopPin 69
#define finNegStopPin 68
//#define waterValvePosStopPin 67
//#define waterValveNegStopPin 66
//#define finPosSensorPin 62
#define motorFin1Pin 53
#define motorFin2Pin 51
#define ledStartPin 23
#define ledStopPin 25



// === CONSTANTS
#define displayAddress 0x27
#define displayCol 2
#define displayLin 16

// === GLOBAL VARIABLES

// == Speed sensor calculation
volatile unsigned int speedSensorCount = 0;          // to count the pulses of the speed sensor
unsigned long lastSpeedSensorCount = 0;  // to be used as delta pulse count for speed calculationio
unsigned long speedSensorLastTime = 0;       // to be used as delta time for speed calculation
int dTime = 0;          // used in getActualSpeed function                
int dPulse = 0;         // used in getActualSpeed function
int speedLastValue = 0; // used in getActualSpeed function

//int actualSpeed = 0;
//int refSpeed = 0;


// variables for push buttons
unsigned long lastStartDebouncingTime = 0;
unsigned long lastStopDebouncingTime = 0;
int buttonStartState =1;
int buttonStopState = 1;
int deboucingTime = 50; //in ms;


// == LCD display
LiquidCrystal_I2C display(displayAddress, displayCol, displayLin);  // LCD display instance declaration
char line0[16];                              // buffer char vector for the line0 of the display
char line1[16];                              // buffer char vector for the line1 of the display


// == Swithes variables (NOT push button types)

int temp;
// === SETUP, code executed one time since uC started
void setup() {

  Serial.begin(9600);  //if needs serial

  initSensors();    // sensors initialization
  initActuators();  // actuators initialization
}


// === LOOP, code executed in loop after the SETUP part
void loop() {
  
  delay(100);
  lcdDisplaySpeed();
 
 
  if(getStartButtonStatus()){
    setMotorFins(1); setStartLed(1);
    delay(2000);
    setMotorFins(0); setStopLed(0);
    delay(100);
  }
  if(getStopButtonStatus()){
    setMotorFins(-1); setStopLed(1); 
    delay(2000);
    setMotorFins(0); setStopLed(0);
    delay(100);
  }

  /*
 
  //Serial.println(getActualSpeed());
  
  //digitalWrite(ledStopPin, HIGH);
  //delay(1000);
  //digitalWrite(ledStopPin, LOW);
  //delay(1000);

  Serial.println("Sensors testing mode...");
  Serial.print("start button:                     "); Serial.println(getStartButtonStatus());
  Serial.print("stop button:                      "); Serial.println(getStopButtonStatus());
  Serial.print("fault condition:                  "); Serial.println(getPhaseFaultCondition());
  Serial.print("control mode:                     "); Serial.println(getControlMode());
  Serial.print("fin end travel sensor positive:   "); Serial.println(getFinEndSensorPositiveStatus());
  Serial.print("fin end travel sensor negative:   "); Serial.println(getFinEndSensorNegativeStatus());
  // Serial.print("water end travel sensor positive: "); Serial.println(getWaterValveEndSensorPositiveStatus());
  // Serial.print("water end travel sensor negative: "); Serial.println(getWaterValveEndSensorNegativeStatus());
  // Serial.print("fin position sensor:              "); Serial.println(getFinPosition());
  // Serial.println("-------");
  
  //setDisplay(getSpeedReference(), getActualSpeed(), 1);  // * SHALL BE INSIDE writeOutputs() after debugging...
  */

}

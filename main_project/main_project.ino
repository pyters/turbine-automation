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
#define phaseFaultPin 17
#define controlModePin 16
#define finPosStopPin 69
#define finNegStopPin 68
#define waterValvePosStopPin 67
#define waterValveNegStopPin 66
#define finPosSensorPin 62



// === CONSTANTS
#define displayAddress 0x27
#define displayCol 2
#define displayLin 16

// === GLOBAL VARIABLES

// == Speed sensor calculation
volatile unsigned int speedSensorCount = 0;          // to count the pulses of the speed sensor
//unsigned long speedSensorCountLastTime = 0;  // to be used as delta pulse count for speed calculationio
unsigned long speedSensorLastTime = 0;       // to be used as delta time for speed calculation
int dTime = 0;          // used in getActualSpeed function                
int dPulse = 0;         // used in getActualSpeed function
int speedLastValue = 0; // used in getActualSpeed function

int actualSpeed = 0;
int refSpeed = 0;

// == LCD display
LiquidCrystal_I2C display(displayAddress, displayCol, displayLin);  // LCD display instance declaration
char line0[16];                              // buffer char vector for the line0 of the display
char line1[16];                              // buffer char vector for the line1 of the display

// == START and STOP push buttons
volatile int startButtonStatus = 0;
volatile int stopButtonStatus  = 0;

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

  
  //Serial.println(stopButtonStatus);
  if (stopButtonStatus) {
    stopButtonStatus = 0;
    //Serial.println("botao STOP pressionado");
    temp = temp+1;
  }

  //Serial.println(startButtonStatus);
  if (startButtonStatus){
    startButtonStatus = 0;
    //Serial.println("botao START pressionado");
    temp = temp-1;
  }
  Serial.println(temp);
  
  delay(500);
  
  refSpeed = getSpeedReference();
  actualSpeed = getActualSpeed();
  setDisplay(refSpeed, actualSpeed, 1);  // * SHALL BE INSIDE writeOutputs() after debugging...
  
  Serial.println("Sensors testing mode...");
  Serial.print("fault condition:                  "); Serial.println(getPhaseFaultCondition());
  Serial.print("control mode:                     "); Serial.println(getControlMode());
  Serial.print("fin end travel sensor positive:   "); Serial.println(getFinEndSensorPositiveStatus());
  Serial.print("fin end travel sensor negative:   "); Serial.println(getFinEndSensorNegativeStatus());
  Serial.print("water end travel sensor positive: "); Serial.println(getWaterValveEndSensorPositiveStatus());
  Serial.print("water end travel sensor negative: "); Serial.println(getWaterValveEndSensorNegativeStatus());
  Serial.print("fin position sensor:              "); Serial.println(getFinPosition());
  Serial.println("-------");

  
  //setDisplay(getSpeedReference(), getActualSpeed(), 1);  // * SHALL BE INSIDE writeOutputs() after debugging...


}

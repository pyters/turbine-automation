/*
  General comments here
*/

// === LIBRARIES
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

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
#define motorFin1Pin 53 //49
#define motorFin2Pin 51 //47
#define ledStartPin 23
#define ledStopPin 25
#define contactorPin 39



// === CONSTANTS
#define displayAddress 0x27
#define displayCol 2
#define displayLin 16
#define N_MAGNETS 4

// === GLOBAL VARIABLES

// == Speed sensor calculation
volatile unsigned int speedSensorCount = 0;          // to count the pulses of the speed sensor
unsigned long lastSpeedSensorCount = 0;  // to be used as delta pulse count for speed calculationio
unsigned long speedSensorLastTime = 0;       // to be used as delta time for speed calculation
int dTime = 0;          // used in getActualSpeed function                
int dPulse = 0;         // used in getActualSpeed function
int speedLastValue = 0; // used in getActualSpeed function

int speedValue_k =0;    // used for filter;
int speedValue_kp =0;
float alpha = 0; // alpha value, zero for no-filter at all;

int actualSpeed = 0;
int sum; //sums the vector to get average after...

// moving average filter definitions

const int numReadings  = 6; // 1 for no filter condition
int readings [numReadings];
int readIndex  = 0;
long total  = 0;

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

// CONTROL variables
int controlActualSpeed;
int controlReferenceSpeed;
int controlError;
int controlHist = 100;
int controlHistDead = 50;
int controlLastState = 0;

// variables used in the ROUTINE.INO
unsigned long lastTimeMillis1s0 = 0;
unsigned long lastTimeMillis0s5 = 0;
unsigned long lastTimeMillis2s0 = 0;
unsigned long lastTimeMillis0s3 = 0;
int time1s0 = 1000;
int time0s5 = 500;
int time0s3 = 300;
int time2s0 = 2000;

int STATE = 0;  // 0 for INITIALIZATION
                // 1 for CONTROL
                // 2 for STOP

int stateAddressEEPROM = 1;
int temp_STATE_EEPROM = 0;



// == Swithes variables (NOT push button types)

int temp;



// === SETUP, code executed one time since uC started
void setup() {

  // Serial.begin(9600);  //if needs serial

  initSensors();    // sensors initialization
  initActuators() ;  // actuators initialization

  // SPECIAL CONDITION if a new arduino with epprom cleared is flashed.
  temp_STATE_EEPROM = EEPROM.read(stateAddressEEPROM);
  if (temp_STATE_EEPROM != 0 && temp_STATE_EEPROM != 1 && temp_STATE_EEPROM != 2 ){
    STATE = 0;
  }else{
    STATE = EEPROM.read(stateAddressEEPROM);
    
  }
  
  

}


// === LOOP, code executed in loop after the SETUP part
void loop() {
  
  // setMotorFins(1); 
  // STATE = -3;
  // Serial.println(STATE);

  EEPROM.update(stateAddressEEPROM, STATE);

  switch (STATE) {
    case 0:               // INITIALIZATION
      INITIALIZATION();
      break;

    case 1:
      CONTROL();
      break;

    case 2:               // STOP
      STOP();
      break;

    // BELOW CASES ARE ONLY FOR DEBUG;
    case -1: //DEBU
      // if ((millis()-lastTimeMillis0s5) > 500){
        
      //   lastTimeMillis0s5 = millis();

      //   actualSpeed = getActualSpeed();
      //   lcdDisplaySpeed(actualSpeed);
      //   // Serial.println(STATE);
      //   }
      display.noBacklight();
      break;
    case -2:
      setMotorFins(1); 
      break;

    case -3:
      setContactor(0);
      setStopLed(0);
      delay(2000);
      
      setContactor(1);
      setStopLed(1);
      delay(2000);
      
  }


/*
  //setMotorFins(-1);
 
    controlActualSpeed = getActualSpeed();
    controlReferenceSpeed = getSpeedReference();
    controlError = controlReferenceSpeed - controlActualSpeed;
    //Serial.println(controlError);
    //Serial.println(controlLastState);
  //  setMotorFins(1);

  lcdDisplaySpeed(controlActualSpeed);
  delay(500);

  if((controlError > controlHist))
    if((controlLastState == 0)){
        controlLastState = 1;
        setMotorFins(1);
        delay(1000);
        Serial.println("entrou 1");
  }
  if((controlError < 0))
    if(controlLastState == 1) {
      controlLastState = 0;
      setMotorFins(0);
      delay(1000);
      Serial.println("entrou 2");
  }
  if((controlError < -1*controlHist))
    if((controlLastState == 0)){
      controlLastState = -1;
      setMotorFins(-1);
      delay(1000);
      Serial.println("entrou 3");
  }
  if((controlError > 0))
    if((controlLastState == -1)){
      controlLastState = 0;
      setMotorFins(0);
      delay(1000);
      Serial.println("entrou 4");
  }
  Serial.print("control error=    "); Serial.println(controlError);
  Serial.print("controlLastState= "); Serial.println(controlLastState);
   
   
   /* 
    if((controlError > controlHist)){
        if(controlLastState != 1){
            Serial.println("entrou");
            controlLastState = 1;
            //setMotorFins(0); delay(100);
            setMotorFins(1);
        }
    }
    if((controlError < controlHist)){
        if(controlLastState != -1){
            controlLastState = -1;
            //setMotorFins(0); delay(100);
            setMotorFins(-1);
        }
    }
    if((controlError < controlHist) && (controlError < controlHistDead)){
        if(controlLastState == 1){
          controlLastState = 0;
          setMotorFins(0);
        }
    }
    if((controlError > controlHist) && (controlError > controlHistDead)){
        if(controlLastState == -1){
          controlLastState = 0;
          setMotorFins(0);
        }
    }
     
    delay(1000);
    lcdDisplaySpeed();


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




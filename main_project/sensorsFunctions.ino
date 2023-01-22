/*
  This *.ino file deals with all functions that are related to sensors:
    > speedSensor;
    > speedRefence;
*/

/*
  Describe the initial behaviours of the sensors.
  This function is called in the setup() part of the main project code.
  Returns: void.
*/
void initSensors(){
  // no need to initiate the speed refence sensor since it is a analog input;

  // set the interruption pin for the speed sensor and the function to deal with it
  attachInterrupt(digitalPinToInterrupt(speedSensorPin), countSensorHallISR, RISING);

  // set interruption pin for the START button
  pinMode(startButtonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(startButtonPin), startButtonISR, FALLING);

  // set interruption pin for the STOP button
  pinMode(stopButtonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(stopButtonPin), stopButtonISR, FALLING);
  
  // initialization pin for the phase fault sensor;
  pinMode(phaseFaultPin, INPUT_PULLUP);
  
  // initialization pin control mode;
  pinMode(controlModePin, INPUT_PULLUP);

  // initialization pin for end of travel sensors of the FINs;
  pinMode(finPosStopPin, INPUT_PULLUP);
  pinMode(finNegStopPin, INPUT_PULLUP);

  // initialization pin for end of travel sensors of the WATER VALVE;
  pinMode(waterValvePosStopPin, INPUT_PULLUP);
  pinMode(waterValveNegStopPin, INPUT_PULLUP);

}

/*
  Deals with the interruption that the speed sensor is attached;.
  It simply increases the variable speedSensorCount each time is called for further calculation;
  Returns: void (since it is a interruption deal function)
*/
void countSensorHallISR(){
  speedSensorCount++;
}

/*
  Get the value of the speedReference sensor;
  Basically reads the analog of the Speed reference pin and make some small mathematical 
  processing in it;
  Returns: int, value of speed already converted in RPM;
*/
int getSpeedReference(){
  return analogRead(speedReferencePin)*2 - 1024;
}

/*
  Get the actual speed of turbine axis;
  Reads the pulses between this function call and calculate the speed;
  Returns: int, value of speed already converted in RPM;
*/
int getActualSpeed(){
  
  dTime  = millis() - speedSensorLastTime;
  dPulse = speedSensorCount;
  speedSensorCount = 0;

  speedLastValue = 60000*dPulse/dTime;
  //Serial.println(dPulse);
  //Serial.println(dTime);
  //Serial.println(speedLastValue);
  // updating the variables for next loop
  speedSensorLastTime = millis();
  //speedSensorCountLastTime = speedSensorCount;

  return speedLastValue;
}

/*
  Set the status of the startButtonStatus variable to 1 when is called.
  startButtonStatus variable must be trated and set again to 0
  Returns: void (ISR Interruption Service Routine);
*/
void startButtonISR(){
  startButtonStatus = 1;
  //Serial.println("entrou int");

}

/*
  Set the status of the stopButtonStatus variable to 1 when is called.
  stopButtonStatus variable must be trated and set again to 0
  Returns: void (ISR Interruption Service Routine);
*/
void stopButtonISR(){
  stopButtonStatus = 1;
}

/*
  Read and return the DIGITAL status of the phase fault sensor
  Returns:  0 if there is NO FAULT;
            1 if there is fault;
*/
int getPhaseFaultCondition(){
  return digitalRead(phaseFaultPin);
}

/*
  Read and return the DIGITAL status of the switch that controls the control mode
  Returns:  0 for MANUAL control mode; 
            1 for SPEED control mode;
*/
int getControlMode(){
  return digitalRead(controlModePin);
}

/*
  Read and return the DIGITAL status of the POSITIVE FIN TRAVEL END SENSOR
  Returns:  0 if sensor is no activated;
            1 if sensor is activated, ! MOTOR IS IN THE END OF THE TRAVEL !;
*/
int getFinEndSensorPositiveStatus(){
  return digitalRead(finPosStopPin);
}

/*
  Read and return the DIGITAL status of the NEGATIVE FIN TRAVEL END SENSOR
  Returns:  0 if sensor is no activated;
            1 if sensor is activated, ! MOTOR IS IN THE END OF THE TRAVEL !;
*/
int getFinEndSensorNegativeStatus(){
  return digitalRead(finNegStopPin);
}

/*
  Read and return the DIGITAL status of the POSITIVE WATER MOTOR TRAVEL END SENSOR
  Returns:  0 if sensor is no activated;
            1 if sensor is activated, ! MOTOR IS IN THE END OF THE TRAVEL !;
*/
int getWaterValveEndSensorPositiveStatus(){
  return digitalRead(waterValvePosStopPin);
}

/*
  Read and return the DIGITAL status of the NEGATIVE WATER MOTOR TRAVEL END SENSOR
  Returns:  0 if sensor is no activated;
            1 if sensor is activated, ! MOTOR IS IN THE END OF THE TRAVEL !;
*/
int getWaterValveEndSensorNegativeStatus(){
  return digitalRead(waterValveNegStopPin);
}

/*
  Get the value of the FIN POSITION sensor;
  Basically reads the analog of the fin position sensor and make convertion to the total course
  Returns: int, in percetange of the course;
*/
int getFinPosition(){
  return analogRead(finPosSensorPin);
}

/*
  Get if START BUTTON was pressed;
  It will return 1 if the Start Button was pressed, after that it will write zero on the variable
  Returns:  0 if the START BUTTON was NOT pressed;
            1 if the START BUTTON WAS PRESSED, after it sets the variable to zero.
*/
int getStartButtonStatus(){
  if(startButtonStatus){
    startButtonStatus = 0;
    return 1;
  }
  else {
    return 0;
  }
}

/*
  Get if STOP BUTTON was pressed;
  It will return 1 if the Stop Button was pressed, after that it will write zero on the variable
  Returns:  0 if the STOP BUTTON was NOT pressed;
            1 if the STOP BUTTON WAS PRESSED, after it sets the variable to zero.
*/
int getStopButtonStatus(){
  if(stopButtonStatus){
    stopButtonStatus = 0;
    return 1;
  }
  else {
    return 0;
  }
}
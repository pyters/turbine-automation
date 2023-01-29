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
  pinMode(startButtonPin, INPUT);
  //attachInterrupt(digitalPinToInterrupt(startButtonPin), startButtonISR, RISING);

  // set interruption pin for the STOP button
  pinMode(stopButtonPin, INPUT);
  //attachInterrupt(digitalPinToInterrupt(stopButtonPin), stopButtonISR, RISING);
  
  // initialization pin for the phase fault sensor;
  pinMode(phaseFaultPin, INPUT);
  
  // initialization pin control mode;
  pinMode(controlModePin, INPUT_PULLUP);

  // initialization pin for end of travel sensors of the FINs;
  pinMode(finPosStopPin, INPUT_PULLUP);
  pinMode(finNegStopPin, INPUT_PULLUP);

  // pinMode(contactorPin, INPUT);

  // initialization pin for end of travel sensors of the WATER VALVE;
  //pinMode(waterValvePosStopPin, INPUT_PULLUP);
  //inMode(waterValveNegStopPin, INPUT_PULLUP);

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
<<<<<<< Updated upstream
int getSpeedReference(){
  
  if(getControlMode()){
    return (analogRead(speedReferencePin)*-1 +1023);  
  }else{
    return 540;
  }
=======
int getReference(){
  return analogRead(speedReferencePin)*2 - 1024;
>>>>>>> Stashed changes
}

/*
  Get the actual speed of turbine axis;
  Reads the pulses between this function call and calculate the speed;
  Returns: int, value of speed already converted in RPM;
*/

int getActualSpeed(){
  
  dTime  = millis() - speedSensorLastTime;
  speedSensorLastTime = millis();

  dPulse = speedSensorCount-lastSpeedSensorCount;
  lastSpeedSensorCount = speedSensorCount;

  speedLastValue = (60000*dPulse/dTime)/N_MAGNETS;
  // Serial.print("dPulse "); Serial.println(dPulse);
  // Serial.print("dTime  "); Serial.println(dTime);
  //Serial.println(speedLastValue);
  // updating the variables for next loop
  //speedSensorCountLastTime = speedSensorCount;
  
  // speedValue_k = speedLastValue;
  // speedValue_k = alpha*speedValue_kp+(1+alpha)*speedValue_k;
  // speedValue_kp = speedValue_k;

  //Serial.print("speed  "); Serial.println(speedLastValue);
  //Serial.print("speed  "); Serial.println(speedValue_k);

  // sum -= samples[index]; // subtract the oldest sample from the sum
  // samples[index] = speedLastValue; // store the current sample
  // sum += speedLastValue; // add the current sample to the sum
  // index = (index + 1) % NUM_SAMPLES; // increment the index and wrap around
  //int average = sum / NUM_SAMPLES; // calculate the average
  // use the value of 'average' as needed  
  // Serial.print("raw speed "); Serial.println(speedLastValue);
  // Serial.print("average   "); Serial.println(sum / NUM_SAMPLES);
  
  // return sum / NUM_SAMPLES;

  total = total - readings[readIndex];
  readings[readIndex] = speedLastValue;

  for (int i = 0; i < numReadings; i++) {
    sum += readings[i]; // add the current element to the sum
  }

  // Serial.print(readings[0]); Serial.print(",");
  // Serial.print(readings[1]); Serial.print(",");
  // Serial.print(readings[2]); Serial.print(",");
  // Serial.print(readings[3]); Serial.print(",");
  // Serial.print(readings[4]); Serial.print(",");
  // Serial.println("");

  //total = total + readings[readIndex];

  // Serial.print("total depois "); Serial.println(total);

  readIndex = readIndex + 1;
  if(readIndex >= numReadings){ readIndex = 0; }
  
  //Serial.print("actual value avg  "); Serial.println(sum/numReadings);

  // return sum/numReadings;


  return speedLastValue;
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
  Returns:  0 for FIXED REFERENCE control mode; 
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
  Get if START BUTTON was pressed;
  It will return 1 if the Start Button was pressed, after that it will write zero on the variable
  Returns:  0 if the START BUTTON is PRESSED;
            1 if the START BUTTON is RELEASED.
*/
int getStartButtonStatus(){
  
  return !digitalRead(startButtonPin);
}

/*
  Get if STOP BUTTON was pressed;
  It will return 1 if the Stop Button was pressed, after that it will write zero on the variable
  Returns:  0 if the STOP BUTTON was NOT pressed;
            1 if the STOP BUTTON WAS PRESSED, after it sets the variable to zero.
*/
int getStopButtonStatus(){

    return !digitalRead(stopButtonPin);
}

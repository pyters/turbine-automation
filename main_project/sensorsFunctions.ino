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
unsigned long getActualSpeed(){
  
  dTime  = millis() - speedSensorLastTime;
  dPulse = speedSensorCount - speedSensorCountLastTime;

  speedLastValue = 60000*dPulse/dTime;
  //Serial.println(dPulse);
  //Serial.println(dTime);
  //Serial.println(speedLastValue);
  // updating the variables for next loop
  speedSensorLastTime = millis();
  speedSensorCountLastTime = speedSensorCount;

  return speedLastValue;
}

/*
  Set the status of the startButtonStatus variable to 1 when is called.
  startButtonStatus variable must be trated and set again to 0
  Returns: void (ISR Interruption Service Routine);
*/
void startButtonISR(){
  startButtonStatus = 1;
  //Serial.println("INT START");

}

/*
  Set the status of the stopButtonStatus variable to 1 when is called.
  stopButtonStatus variable must be trated and set again to 0
  Returns: void (ISR Interruption Service Routine);
*/
void stopButtonISR(){
  stopButtonStatus = 1;
  //Serial.println("INT STOP");
}
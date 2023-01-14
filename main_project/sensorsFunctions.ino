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
  attachInterrupt(digitalPinToInterrupt(speedSensorPin), countSensorHall, RISING);

}

/*
  Deals with the interruption that the speed sensor is attached;.
  It simply increases the variable speedSensorCount each time is called for further calculation;
  Returns: void (since it is a interruption deal function)
*/
void countSensorHall(){
  speedSensorCount++;
}

/*
  Get the value of the speedReference sensor;
  Basically reads the analog of the Speed reference pin and make some small mathematical 
  processing in it;
  Returns: int, value of speed already converted in RPM;
*/
int getSpeedReference(){
  int val;
  val = analogRead(speedReferencePin)*2 - 1024;
  return val;
}

/*
  Get the actual speed of turbine axis;
  Reads the pulses between this function call and calculate the speed;
  Returns: int, value of speed already converted in RPM;
*/
unsigned long getActualSpeed(){
  
  dTime  = millis() - speedSensorLastTime;
  dPulse = speedSensorCount - speedSensorCountLastTime;
  //Serial.println(dTime);
  //Serial.println(dPulse);
  //Serial.println(speedSensorCount);
  //Serial.println(speedSensorCountLastTime);

  speedLastValue = 60000*dPulse/dTime;
  //updating the last value of speed;
  //Serial.println(speedLastValue);
  
  // updating the variables for next loop
  speedSensorLastTime = millis();
  speedSensorCountLastTime = speedSensorCount;

  return speedLastValue;
}
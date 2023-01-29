/*
  This *.ino file deals with all functions that are related to ACTUATORS:
    > LCD sensor
*/


/*
  Describe the initial behaviours of the actuators.
  This function is called in the setup() part of the main project code.
  Returns nothing.
*/
void initActuators(){
  // set the initial configs for the LCD display
  // display.begin(displayCol, displayLin);
  display.init();       //  initializatiton of the LCD display
  display.backlight();  //  turn on the backlight of the display
  display.clear();      //  cleans the display
  //display.noBacklight();

  pinMode(motorFin1Pin, OUTPUT);
  digitalWrite(motorFin1Pin, HIGH);
  pinMode(motorFin2Pin, OUTPUT);
  digitalWrite(motorFin2Pin, HIGH);
  
  pinMode(ledStartPin, OUTPUT);
  digitalWrite(ledStopPin, LOW);
  pinMode(ledStopPin, OUTPUT);
  digitalWrite(ledStopPin, LOW);

  pinMode(contactorPin, OUTPUT);


}
// ---------------------- //

/*
  set the LCD display accordingly to the inputs.
  > refValue:     the reference value that will be displayed in the first line;
  > actualValue:  last value of the given reading
  > controlMode:  0-Manual mode, 1-Speed control mode. Only changes the display information (% or RPM)  
  Returns nothing.
*/

void lcdDisplaySpeed(int actualSpeed){
  // reading the reference and actual speed... 
  // By some reason when input it direcly in below sprintf function, the pushbuttom interruption does not works well...
  //int actualSpeed;
  int refSpeed;
  refSpeed = getSpeedReference();
  //actualSpeed = getActualSpeed();
  //Serial.println(actualSpeed);
  //actualSpeed = 208;

  //Serial.print("value input function    "); Serial.println(actualSpeed);

  sprintf(line0, "REF. [RPM]  %4u", refSpeed);  // use %03u for leading zero or %3u for no-leading zero.
  display.setCursor(0, 0);                      // set the cursor to line 0, column 0
  display.print(line0);                         // print the buffer line0
  //Serial.println(line0);
  // LINE 1 of the display formatting, same idea of the LINE 0
  sprintf(line1, "ROTACAO     %4u", actualSpeed);
  display.setCursor(0, 1);
  display.print(line1);
  //Serial.println(line1);
}


void lcdDisplayMessage(char lin0[16], char lin1[16]){
  
  display.clear();  
  
  display.setCursor(0, 0);                      // set the cursor to line 0, column 0
  display.print(lin0);                         // print the buffer line0
  //Serial.println(line0);
  // LINE 1 of the display formatting, same idea of the LINE 0
  display.setCursor(0, 1);
  display.print(lin1);
  //Serial.println(line1);

}

/*
  Set the direction of the FIN motor;
  1  - forward
  -1 - backwards
  0  - STOP
*/

void setMotorFins(int dir){
  if (dir == 1){
    // foward commands
    digitalWrite(motorFin1Pin, LOW);
    digitalWrite(motorFin2Pin, HIGH);
  }
  else if (dir == -1){
    digitalWrite(motorFin1Pin, HIGH);
    digitalWrite(motorFin2Pin, LOW);

  }else{
    digitalWrite(motorFin1Pin, HIGH);
    digitalWrite(motorFin2Pin, HIGH);
  }
}


/*
  SET the START LED state of the LED;
  1  - LED ON
  0  - LED OFF
*/

void setStartLed(int dir){
  if (dir == 1){
    digitalWrite(ledStartPin, HIGH);
  }
  else{
    digitalWrite(ledStartPin, LOW);
  }
}

/*
  SET the STOP LED state of the LED;
  1  - LED ON
  0  - LED OFF
*/

void setStopLed(int dir){
  if (dir == 1){
    digitalWrite(ledStopPin, HIGH);
  }
  else{
    digitalWrite(ledStopPin, LOW);
  }
}

/*
  SET the CONTACTOR to connect the gerator into the AC LINE;
  1  - LED ON
  0  - LED OFF
*/

void setContactor(int dir){
  if (dir == 1){
    digitalWrite(ledStopPin, HIGH);
  }
  else{
    digitalWrite(ledStopPin, LOW);
  }
}

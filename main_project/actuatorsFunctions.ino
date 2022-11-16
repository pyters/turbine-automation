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
  display.init();       //  initializatiton of the LCD display
  display.backlight();  //  turn on the backlight of the display
  display.clear();      //  cleans the display


}

/*
  Update the LCD display accordingly to the inputs.
  > refValue:     the reference value that will be displayed in the first line;
  > actualValue:  last value of the given reading
  > controlMode:  0-Manual mode, 1-Speed control mode. Only changes the display information (% or RPM)  
  Returns nothing.
*/

void updateDisplay(int refValue, int actualValue, bool controlMode){
  
  // First check which control mode the program is working on
  if (controlMode) {
    // if the program is working on SPEED CONTROL mode, show this in the display
    
    // LINE 0 of the display formatting
    sprintf(line0, "REF. [RPM]  %4u", refValue);  // use %03u for leading zero or %3u for no-leading zero.
    display.setCursor(0, 0);                      // set the cursor to line 0, column 0
    display.print(line0);                         // print the buffer line0
    
    // LINE 1 of the display formatting, same idea of the LINE 0
    sprintf(line1, "VELOCIDADE  %4u", actualValue);
    display.setCursor(0, 1);
    display.print(line1);
  } else {
    // if the program is working on MANUAL CONTROL mode, show this in the display

    // LINE 0 of the display formatting, same idea of the formatting for the speed control above
    sprintf(line0, "REF. [%%]     %03u", refValue); 
    display.setCursor(0, 0);
    display.print(line0);
    
    // LINE 1 of the display formatting, same idea of the LINE 0
    sprintf(line1, "POSICAO      %03u", actualValue);
    display.setCursor(0, 1);
    display.print(line1);
  } 

}

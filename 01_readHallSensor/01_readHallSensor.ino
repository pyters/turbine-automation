/*
This code is only used o read the digital input from the hall sensor and publish it in the serial.
*/

int hallSensorPin =  62;

void setup() {

Serial.begin(9600);
pinMode(hallSensorPin, INPUT_PULLUP);

}

void loop() {
  Serial.println(digitalRead(hallSensorPin));
  delay(1000);
}
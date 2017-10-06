//http://www.berryjam.eu/2014/07/advanced-arduino-i2c-communication/
// (c) 2014 Ignas Gramba
//
#include <Wire.h>

#define SensorPin 1
#define ledPin 13

const byte SlaveDeviceId = 9;
byte LastMasterCommand = 0;
byte state = 0;

void setup() {
  //Serial.begin(115200);
  Wire.begin(SlaveDeviceId);      // join i2c bus with Slave ID
  Wire.onReceive(receiveCommand); // register talk event
  Wire.onRequest(slavesRespond);  // register callback event
  pinMode(SensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  delay(100);
}

void receiveCommand(int howMany) {
  LastMasterCommand = Wire.read(); // 1 byte (maximum 256 commands)
}

void slavesRespond() {
  int returnValue = 0;

  switch (LastMasterCommand) {
    case 0:   // No new command was received
      Wire.write("NA");
      break;

    case 1:   // Return X sensor value
      returnValue = GetXSensorValue();
      break;

    case 8:   // Return Y sensor value
      returnValue = setPin();
      break;
  }

  byte buffer[2];
  buffer[0] = returnValue >> 8;
  buffer[1] = returnValue & 255;
  Wire.write(buffer, 2);
  LastMasterCommand = 0;          // null last Master's command
}

int GetXSensorValue() {
  int val = analogRead(SensorPin);
  return val;
}

int setPin() {
  state = !state;
  digitalWrite(ledPin, state);
  return state;
}

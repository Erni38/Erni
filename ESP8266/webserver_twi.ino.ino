//I2c code from: http://www.berryjam.eu/2014/07/advanced-arduino-i2c-communication/
// (c) 2014 Ignas Gramba
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>

ESP8266WebServer webserver(80);

#define SlaveDeviceId 9
int value;
int pinValue;

void handleRoot() {
  String arg0 = String(webserver.arg(0));
  String arg1 = String(webserver.arg(1));
  if (webserver.arg(0) == "1") {
    pinValue = setPin(); // slave toggles led
  }

  if (webserver.arg(0) == "2") {
    value = getValue(); // slave reads analog
  }
  String val = String(value);
  String pin = String(pinValue);
  String htmlDoc = "<!DOCTYPE html>";
  htmlDoc += "<html>";
  htmlDoc += "<head>";
  htmlDoc += "<title>TWI . Demo</title>";
  htmlDoc += "</head>";
  htmlDoc += "<body>";
  htmlDoc += "<br>Toggle led <a href='/?led=1'>click</a><br>";
  htmlDoc += "<br>Raed analog value <a href='/?led=2'>click</a><br>";
  htmlDoc += "<br>";
  htmlDoc += "pinStatus=" + pin + "  Analog value=" + val;
  htmlDoc += "<br>";
  htmlDoc += "</body>";
  htmlDoc += "</html>";
  webserver.send(200, "text/html", htmlDoc);
}

void handleNotFound() {
  webserver.send(404, "text/plain", "Page not found ...");
}

void setup() {
  Serial.begin(115200);
  Wire.begin(0, 2);       // join i2c bus sda,scl
  WiFi.begin("Erni", "ErNi2001");
  delay(10);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  delay(100);
  webserver.on("/", handleRoot);
  webserver.onNotFound(handleNotFound);
  webserver.begin();
  Serial.println("Web server has started on: ");
  Serial.print(WiFi.localIP());
}

void loop() {
  webserver.handleClient();
}

int setPin() {
  Wire.beginTransmission(SlaveDeviceId);
  Wire.write(8); // Transfer command ("8") to set pin command
  Wire.endTransmission();  // Moved up here by frippe75
  delay(100);
  // GET RESPONSE
  int receivedValue = 0;
  Wire.requestFrom(SlaveDeviceId, 2);
  receivedValue =  Wire.read() << 8 | Wire.read(); // combine two bytes into integer
  int error = Wire.endTransmission();
  Serial.println(error);
  return receivedValue;
}

int getValue() {
  Wire.beginTransmission(SlaveDeviceId);
  Wire.write(1); // Transfer command ("1") to get X sensor value;
  Wire.endTransmission();  // Moved up here by frippe75
  delay(100);
  // GET RESPONSE
  int receivedValue1 = 0;
  Wire.requestFrom(SlaveDeviceId, 2);
  receivedValue1 = Wire.read() << 8 | Wire.read(); // combine two bytes into integer
  int error = Wire.endTransmission();
 
  return receivedValue1;
}

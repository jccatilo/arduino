#include <SoftwareSerial.h>
SoftwareSerial esp(0,1); // RX, TX

String ssid ="PLDTMyDSL";
String password="pldtwifiB679B";
String mode ="1";

void setup() {
  // put your setup code here, to run once:
esp.begin(115200);
Serial.begin(115200);
String
String cmd = "AT+CWJAP=\""+ssid+"\",\""+password+"\"";
Serial.println(cmd);
esp.println(cmd);
delay(4000);
if(esp.find("OK")) {
Serial.println("Connected!");
}
else {
Serial.println("Cannot connect to wifi. jerk.");
}

}

void loop() {
  // put your main code here, to run repeatedly:

}

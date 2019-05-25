#include "ESP8266WiFi.h"

const char* ssid = "trackemall";
const char* password = "password";

void setup()
{
Serial.begin(9600);
 Serial.println("Yo.");
 WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
 
}


void loop () {
   
 // long rssi = WiFi.RSSI();
 // Serial.print("RSSI:");
  //Serial.println(rssi);
 // delay(1000);
  }

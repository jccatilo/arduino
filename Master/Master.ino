/*
 * NodeMCU/ESP8266 act as AP (Access Point) and simplest Web Server
 * Connect to AP "arduino-er", password = "password"
 * Open browser, visit 192.168.4.1
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

const char *ssid = "trackemall";
const char *password = "password";
//D1 ay 5, D2 ay 4, D0 ay 16, D3 ay 0
int pinone= 5;
int pintwo = 4;
int pinthree = 16;
int pinfour = 0;

ESP8266WebServer server(80);

void handleRoot() {
    server.send(200, "text/html", "<h1>Hello! from Track'EmAll!</h1>");
}

char* htmlBody_help = "<h1>Help</h1><br/>\n"
  "Visit http://192.168.4.1/ to access web server.<br/>\n"
  "Visit http://192.168.4.1/help to access this page.<br/>\n";

void handleHelp(){
  server.send(200, "text/html", htmlBody_help);
}

void setup() {
    delay(1000);
    Serial.begin(9600);
    pinMode(pinone,OUTPUT);
    pinMode(pintwo,OUTPUT);
    pinMode(pinthree,OUTPUT);
    pinMode(pinfour,OUTPUT);
    
    Serial.println();
    WiFi.softAP(ssid, password);
    IPAddress apip = WiFi.softAPIP();
    Serial.print("visit: \n");
    Serial.println(apip);
    server.on("/", handleRoot);
    server.on("/help", handleHelp);
    server.begin();
    Serial.println("HTTP server started");
}


void loop() { 
  
  
  if ((WiFi.softAPgetStationNum())==0){
  digitalWrite(pinone, LOW);
  digitalWrite(pintwo, LOW);
  digitalWrite(pinthree, LOW);
  digitalWrite(pinfour, HIGH);
  delay(500);
  }
  
  else if ((WiFi.softAPgetStationNum())==1){
  
  digitalWrite(pinone, HIGH);
  digitalWrite(pintwo, LOW);
  digitalWrite(pinthree, LOW);
  digitalWrite(pinfour, HIGH);
  delay(500);
  }

  else if ((WiFi.softAPgetStationNum())==2){
  
  digitalWrite(pinone, HIGH);
  digitalWrite(pintwo, HIGH);
  digitalWrite(pinthree, LOW);
  digitalWrite(pinfour, LOW);
  delay(500);
  }

  else if ((WiFi.softAPgetStationNum())==3){
  
  digitalWrite(pinone, HIGH);
  digitalWrite(pintwo, HIGH);
  digitalWrite(pinthree, HIGH);
  delay(500);
  }
  
  Serial.print("Connected devices: ");
  Serial.println(WiFi.softAPgetStationNum());
 
  //  Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  //Serial.print("Devices Connected: ");
 // Serial.println(WiFi.softAPgetStationNum(), DEC);
     
  delay(500);

server.handleClient();
    
}

